; Yasar Cahit Yildirim / 2310647
;
; THE2_v2, simulation environment.
; I have not tried it on a board because I don't have one.
; I started the term late, because of erasmus cancellation. Due to pandemic.
; Thats why I did the homework alone.
;
; But if necessary configurations are set, i believe only
; 7-segment displays wouldn't work on a board,
; and that's because there is no delay between them.

#include <p18f8722.inc>

radix       dec                         ; number format is decimal by default
errorlevel  -207                        ; disable indentation warnings for labels

RES_VECT    CODE    0x0000              ; processor reset vector
    GOTO    START                       ; go to beginning of program

ISR_VECT    CODE    0x0008              ; interrupt vector location
    BTFSC   INTCON, 2, A                ; is this a timer interrupt
    GOTO    TIMER0_ISR                  ; handle timer0 interrupt
    RETFIE  FAST

MAIN_PROG   CODE                        ; let linker place main program

flagAdr       = 0x20                    ; interrupt ready flag
blockPosAdr   = 0x21                    ; block's left side variable address
levelAdr      = 0x22                    ; level             variable address
healthAdr     = 0x23                    ; health            variable address
T0IntrCntAdr  = 0x24                    ; timer0 software post-scaler counter
T0InitValAdr  = 0x25
T1HighAdr     = 0x26                    ; 16byte timer1's initial value, high byte
T1LowAdr      = 0x27                    ; 16byte timer1's initial value, low  byte
allBallCnt    = 0x28

flagIntBit   = 0
flagBOverBit = 1
flagGOverBit = 2

T0Level1InitVal =  61                   ; 256* 95*100 = 4,992,000 nearly   5M/500ms
T0Level2InitVal = 100                   ; 256*156*100 = 3,993,600 nearly   4M/400ms
T0Level3InitVal = 119                   ; 256*137*100 = 3,507,200 nearly 3.5M/350ms


START
    NOP

PREPARE_PORTS
    MOVLW   0x0F                        ; configure PORTA A/D
    MOVWF   ADCON1                      ; all pins digital

    MOVLW   b'00000000'
    CLRF    PORTA
    ANDWF   TRISA
    CLRF    PORTB
    ANDWF   TRISB
    CLRF    PORTC
    ANDWF   TRISC
    CLRF    PORTD
    ANDWF   TRISD                       ; A0-A5 through D0-D5 for screen output

    MOVLW   b'00000000'
    CLRF    PORTH
    ANDWF   TRISH
    CLRF    PORTJ
    ANDWF   TRISJ                       ; all H and J for 7-segment output

    MOVLW   b'00001101'
    CLRF    PORTG
    IORWF   TRISG                       ; RG3(left) & RG2(right) for movement input

PREPARE_INTERRUPTS
    CLRF    INTCON
    CLRF    INTCON2

    MOVLW   b'01000111'                 ; disable timer0 for now
    MOVWF   T0CON                       ; 8bit, prescaler 256
    CLRF    TMR0L                       ; clear timer0 low byte

    MOVLW   b'00000001'                 ; enable timer1 and let it count
    MOVWF   T1CON                       ; 16bit, no prescaler and no interrupt
    CLRF    TMR1L                       ; clear timer1 low byte
    CLRF    TMR1H                       ; clear timer1 high byte

    MOVLW   b'10100000'
    MOVWF   INTCON                      ; GIE<7> and TMR0IE<5> set

INIT_STATS
    CLRF    flagAdr                     ; no flag is set initially
    CLRF    allBallCnt                  ; generated ball count is 0 initially
    CLRF    T0IntrCntAdr                ; timer 0 counter needs to be 0
    CLRF    blockPosAdr                 ; initial block position is 00
    MOVLW   T0Level1InitVal             ; for level one, starting value
    MOVWF   T0InitValAdr                ; is 100 for the timer0
    MOVLW   1
    MOVWF   levelAdr, A
    CALL    SET_LEVEL_W_7SEG            ; level is 1 initially
    MOVLW   5
    MOVWF   healthAdr, A
    CALL    SET_HEALTH_W_7SEG           ; health is 5 initially
    BSF     PORTA, 5, A                 ; initially block is
    BSF     PORTB, 5, A                 ; 00, so A and B

WAIT_FOR_START
    BTFSS   PORTG, 0, A                 ; is RG0(start) pressed
    GOTO    WAIT_FOR_START              ; RG0(start) not pressed
