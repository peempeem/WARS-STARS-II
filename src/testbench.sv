module testbench();

    timeunit 10ns;
    timeprecision 1ns;

    logic           clk,
                            reset;

    logic   [31:0]  sprite_address,
                            sprite_dims,
                            sprite_xy,
                            sprite_startxy,
                            sprite_endxy;
    
    logic   [31:0]  frame_address;
    
    logic   [31:0]  address;
    logic   [ 4:0]  burstcount;
    logic   [ 3:0]  byteenable;
    logic           read;
    logic   [31:0]  readdata;
    logic           readdatavalid,
                            waitrequest;
    logic           write;
    logic   [31:0]  writedata;

    logic           start;
    logic           done;

    logic   [3:0]   state_out;
    logic   [15:0]  x, y;
	 
	 
	logic Clk;
	assign clk = Clk;
            
    Blitter blitter(.*);


    always begin: CLOCK_GENERATION
    #1 Clk = ~Clk;
    end

    initial begin: CLOCK_INITIALIZATION
         Clk = 0;
         frame_address = 32'h00000000;
         sprite_address = 32'h10000000;
         sprite_dims = 32'h00030003;
         sprite_xy   = 32'h00000000;
         sprite_startxy = 32'h00000000;
         sprite_endxy = 32'h00030003;
    end 

    
    initial begin: TEST_VECTORS
        #2
            reset = 1;
        #3
            reset = 0;
        #10
            start = 1;
        #11
            start = 0;


    end

endmodule
