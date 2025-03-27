`default_nettype none
`define DUMPSTR(x) `"x.vcd`"
`timescale 1 ns / 10 ps

module aes_mixcolumns_tb();
	
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
	reg  [31:0] vector_in;
	wire [31:0] vector_out;
	   
			
	//--------------------------------------
	//-- AES sbox Instance
	//--------------------------------------
	
	aes_mixcolumns DUT(
					   .enc(enc),
					   .vector_in(vector_in),
					   .vector_out(vector_out)
					   ); 
	
	
	//---------------------------------
	//-- Test Values
	//---------------------------------
	
	initial begin
		enc 		= 1'b1;
		vector_in 	= 32'h0;
		#10
		vector_in 	= {8'b00001100,
					   8'b10000001,
					   8'b00110001,
					   8'b00111000};
	end
	
	
endmodule
