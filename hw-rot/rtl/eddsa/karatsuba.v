
module karatsuba #(
                   parameter BIT_LENGTH = 256,
                   parameter n_LEVEL = 4,                    //-- Recursion Levels
                   parameter n_LEVEL_0 = 4                   //-- Original Recursion Level  
                   ) 
                   (
					input  wire clk,						 //-- Clock Signal
					input  wire rst,						 //-- Active High Reset 
					input  wire redux,                       //-- Reduction Option
					input  wire [BIT_LENGTH-1:0] A,		     //-- Operand A
					input  wire [BIT_LENGTH-1:0] B,		     //-- Operand B
					output reg  [2*BIT_LENGTH-1:0] C,        //-- Output product A*B = C
					output reg  valid						 //-- Output data valid
					);	
	
	genvar i;
	integer j;

	//--  A and B assignations to High and Low radix
	wire [BIT_LENGTH/2-1:0] A_hi;
	wire [BIT_LENGTH/2-1:0] A_lo;
	assign A_hi = A[BIT_LENGTH-1:BIT_LENGTH/2];
	assign A_lo = A[BIT_LENGTH/2-1:0];
	
	wire [BIT_LENGTH/2-1:0] B_hi;
	wire [BIT_LENGTH/2-1:0] B_lo;
	assign B_hi = B[BIT_LENGTH-1:BIT_LENGTH/2];
	assign B_lo = B[BIT_LENGTH/2-1:0];
	
	//-- Partial Products
	wire A_neg;
	wire B_neg; 
	assign A_neg = (A_hi > A_lo) ? 1 : 0;
	assign B_neg = (B_lo > B_hi) ? 1 : 0;
    
    generate if (n_LEVEL > 1) begin: minus
	   wire [BIT_LENGTH/2-1:0] A_minus;
	   wire [BIT_LENGTH/2-1:0] B_minus;
	   assign A_minus = (A_neg) ? (A_hi - A_lo) : (A_lo - A_hi);
	   assign B_minus = (B_neg) ? (B_lo - B_hi) : (B_hi - B_lo);
    end
    endgenerate
	
	wire [BIT_LENGTH-1:0] z2;
	wire [BIT_LENGTH-1:0] AB_minus;
	wire [BIT_LENGTH-1:0] z0;
	
	wire [BIT_LENGTH:0] z1;
    
    generate if (n_LEVEL > 1) begin: ready
       wire ready_z2;
	   wire ready_z1;
	   wire ready_z0;
    end
    endgenerate
    
    
    ///////////////////////////////////////////////////////////////////
    //-- First Levels of Recursion
    ///////////////////////////////////////////////////////////////////
    
    generate if (n_LEVEL > 1 && n_LEVEL != n_LEVEL_0) begin
       
       localparam DELAY = 1;
       
       reg neg;
	   reg neg_i[n_LEVEL-2+DELAY:0];
	   
	   assign z1 = (neg) ? (z2 + z0 - AB_minus) : (z2 + z0 + AB_minus);

       always @(posedge clk) begin
		  if (rst) begin
	         C 		  <= 0;
			 valid 	  <= 0;
			 neg      <= 0;
			 for (j = 0; j < n_LEVEL-1+DELAY; j = j + 1) begin
			     neg_i[j] <= 0;
             end
		  end
		  else begin
		     if (ready.ready_z2 & ready.ready_z1 & ready.ready_z0) begin
		        C       <= (z2 << BIT_LENGTH) + (z1 << BIT_LENGTH/2) + z0;
		        valid   <= 1;
		     end

	         neg_i[0] <= A_neg ^ B_neg;
	         for (j = 1; j < n_LEVEL-1+DELAY; j = j + 1) begin 
	             neg_i[j] <= neg_i[j-1];
	         end
	         neg <= neg_i[n_LEVEL-2+DELAY];
		  end
       end
    
    end
    endgenerate
    
    generate if (n_LEVEL > 1 && n_LEVEL == n_LEVEL_0) begin
       
       localparam DELAY = 1;
       
       reg neg;
	   reg neg_i[n_LEVEL-2+DELAY:0];
	   
	   assign z1 = (neg) ? (z2 + z0 - AB_minus) : (z2 + z0 + AB_minus);

       always @(posedge clk) begin
		  if (rst) begin
	         C 		  <= 0;
			 valid 	  <= 0;
			 neg      <= 0;
			 for (j = 0; j < n_LEVEL-1+DELAY; j = j + 1) begin
			     neg_i[j] <= 0;
             end
		  end
		  else if (redux) begin
		     if (ready.ready_z2 & ready.ready_z1 & ready.ready_z0) begin
		         C       <= (z2 << 5) + (z2 << 2) + (z2 << 1) + (z1 << BIT_LENGTH/2) + z0;
		         valid   <= 1;
		     end

	         neg_i[0] <= A_neg ^ B_neg;
	         for (j = 1; j < n_LEVEL-1+DELAY; j = j + 1) begin 
	             neg_i[j] <= neg_i[j-1];
	         end
	         neg <= neg_i[n_LEVEL-2+DELAY];
		  end
		  else begin
		     if (ready.ready_z2 & ready.ready_z1 & ready.ready_z0) begin
		        C       <= (z2 << BIT_LENGTH) + (z1 << BIT_LENGTH/2) + z0;
		        valid   <= 1;
		     end

	         neg_i[0] <= A_neg ^ B_neg;
	         for (j = 1; j < n_LEVEL-1+DELAY; j = j + 1) begin 
	             neg_i[j] <= neg_i[j-1];
	         end
	         neg <= neg_i[n_LEVEL-2+DELAY];
		  end
       end
    
    end
    endgenerate
    
    
    ///////////////////////////////////////////////////////////////////
    //-- Last Level of Recursion
    ///////////////////////////////////////////////////////////////////
    
    generate if (n_LEVEL == 1 && n_LEVEL != n_LEVEL_0) begin
       
       reg valid_0;
       reg valid_1;
        
       reg [BIT_LENGTH/2-1:0] A_minus; 
       reg [BIT_LENGTH/2-1:0] B_minus;

       reg [BIT_LENGTH/2-1:0] A_hi_r;
	   reg [BIT_LENGTH/2-1:0] A_lo_r;
	   reg [BIT_LENGTH/2-1:0] B_hi_r;
	   reg [BIT_LENGTH/2-1:0] B_lo_r;
        
       reg [BIT_LENGTH-1:0] z2_r;
	   reg [BIT_LENGTH-1:0] AB_minus_r;
	   reg [BIT_LENGTH-1:0] z0_r;
       
       reg neg;
       reg neg_0;

	   assign z1 = (neg) ? (z2 + z0 - AB_minus) : (z2 + z0 + AB_minus);
        
       assign z2        = z2_r; 		
       assign AB_minus  = AB_minus_r;
       assign z0        = z0_r;    

	   always @(posedge clk) begin
	       if (rst) begin
	       	   C            <= 0;
	       	   neg          <= 0;
	       	   neg_0        <= 0;
               
               A_minus      <= 0;
               B_minus      <= 0;
               
               A_hi_r       <= 0;
               A_lo_r       <= 0;
               B_hi_r       <= 0;
               B_lo_r       <= 0;
               
	       	   z2_r 		<= 0;
               AB_minus_r	<= 0;
               z0_r         <= 0;
	      
	       	   valid_0      <= 0;
	       	   valid_1      <= 0;
	       	   valid 	    <= 0;
	       end
	       else begin
	       	   neg_0   <= A_neg ^ B_neg;
	       	   neg     <=  neg_0;
	       	  
               A_hi_r  <= A_hi; 
               A_lo_r  <= A_lo; 
               B_hi_r  <= B_hi; 
               B_lo_r  <= B_lo; 
                    
               A_minus <= (A_neg) ? (A_hi - A_lo) : (A_lo - A_hi);
               B_minus <= (B_neg) ? (B_lo - B_hi) : (B_hi - B_lo);
               
               valid_0 <= 1;
               
	       	   if (valid_0) begin
	       	       z2_r 	   <= A_hi_r * B_hi_r;      
	       	       AB_minus_r  <= A_minus * B_minus;
	       	       z0_r	       <= A_lo_r * B_lo_r;
                    
	               valid_1 	   <= 1;
               end
	           
	           if (valid_1) begin
	       	       C       <= (z2 << BIT_LENGTH) + (z1 << BIT_LENGTH/2) + z0;
	               valid   <= 1;
	           end
	           
	       end
	   end
    end
    endgenerate
    
    generate if (n_LEVEL == 1 && n_LEVEL == n_LEVEL_0) begin
    
       reg valid_0;
       reg valid_1;
        
       reg [BIT_LENGTH/2-1:0] A_minus; 
       reg [BIT_LENGTH/2-1:0] B_minus;

       reg [BIT_LENGTH/2-1:0] A_hi_r;
	   reg [BIT_LENGTH/2-1:0] A_lo_r;
	   reg [BIT_LENGTH/2-1:0] B_hi_r;
	   reg [BIT_LENGTH/2-1:0] B_lo_r;
        
       reg [BIT_LENGTH-1:0] z2_r;
	   reg [BIT_LENGTH-1:0] AB_minus_r;
	   reg [BIT_LENGTH-1:0] z0_r;
       
       reg neg;
       reg neg_0;

	   assign z1 = (neg) ? (z2 + z0 - AB_minus) : (z2 + z0 + AB_minus);
        
       assign z2        = z2_r; 		
       assign AB_minus  = AB_minus_r;
       assign z0        = z0_r;    

	   always @(posedge clk) begin
	       if (rst) begin
	       	   C            <= 0;
	       	   neg          <= 0;
	       	   neg_0        <= 0;
               
               A_minus      <= 0;
               B_minus      <= 0;
               
               A_hi_r       <= 0;
               A_lo_r       <= 0;
               B_hi_r       <= 0;
               B_lo_r       <= 0;
               
	       	   z2_r 		<= 0;
               AB_minus_r	<= 0;
               z0_r         <= 0;
	      
	       	   valid_0      <= 0;
	       	   valid_1      <= 0;
	       	   valid 	    <= 0;
	       end
	       else begin
	       	   neg_0   <= A_neg ^ B_neg;
	       	   neg     <=  neg_0;
	       	  
               A_hi_r  <= A_hi; 
               A_lo_r  <= A_lo; 
               B_hi_r  <= B_hi; 
               B_lo_r  <= B_lo; 
                    
               A_minus <= (A_neg) ? (A_hi - A_lo) : (A_lo - A_hi);
               B_minus <= (B_neg) ? (B_lo - B_hi) : (B_hi - B_lo);
               
               valid_0 <= 1;
               
	       	   if (valid_0) begin
	       	       z2_r 	   <= A_hi_r * B_hi_r;      
	       	       AB_minus_r  <= A_minus * B_minus;
	       	       z0_r	       <= A_lo_r * B_lo_r;
                    
	               valid_1 	   <= 1;
               end
	           
	           if (valid_1 && redux) begin
				   C       <= (z2 << 5) + (z2 << 2) + (z2 << 1) + (z1 << BIT_LENGTH/2) + z0;
	               valid   <= 1;
	           end
	           else if (valid_1) begin
	               C       <= (z2 << BIT_LENGTH) + (z1 << BIT_LENGTH/2) + z0;
	               valid   <= 1;
	           end
	           
	       end
	   end
	   
    end
    endgenerate
    
	
	
	//--------------------------------------
	//-- Karatsuba Multipliers             
	//--------------------------------------
	
	generate 
	if (n_LEVEL > 1) begin
	    karatsuba #(
	                .BIT_LENGTH(BIT_LENGTH/2), 
	                .n_LEVEL(n_LEVEL-1), 
	                .n_LEVEL_0(n_LEVEL_0)
	                ) 
	                karatsuba_recursion_z2
	                (
	                 .clk(clk),
	                 .rst(rst),
	                 //.redux(redux),
	                 .A(A_hi),
	                 .B(B_hi),
	                 .C(z2),
	                 .valid(ready.ready_z2)
	                 );
	end
	endgenerate
	
	generate
	if (n_LEVEL > 1) begin
	    karatsuba #(
	                .BIT_LENGTH(BIT_LENGTH/2), 
	                .n_LEVEL(n_LEVEL-1),
	                .n_LEVEL_0(n_LEVEL_0)
	                ) 
	                karatsuba_recursion_z1
	                (
	                 .clk(clk),
	                 .rst(rst),
	                 //.redux(redux),
	                 .A(minus.A_minus),
	                 .B(minus.B_minus),
	                 .C(AB_minus),
	                 .valid(ready.ready_z1)
	                 );
	end
	endgenerate
							
	generate
	if (n_LEVEL > 1) begin
	    karatsuba #(
	                .BIT_LENGTH(BIT_LENGTH/2), 
	                .n_LEVEL(n_LEVEL-1),
	                .n_LEVEL_0(n_LEVEL_0)
	                ) 
	                karatsuba_recursion_z0
	                (
	                 .clk(clk),
	                 .rst(rst),
	                 //.redux(redux),
	                 .A(A_lo),
	                 .B(B_lo),
	                 .C(z0),
	                 .valid(ready.ready_z0)
	                 );
	end
	endgenerate
	
endmodule