START_RELEASE
    BTFSC   PORTG, 0, A                 ; is RG0(start) released
    GOTO    START_RELEASE               ; RG0(start) not released

    BCF     T1CON, 0, A                 ; disable tmr1, only initial value is needed
    MOVFF   TMR1L, T1LowAdr             ; save tmr1 low byte
    MOVFF   TMR1H, T1HighAdr            ; save tmr1 high byte

    CALL    UPDATE_STATE
    MOVFF   T0InitValAdr, TMR0          ; initialize timer0 with level 1 value
    BSF     T0CON, 7, A                 ; enable timer0 and start game

GAME_LOOP
    BTFSC   flagAdr, flagIntBit, A
    CALL    UPDATE_STATE
    BTFSC   flagAdr, flagGOverBit, A
    GOTO    ENDGAME

    CALL    SET_LEVEL_W_7SEG
    CALL    SET_HEALTH_W_7SEG
LEFT_TEST
    BTFSS   PORTG, 3, 0                 ; is RG3(left) pressed
    GOTO    RIGHT_TEST                  ; no left button press
    GOTO    LEFT_PRESSED                ; RG3(left) is pressed
RIGHT_TEST
    BTFSS   PORTG, 2, 0                 ; is RG2(right) pressed
    GOTO    GAME_LOOP                   ; no right button press
    GOTO    RIGHT_PRESSED               ; RG2(right) is pressed

;;;;;;;;;;;;;;;;;;;; MOVE THE BLOCK ;;;;;;;;;;;;;;;;;;;;
RIGHT_PRESSED
    BTFSC   flagAdr, flagIntBit, A
    CALL    UPDATE_STATE
    BTFSC   flagAdr, flagGOverBit, A
    GOTO    ENDGAME

    CALL    SET_LEVEL_W_7SEG
    CALL    SET_HEALTH_W_7SEG

    BTFSC   PORTG, 2, A                 ; is RG2(right) released
    GOTO    RIGHT_PRESSED               ; RG2(right) not released
RIGHT_RELEASED
    BTFSC   blockPosAdr, 1, A           ; if second bit is set
    GOTO    MOVE_RETURN                 ; block is at right bound(2), don't move
    CALL    CLEAR_BLOCK
    INCF    blockPosAdr, f, A           ; set new block position (+1)
    CALL    SHOW_BLOCK                  ; adjust output and go back to loop
    GOTO    MOVE_RETURN

LEFT_PRESSED
    BTFSC   flagAdr, flagIntBit, A
    CALL    UPDATE_STATE
    BTFSC   flagAdr, flagGOverBit, A
    GOTO    ENDGAME

    CALL    SET_LEVEL_W_7SEG
    CALL    SET_HEALTH_W_7SEG

    BTFSC   PORTG, 3, 0                 ; is RG3(left) released
    GOTO    LEFT_PRESSED                ; RG3(left) not released
LEFT_RELEASED
    MOVF    blockPosAdr, f, A           ; set STATUS reg to check
    BTFSC   STATUS, Z, 0 ; TSTFSZ       ; if 00
    GOTO    MOVE_RETURN                 ; block is at left bound(0), don't move
    CALL    CLEAR_BLOCK
    DECF    blockPosAdr, f, A           ; set new block position (-1)
    CALL    SHOW_BLOCK
    GOTO    MOVE_RETURN
LEFT_RELEASED_01

MOVE_RETURN
    GOTO    GAME_LOOP                   ; handled button press
;;;;;;;;;;;;;;;;;;;; BLOCK ROUTINES ;;;;;;;;;;;;;;;;;;;;
CLEAR_BLOCK                             ; clear current block
    DECF    blockPosAdr, w, A           ; decrement to test around 0
    BTFSC   STATUS, Z, A                ; if 00, was 01
    BRA     CLEAR_BLOCK_01
    BTFSC   STATUS, N, A                ; if -1, was 00
    BRA     CLEAR_BLOCK_00
                                        ; if 01, was 10
CLEAR_BLOCK_10
    BCF     PORTC, 5, A
    BCF     PORTD, 5, A
    BRA     SHOW_BLOCK_END
CLEAR_BLOCK_01
    BCF     PORTB, 5, A
    BCF     PORTC, 5, A
    BRA     SHOW_BLOCK_END
