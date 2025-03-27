

module aes_keyaddition(
					   input  wire [127:0] subkey,			//-- Input Subkey
					   input  wire [127:0] state_in,		//-- Input State
					   output wire [127:0] state_out		//-- Output Data
					   );
	
	
	//------------------------------------------------------------------------------------------------
	//-- Key Addition Layer
	//------------------------------------------------------------------------------------------------
    
	assign state_out = state_in ^ subkey;
								 
	
endmodule
