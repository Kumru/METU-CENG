// 2310647
// Yasar Cahit Yildirim

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF
#pragma config WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF
#pragma config DEBUG = OFF

#include <xc.h>
#include "breakpoints.h"


union {
  unsigned char byte;
  struct
  {
    unsigned hsac   :1; // half second or adc complete
    unsigned bhig   :1; // button is high
    unsigned rb4h   :1; // rb4 button pressed
    unsigned gdir   :1; // guess direction hint
    unsigned rhid   :1; // result hidden/visible
    unsigned loss   :1; // lost the game to the time
    unsigned win    :1; // win the game by guess
    unsigned over   :1; // game over
  };
} FLAGS;

unsigned char const SEG7[10] = {
    0x3F,0x06,
    0x5B,0x4F,
    0x66,0x6D,
    0x7D,0x07,
    0x7F,0x6F
};



char current_guess = 0;

char blink_cnt = 0;

char t1_cnt = 0;
char t1_postscale = 0;        // 500ms with 16 bit timer1, 8 pre, 10 postscaler
const unsigned t1_init_val = 3036;

char t0_postscale = 0;
const unsigned t0_init_val = 64911;// 50ms with 8 bit timer0, 128 prescaler

signed char debounce_counter = -1;  // timer0 ticks every 2sec, 5 t0 tick

void __interrupt() ISR(void)
{
    if(PIR1bits.ADIF)   // AD Conversion interrupt
    {
        PIR1bits.ADIF = 0;
        adc_value = (ADRESH << 8 | ADRESL);
        adc_complete();
        FLAGS.hsac = 1;
        
        if(adc_value >= 0 && adc_value <= 102)          current_guess = 0;
        else if(adc_value > 102 && adc_value <= 204)    current_guess = 1;
        else if(adc_value > 204 && adc_value <= 306)    current_guess = 2;
        else if(adc_value > 306 && adc_value <= 408)    current_guess = 3;
        else if(adc_value > 408 && adc_value <= 510)    current_guess = 4;
        else if(adc_value > 510 && adc_value <= 612)    current_guess = 5;
        else if(adc_value > 612 && adc_value <= 714)    current_guess = 6;
        else if(adc_value > 714 && adc_value <= 816)    current_guess = 7;
        else if(adc_value > 816 && adc_value <= 918)    current_guess = 8;
        else if(adc_value > 918 && adc_value <= 1023)   current_guess = 9;
    }
    
    else if(PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        t1_postscale++;
        
        if(t1_postscale == 10)  // timer1 ISR
        {
            hs_passed();
            t1_postscale = 0;
            FLAGS.hsac = 1;
            
            if(FLAGS.win | FLAGS.loss)
            {
                if(++blink_cnt == 4)
                    FLAGS.over = 1;
                
                FLAGS.rhid = (blink_cnt & 1) ? 1:0; // on 1 and 3, disable 7seg
            }
            else if(++t1_cnt == 10)  // game time end
            {
                FLAGS.loss = 1;
                game_over();
            }
        }
        
        TMR1 = t1_init_val;
    }
    
    else if(INTCONbits.TMR0IF)   // timer0 ISR
    {
        INTCONbits.TMR0IF = 0;
        TMR0 = t0_init_val;
        
        if(++t0_postscale == 25)
        {
            ADCON0bits.GO = 1;
            t0_postscale = 0;
        }
        
        if(debounce_counter > -1)
        {
            debounce_counter--;
            
            if(debounce_counter == -1)
            {
                FLAGS.bhig = PORTBbits.RB4;
            }
        }
    }
    
    else if(INTCONbits.RBIF)
    {
        if(debounce_counter == -1)
        {
            if(PORTBbits.RB4 && !FLAGS.bhig)
            {
                FLAGS.rb4h = 1; // rb4_handled

                rb4_handled();
                debounce_counter = 5; // ~10ms delay
            }
            else if(!PORTBbits.RB4 && FLAGS.bhig)
            {
                debounce_counter = 5; // ~10ms delay
            }
        }
        
        //PORTB = 0;
        INTCONbits.RBIF = 0;
    }
    
    return;
}


void init(void)
{
    /* variables */
    current_guess = 0;
    blink_cnt = 0;
    t1_cnt = 0;
    t1_postscale = 0;
    t0_postscale = 0;
    debounce_counter = -1;
    
    FLAGS.byte = 0;
    
    
    if(PORTB != 0) PORTB = 0;
    TRISB = 0b00010000;     // RB4 input
    
    PORTJ = 0; PORTH = 0;
    TRISJ &= 0b00000000;
    TRISH &= 0b11110000;     // rightmost 7segment display
    
    PORTC = 0; PORTD = 0; PORTE = 0;
    TRISC &= 0b11110000;
    TRISD &= 0b11110000;     // hint arrows, lower 4 output
    TRISE &= 0b11110000;
    
    ADCON0 = 0b00110000;    // channel12, !GO, ADON
    ADCON1 = 0b00000000;    // VREF default, all ports analog
    ADCON2 = 0b10000010;    // left justified, , FOSC/32
    ADCON0bits.ADON = 1;    // start A/D converter
    
    T0CON = 0b00000100;     /* init timer 0 */
    TMR0 = t0_init_val;
    T1CON = 0b10110100;     /* init timer 1 */
    TMR1 = t1_init_val;
    T1CONbits.TMR1ON = 1;   // start counting
    T0CONbits.TMR0ON = 1;   // start counting
    
    
    INTCON = 0b01101000;    // PEIE, TMR0IE, RBIE
    PIE1 = 0b01000001;      // ADIE, TMR1IE
    PIR1 = 0b00000000;      // PER. INT. REQ
    INTCONbits.GIE = 1;     // GIE, start interrupts
    
    return;
}

void update_latjh(void)
{
    PORTH = 0b00001000;
    if(FLAGS.win | FLAGS.loss)
    {
        if(FLAGS.rhid) PORTJ = 0b00000000;
        else PORTJ = SEG7[special_number()];
    }
    else
    {
        PORTJ = SEG7[current_guess];
    }
    
    latjh_update_complete();
}

void update_latcde(void)    // set FLAGS.gdir before calling
{
    PORTD = 0b00001111;
    if(FLAGS.gdir)
    {
        PORTC = 0b00000010;
        PORTE = 0b00000010;
    }
    else
    {
        PORTC = 0b00000100;
        PORTE = 0b00000100;
    }
    
    latcde_update_complete();
}

void game_loop(void)
{
    char snum = special_number();
    
    while(!FLAGS.over)
    {
        if(FLAGS.loss)
        {
            update_latjh();
            update_latcde();
        }

        if(FLAGS.rb4h)
        {
            FLAGS.rb4h = 0;

            if(current_guess < snum)
            {
                FLAGS.gdir = 1;
            }
            else if(current_guess > snum)
            {
                FLAGS.gdir = 0;
            }
            else        // guess = special, game won
            {
                FLAGS.win = 1;
                TMR1 = t1_init_val;
		t1_postscale = 0;

                // disable interrupts?

                correct_guess();
            }

            update_latjh();
            update_latcde();
        }

        if(FLAGS.hsac)
        {
            FLAGS.hsac = 0;

            update_latjh();
        }
    }
}

void main(void)
{   
    while(1)
    {
        init();
        init_complete();
        
        game_loop();
        
        restart();
    }
}