CLEAR_BLOCK_00
    BCF     PORTA, 5, A
    BCF     PORTB, 5, A
CLEAR_BLOCK_END
    RETURN

SHOW_BLOCK                              ; show block output
    DECF    blockPosAdr, w, A           ; decrement to test around 0
    BTFSC   STATUS, Z, A                ; if 00, was 01
    BRA     SHOW_BLOCK_01
    BTFSC   STATUS, N, A                ; if -1, was 00
    BRA     SHOW_BLOCK_00
                                        ; if 01, was 10
SHOW_BLOCK_10
    BSF     PORTC, 5                    ; block: 0011
    BSF     PORTD, 5                    ; set C and D
    BRA     SHOW_BLOCK_END
SHOW_BLOCK_01
    BSF     PORTB, 5                    ; block: 0110
    BSF     PORTC, 5                    ; set B and C
    BRA     SHOW_BLOCK_END
SHOW_BLOCK_00
    BSF     PORTA, 5                    ; block: 1100
    BSF     PORTB, 5                    ; set A and B
SHOW_BLOCK_END
    RETURN
;;;;;;;;;;;;;;;;;;;; BLOCK ROUTINES ;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;; SET_7-SEGMENT ROUTINES ;;;;;;;;;;;;;;;;;;;;
SET_HEALTH_W_7SEG
    CLRF    PORTH
    BSF     PORTH, 3, 0                 ; select rightmost 7-segment display
    CLRF    PORTJ
    MOVF    healthAdr, w, A

    XORLW   1
    BZ      SEG7_1                      ; if health is 1
    XORLW   2^1
    BZ      SEG7_2                      ; if health is 2
    XORLW   3^2
    BZ      SEG7_3                      ; if health is 3
    XORLW   4^3
    BZ      SEG7_4                      ; if health is 4
    BRA     SEG7_5                      ; if health is 5

SET_LEVEL_W_7SEG
    CLRF    PORTH
    BSF     PORTH, 0, 0                 ; select leftmost 7-segment display
    CLRF    PORTJ
    MOVF    levelAdr, w, A

    XORLW   1                           ; if level is 1
    BZ      SEG7_1
    XORLW   2^1
    BZ      SEG7_2                      ; if level is 2
    BRA     SEG7_3                      ; if level is 3

SEG7_1
    MOVLW   0x06                        ; set W to 1 in 7-segment display format
    BRA     FINISH_7SEG
SEG7_2
    MOVLW   0x5B                        ; set W to 2 in 7-segment display format
    BRA     FINISH_7SEG
SEG7_3
    MOVLW   0x4F                        ; set W to 3 in 7-segment display format
    BRA     FINISH_7SEG
SEG7_4
    MOVLW   0x66                        ; set W to 4 in 7-segment display format
    BRA     FINISH_7SEG
SEG7_5
    MOVLW   0x6D                        ; set W to 5 in 7-segment display format
FINISH_7SEG
    MOVWF   PORTJ, 0                    ; show W on selected 7-segment display
    RETURN                              ; return from set_health/level routine
;;;;;;;;;;;;;;;;;;;; SET_7-SEGMENT ROUTINES ;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;; TIMER0 INTERRUPT ROUTINE ;;;;;;;;;;;;;;;;;;;;
TIMER0_ISR
    INCF    T0IntrCntAdr, f, A
    MOVF    T0IntrCntAdr, w, A
    SUBLW   100
    BTFSS   STATUS, Z
    BRA     TIMER0_ISR_EXIT
    CLRF    T0IntrCntAdr, A
    BSF     flagAdr, flagIntBit, A      ; set custom interrupt flag, refresh scene

TIMER0_ISR_EXIT
    BCF     INTCON, 2                   ; clear timer0 interrupt flag
    MOVFF   T0InitValAdr, TMR0          ; set timer0 to a generic value initially
    RETFIE  FAST                        ; return from interrupt with state recovery
;;;;;;;;;;;;;;;;;;;; TIMER0 INTERRUPT ROUTINE ;;;;;;;;;;;;;;;;;;;;


