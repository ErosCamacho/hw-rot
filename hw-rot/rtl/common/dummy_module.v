
`timescale 1ns / 1ps

module dummy_module(
    input                           i_clk,
    input                           i_rst,
    input   [63:0]                  i_data_in,
    input   [63:0]                  i_add,
    input   [31:0]                  i_control,
    output  [63:0]                  o_data_out,
    output                          o_end_op
    );
    
    assign o_end_op = 1'b1;
    assign o_data_out = 64'hFFFF_FFFF_FFFF_FFFF;
    
endmodule
