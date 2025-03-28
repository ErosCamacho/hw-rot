
module aes_itf #(
                 localparam WIDTH      = 64,
                 localparam IN_REG     = 7,
                 localparam OUT_REG    = 2
                 )
                 (
                  input  wire clk,                                    //-- Clock Signal
                  input  wire i_rst,                                  //-- Reset Signal                              
                  input  wire [3:0] control,                          //-- Control Signal: {read, load, rst_itf, rst} 
                  input  wire [WIDTH-1:0] address,                    //-- Address
                  input  wire [WIDTH-1:0] data_in,                    //-- Data Input
                  output wire [WIDTH-1:0] data_out,                   //-- Data Output
                  output wire end_op                                  //-- End Operation Signal
                  );

    
    //--------------------------------------
	//-- Wires & Registers
	//--------------------------------------
    
    //-- Control Signals
    wire rst;
    wire rst_itf;
    wire load;
    wire read;
    
    assign rst      = control[0] | !i_rst;
    assign rst_itf  = control[1] | !i_rst;
    assign load     = control[2];
    assign read     = control[3];
    
    //-- Sipo Input
    wire [IN_REG*WIDTH-1:0] data_aes_in;
    //-- Piso input
    wire [OUT_REG*WIDTH-1:0] data_aes_out;
    
    //-- AES Inputs
	wire enc;
	wire [1:0] aes_len;
	wire [4*WIDTH-1:0] key;
	wire [2*WIDTH-1:0] plaintext;
	
	assign enc			= data_aes_in[0];
	assign aes_len 		= data_aes_in[2:1];
    assign key	  		= data_aes_in[5*WIDTH-1:WIDTH];
    assign plaintext 	= data_aes_in[7*WIDTH-1:5*WIDTH];
    
    //-- EdDSA Outputs
	wire [2*WIDTH-1:0] ciphertext;
	wire valid;
	
	assign data_aes_out = ciphertext;
    
    
    //--------------------------------------
	//-- SIPO          
	//--------------------------------------
    
    sipo #(.R_DATA_WIDTH(WIDTH), .N_REG(IN_REG)) SIPO (
	                                                   .clk(clk),
	                                                   .rst(rst_itf),
	                                                   .load(load),
						                               .addr(address),
						                               .din(data_in),
						                               .dout(data_aes_in)
						                               ); 
                                                      

    //--------------------------------------
	//-- AES                        
	//--------------------------------------
        
	aes_core AES(
				 .clk(clk),
				 .rst(rst),
				 .enc(enc),
				 .aes_len(aes_len),
				 .key(key),
				 .plaintext(plaintext),
				 .ciphertext(ciphertext),
				 .valid(valid)
				 ); 
    
    
    //--------------------------------------
	//-- PISO              
	//--------------------------------------
    
    piso #(.R_DATA_WIDTH(WIDTH), .N_REG(OUT_REG)) PISO(
						                               .clk(clk),
						                               .read(read),
						                               .addr(address),
						                               .din(data_aes_out),
						                               .dout(data_out)
						                               );
    
    
    assign end_op = valid;


endmodule