UPDATE_STATE
    BCF     flagAdr, flagIntBit, A
    CALL    CLEAR_BLOCK
    RLNCF   PORTA, f, A
    RLNCF   PORTB, f, A
    RLNCF   PORTC, f, A
    RLNCF   PORTD, f, A

    BTFSC   PORTA, 6, A
    CALL    DECREASE_HEALTH
    BTFSC   PORTB, 6, A
    CALL    DECREASE_HEALTH
    BTFSC   PORTC, 6, A
    CALL    DECREASE_HEALTH
    BTFSC   PORTD, 6, A
    CALL    DECREASE_HEALTH

    MOVLW   b'00111110'
    ANDWF   PORTA, f, A
    ANDWF   PORTB, f, A
    ANDWF   PORTC, f, A
    ANDWF   PORTD, f, A
    CALL    SHOW_BLOCK

    BTFSC   flagAdr, flagBOverBit, A
    BRA     UPDATE_WITHOUT_BALL

    MOVF    T1LowAdr, w, A
    ANDLW   b'00000011'
    BTFSC   STATUS, Z, A
    BRA     UPDATE_BALL_PORTA
    XORLW   1
    BTFSC   STATUS, Z, A
    BRA     UPDATE_BALL_PORTB
    XORLW   1^2
    BTFSC   STATUS, Z, A
    BRA     UPDATE_BALL_PORTC
    BRA     UPDATE_BALL_PORTD

UPDATE_BALL_PORTA
    BSF     PORTA, 0, A
    BRA UPDATE_BALL_CREATED
UPDATE_BALL_PORTB
    BSF     PORTB, 0, A
    BRA UPDATE_BALL_CREATED
UPDATE_BALL_PORTC
    BSF     PORTC, 0, A
    BRA UPDATE_BALL_CREATED
UPDATE_BALL_PORTD
    BSF     PORTD, 0, A

UPDATE_BALL_CREATED
    INCF    allBallCnt, f, A

    MOVF    levelAdr, w, A
    XORLW   1
    BZ      UPDATE_LEVEL1
    XORLW   1^2
    BZ      UPDATE_LEVEL2
    BRA     UPDATE_LEVEL3

UPDATE_LEVEL1
    MOVF    allBallCnt, w, A
    SUBLW   5
    BTFSC   STATUS, Z, A
    CALL    INCREASE_LEVEL
    CALL    RR16C_TIMER1
    BRA     UPDATE_EXIT
UPDATE_LEVEL2
    MOVF    allBallCnt, w, A
    SUBLW   15
    BTFSC   STATUS, Z, A
    CALL    INCREASE_LEVEL
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    BRA     UPDATE_EXIT
UPDATE_LEVEL3
    MOVF    allBallCnt, w, A
    SUBLW   30
    BTFSC   STATUS, Z, A
    BSF     flagAdr, flagBOverBit, A
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    CALL    RR16C_TIMER1
    BRA     UPDATE_EXIT

UPDATE_WITHOUT_BALL
    INCF    allBallCnt, f, A
    MOVLW   36
    SUBWF   allBallCnt, w, A
    BTFSC   STATUS, Z, A
    BSF     flagAdr, flagGOverBit, A
UPDATE_EXIT
    RETURN

INCREASE_LEVEL
    BTFSS   levelAdr, 1, A
    BRA     INCREASE_TO_LEVEL_2
    BRA     INCREASE_TO_LEVEL_3
INCREASE_TO_LEVEL_2
    MOVLW   T0Level2InitVal
    MOVWF   T0InitValAdr, A
    BRA     INCREASE_LEVEL_EXIT
INCREASE_TO_LEVEL_3
    MOVLW   T0Level3InitVal
    MOVWF   T0InitValAdr, A
INCREASE_LEVEL_EXIT
    INCF    levelAdr, f, A
    CALL    SET_LEVEL_W_7SEG
    RETURN

DECREASE_HEALTH
    DECF    healthAdr, f, A
    MOVF    healthAdr, w, A
    BTFSC   STATUS, Z, A
    BRA     HEALTH_FINISH
    CALL    SET_HEALTH_W_7SEG
    BRA     DECREASE_HEALTH_EXIT
HEALTH_FINISH
    BSF     flagAdr, flagGOverBit, A
DECREASE_HEALTH_EXIT
    RETURN

RR16C_TIMER1
    BCF     STATUS, C, A
    BTFSC   T1LowAdr, 0, A
    BSF     STATUS, C, A
    RRCF    T1HighAdr, f, A
    RRCF    T1LowAdr, f, A
    RETURN

ENDGAME
    GOTO START

END