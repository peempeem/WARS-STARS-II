// VGA screen is 640 by 480 pixels
parameter VGA_START = 32'h08000000;

module graphics_drawing_unit (
    input   logic           clk,
                            reset,

    output  logic   [31:0]  avalon_mm_master_address,
    output  logic   [ 3:0]  avalon_mm_master_byteenable,
    input   logic           avalon_mm_master_waitrequest,
    output  logic           avalon_mm_master_write,
    output  logic   [31:0]  avalon_mm_master_writedata,

    input   logic   [ 9:0]  avalon_mm_slave_address,
    input   logic   [ 3:0]  avalon_mm_slave_byteenable,
    input   logic           avalon_mm_slave_read,
    output  logic   [31:0]  avalon_mm_slave_readdata,
    input   logic           avalon_mm_slave_write,
    input   logic   [31:0]  avalon_mm_slave_writedata
);

    enum logic [3:0] {
        WRITING,
        WAIT_STATE,
        WAITING
    } state;

    always_ff @(posedge clk) begin

    end

endmodule