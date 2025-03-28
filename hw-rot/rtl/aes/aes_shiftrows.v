
module aes_shiftrows(
					 input  wire enc,					//-- Encryption -> 1 | Decryption -> 0 
					 input  wire [127:0] state_in,		//-- Input Data
					 output wire [127:0] state_out		//-- Output Data
					 );
	
	
	//------------------------------------------------------------------------------------------------
	//-- Subdivide I/O in byte blocks
	//------------------------------------------------------------------------------------------------
    
	wire [7:0] B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15;
	
	assign B0   = state_in[127:120];
	assign B1   = state_in[119:112];
	assign B2   = state_in[111:104];
	assign B3   = state_in[103:96];
	assign B4   = state_in[95:88];
	assign B5   = state_in[87:80];
	assign B6   = state_in[79:72];
	assign B7   = state_in[71:64];
	assign B8   = state_in[63:56];
	assign B9   = state_in[55:48];
	assign B10  = state_in[47:40];
	assign B11  = state_in[39:32];
	assign B12  = state_in[31:24];
	assign B13  = state_in[23:16];
	assign B14  = state_in[15:8];
	assign B15  = state_in[7:0];
	
	
    //------------------------------------------------------------------------------------------------
	//-- ShiftRows Sublayer    
	//------------------------------------------------------------------------------------------------
	
	assign state_out = (enc) ? {B0,B5,B10,B15,B4,B9,B14,B3,B8,B13,B2,B7,B12,B1,B6,B11}
								
								:
								
							   {B0,B13,B10,B7,B4,B1,B14,B11,B8,B5,B2,B15,B12,B9,B6,B3};
								 
	
endmodule
