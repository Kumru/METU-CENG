`timescale 1ns / 1ps 
module lab3_2(
			input[3:0] number,
			input CLK, 
			input selection,
			input mode,
			input clear,
			output reg [7:0] digit1,
			output reg [7:0] digit0,
			output reg [7:0] count1,
			output reg [7:0] count0,
			output reg warning
    );
   //Modify the lines below to implement your design .
    
    //selection 0 temps
    reg [7:0] tmp_count1_0;
    reg [7:0] tmp_count0_0;
    //selection 1 temps
    reg [7:0] tmp_count1_1;
    reg [7:0] tmp_count0_1;
    
    initial begin
        warning = 0;
        digit1 = 0;
        digit0 = 0;
        count0 = 0;
        count1 = 0;
        tmp_count1_0 = 0;
        tmp_count0_0 = 0;
        tmp_count1_1 = 0;
        tmp_count0_1 = 0;
    end
    
    always @(posedge CLK)
    begin
        if (clear == 1)
        begin
			warning = 0;
            digit1 = 0;
            digit0 = 0;
            count1 = 0;
            count0 = 0;
            tmp_count1_0 = 0;
            tmp_count0_0 = 0;
            tmp_count1_1 = 0;
            tmp_count0_1 = 0;
        end
        else if(selection == 0)
        begin
            if(number != 2 && number != 3 && number != 5 && number != 7 && number != 11 && number != 13)
                warning = 1;
            else
            begin
                warning = 0;
                if(mode == 1)
                begin
                    tmp_count1_0 = tmp_count1_0 + 1;
                    if(tmp_count1_0>9)
                        tmp_count1_0 = 0;
                    case(number)
                        8'h2:digit1 = 8'h3;
                        8'h3:digit1 = 8'h5;
                        8'h5:digit1 = 8'h7;
                        8'h7:digit1 = 8'hb;
                        8'hb:digit1 = 8'hd;
                        8'hd:digit1 = 8'h2;
                    endcase
                end
                else
                begin
                    tmp_count0_0 = tmp_count0_0 + 1;
                    if(tmp_count0_0>9)
                        tmp_count0_0 = 0;
                    case(number)
                        8'h2:digit1 = 8'hd;
                        8'h3:digit1 = 8'h2;
                        8'h5:digit1 = 8'h3;
                        8'h7:digit1 = 8'h5;
                        8'hb:digit1 = 8'h7;
                        8'hd:digit1 = 8'hb;
                    endcase
                end
                digit0 = number;
                count0 = tmp_count0_0;
                count1 = tmp_count1_0;
            end
        end
        else
        begin
            if(number == 2 || number == 3 || number == 5 || number == 7 || number == 11 || number == 13)
                warning = 1;
            else
            begin
                warning = 0;
                digit1 = 0;
                if(mode == 1)
                begin
                    digit1[3:1] = number[2:0];
                    tmp_count1_1 = tmp_count1_1 + 1;
                    if(tmp_count1_1>9)
                            tmp_count1_1 = 0;
                end
                else
                begin
                    digit1[2:0] = number[3:1];
                    tmp_count0_1 = tmp_count0_1 + 1;
                    if(tmp_count0_1>9)
                            tmp_count0_1 = 0;
                end
                digit0 = number;
                count0 = tmp_count0_1;
                count1 = tmp_count1_1;
            end
        end
    end
    
endmodule
