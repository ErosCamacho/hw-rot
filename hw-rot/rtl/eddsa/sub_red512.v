
module sub_red512 #(parameter BIT_LENGTH = 256)(
                                                input  wire clk,                    //-- Clock Signal
                                                input  wire rst,                    //-- Reset Signal 
                                                input  wire [BIT_LENGTH-1:0] A,     //-- Input A
                                                input  wire [385:0] B,              //-- Input B
                                                output reg  [385:0] C,              //-- Output C
                                                output wire bigger,                 //-- A > B
                                                output wire equal,                  //-- A == B
                                                output wire smaller                 //-- A < B 
                                                );
    
    assign smaller = (B > A) ? 1 : 0;
    assign equal   = (B[BIT_LENGTH-1:0] == A) ? 1 : 0;
    assign bigger  = !smaller & !equal;
    
    always @(posedge clk) begin
        if (rst)
            C <= 0;
        else if (smaller)
            C <= B - A;
        else 
            C <= A - B;
    end
   
endmodule
