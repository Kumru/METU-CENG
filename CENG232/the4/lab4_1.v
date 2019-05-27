`timescale 1ns / 1ps
module MaskROM (input [3:0] RomAddress, output reg[7:0] RomDataOut);
/*  Please write your code below  */
    
    always @( * )
    begin
        case(RomAddress)
            4'h0:  RomDataOut = 8'b00000000; 4'h1:  RomDataOut = 8'b00001111;
            4'h2:  RomDataOut = 8'b00011110; 4'h3:  RomDataOut = 8'b00110000;
            4'h4:  RomDataOut = 8'b01010000; 4'h5:  RomDataOut = 8'b01100110;
            4'h6:  RomDataOut = 8'b01101010; 4'h7:  RomDataOut = 8'b01111110;
            4'h8:  RomDataOut = 8'b10000001; 4'h9:  RomDataOut = 8'b10100000;
            4'ha:  RomDataOut = 8'b10100110; 4'hb:  RomDataOut = 8'b10111101;
            4'hc:  RomDataOut = 8'b11000000; 4'hd:  RomDataOut = 8'b11010000;
            4'he:  RomDataOut = 8'b11010011; 4'hf:  RomDataOut = 8'b11100110;
        endcase
    end

/*  Please write your code above  */
endmodule


/*mode 0:read, 1:write*/																							
module RgbRAM (input Mode,input [3:0] RamAddress, input [23:0] RamDataIn,
					input [7:0] Mask,input [2:0] Op, input CLK, output reg [23:0] RamDataOut);
/*  Please write your code below  */

    reg [23:0] RAM [15:0];
    
    integer k;
    initial for (k = 0; k < 16; k = k + 1) RAM[k] = 0;

    always @( * )
        if(Mode == 0) RamDataOut = RAM[RamAddress];

    always @(posedge CLK)
    begin
        if(Mode == 1)
        begin
            case(Op)
                0: RAM[RamAddress] <= RamDataIn & {3{Mask}};
                1: RAM[RamAddress] <= RamDataIn | {3{Mask}};
                2: RAM[RamAddress] <= RamDataIn ^ {3{Mask}};
                3:
                begin
                    if(RamDataIn[7:0] + Mask > 255) RAM[RamAddress][7:0] <= 8'd255;
                    else RAM[RamAddress][7:0] <= RamDataIn[7:0] + Mask;

                    if(RamDataIn[15:8] + Mask > 255) RAM[RamAddress][15:8] <= 8'd255;
                    else RAM[RamAddress][15:8] <= RamDataIn[15:8] + Mask;

                    if(RamDataIn[23:16] + Mask > 255) RAM[RamAddress][23:16] <= 8'd255;
                    else RAM[RamAddress][23:16] <= RamDataIn[23:16] + Mask;
                end
                4:
                begin
                    if(RamDataIn[7:0] < Mask) RAM[RamAddress][7:0] <= 0;
                    else RAM[RamAddress][7:0] <= RamDataIn[7:0] - Mask;

                    if(RamDataIn[15:8] < Mask) RAM[RamAddress][15:8] <= 0;
                    else RAM[RamAddress][15:8] <= RamDataIn[15:8] - Mask;

                    if(RamDataIn[23:16] < Mask) RAM[RamAddress][23:16] <= 0;
                    else RAM[RamAddress][23:16] <= RamDataIn[23:16] - Mask;
                end
                5:
                begin
                    if(RamDataIn[7:0] < 255) RAM[RamAddress][7:0] <= RamDataIn[7:0] + 1;
                    if(RamDataIn[15:8] < 255) RAM[RamAddress][15:8] <= RamDataIn[15:8] + 1;
                    if(RamDataIn[23:16] < 255) RAM[RamAddress][23:16] <= RamDataIn[23:16] + 1;
                end
                6:
                begin
                    if(RamDataIn[7:0] != 0) RAM[RamAddress][7:0] <= RamDataIn[7:0] - 1;
                    if(RamDataIn[15:8] != 0) RAM[RamAddress][15:8] <= RamDataIn[15:8] - 1;
                    if(RamDataIn[23:16] != 0) RAM[RamAddress][23:16] <= RamDataIn[23:16] - 1;
                end
                7: RAM[RamAddress] <= {RamDataIn[22:16], RamDataIn[23], RamDataIn[14:8], RamDataIn[15], RamDataIn[6:0], RamDataIn[7]};
            endcase
        end
    end

/*  Please write your code above  */
endmodule


module RgbMaskModule(input Mode, input [3:0] Address, input [23:0] RGBin,input [2:0] Op,  input CLK, output wire [23:0] RGBout);
	
	/*  DO NOT edit this module  */
	
	wire [7:0]  romResult;

	MaskROM RO(Address, romResult);
	RgbRAM RA(Mode, Address, RGBin,romResult, Op, CLK, RGBout);
endmodule
