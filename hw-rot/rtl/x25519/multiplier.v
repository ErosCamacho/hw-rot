
module multiplier #(
                    parameter BIT_LENGTH = 256,
                    parameter n_LEVEL = 2                   //-- Recursion Levels
                    ) 
                    (
				     input  wire clk,						//-- Clock Signal
				     input  wire rst,						//-- Active High Reset 
				     input  wire redux,                     //-- Reduction option
				     input  wire [BIT_LENGTH-1:0] A,		//-- Operand B
				     input  wire [BIT_LENGTH-1:0] B,		//-- Operand A
				     output wire [2*BIT_LENGTH-1:0] U,		//-- Output product A*B = U mod P or A*B = U
				     output reg  valid						//-- Output data valid
				     );	
	
	
	//--------------------------------------
	//-- Parameters            
	//--------------------------------------

	localparam P = 256'h7fffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffed;
	
	
	//-------------------------------------------------------
	//-- 256x256 4-level Karatsuba Multiplier with reduction             
	//-------------------------------------------------------
	
	wire ready;
	
	wire [2*BIT_LENGTH-1:0] C;
	
	wire [131:0] C_387_hi;
	wire [254:0] C_387_lo;
	
	assign C_387_hi = C[386:255];
	assign C_387_lo = C[254:0];
	
	karatsuba #(.BIT_LENGTH(BIT_LENGTH), .n_LEVEL(n_LEVEL), .n_LEVEL_0(n_LEVEL)) karatsuba(
							                                                               .clk(clk),
							                                                               .rst(rst),
							                                                               .redux(redux),
							                                                               .A(A),
							                                                               .B(B),
							                                                               .C(C),
							                                                               .valid(ready)
							                                                               ); 
	
	//--------------------------------------------------------------
	//-- 2nd reduction: C_387 = C_hi*2^255 + C_lo = C_hi*19 + C_lo         
	//--------------------------------------------------------------
	
	reg [255:0] C_255;
	
	always @(*) begin
	
		if (rst) begin
			valid    <= 0;
			C_255    <= 0;
		end
		else if (ready)begin
		  	valid <= 1;
		  	// C_255 <= (C_387_hi << 4) + (C_387_hi << 1) + C_387_hi + C_387_lo;
		  	C_255 <= {C_387_hi, 4'h0} + {C_387_hi, 1'b0} + C_387_hi + C_387_lo;
		end   
		else begin
		    valid     <= 0;
		    C_255     <= 0;
		end
	
	end
		
	assign U = (redux == 0) ? C : ((C_255 >= P) ? (C_255 - P) : C_255);
	
endmodule
