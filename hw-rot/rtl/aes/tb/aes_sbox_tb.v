`default_nettype none
`define DUMPSTR(x) `"x.vcd`"
`timescale 1 ns / 10 ps

module aes_sbox_tb();
	
	//---------------------------------
	//-- Simulation time
	//---------------------------------
	
	parameter DURATION = 100;
	
	initial
	begin
		#(DURATION) $display("End of simulation");
		$finish;
	end	
	
	
	//--------------------------------------
	//-- Wires and Registers               
	//--------------------------------------
		
	reg  enc;
	reg  [7:0] sbox_in;
	wire [7:0] sbox_out;
	   
			
	//--------------------------------------
	//-- AES sbox Instance
	//--------------------------------------
	
	aes_sbox DUT(
				 .enc(enc),
				 .sbox_in(sbox_in),
				 .sbox_out(sbox_out)
				 ); 
	
	
	//---------------------------------
	//-- Test Values
	//---------------------------------
	
	initial begin
		enc 	= 1'b1;
		sbox_in = 8'h00;
		#10
		sbox_in = 8'hAB;
		#10
		sbox_in = 8'h0D;
		#10
		sbox_in = 8'h8F;
		#10
		sbox_in = 8'h33;
		
		#10
		enc 	= 1'b0;
		sbox_in = 8'h00;
		#10
		sbox_in = 8'hAB;
		#10
		sbox_in = 8'h0D;
		#10
		sbox_in = 8'h8F;
		#10
		sbox_in = 8'h33;
	end
	
	
endmodule
