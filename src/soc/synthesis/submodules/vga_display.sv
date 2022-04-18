parameter VGA_START = 32'h08000000;

module vga_display (
    input   logic           clk,
                            reset,
    output  logic           avalon_master_read,
    output  logic   [31:0]  avalon_master_address,
    input   logic   [31:0]  avalon_master_readdata,
    input   logic           avalon_master_waitrequest,
                            avalon_master_readdatavalid,
    output  logic   [ 3:0]  red,
                            green,
                            blue,
    output  logic           hs,
                            vs
);

    logic       pixel_clk,
                blank,
                sync;
    logic [9:0] DrawX,
                DrawY;
    
    vga_controller vgac (
        .Clk(clk),
        .Reset(reset),
        .hs(hs),
        .vs(vs),
        .pixel_clk(pixel_clk),
        .blank(blank),
        .sync(sync),
        .DrawX(DrawX),
        .DrawY(DrawY)
    );

    logic   [ 9:0]  lb_addr_a,
                    lb_addr_b;
    logic   [15:0]  lb_write,
                    lb_read;
    logic           lb_wren;

    line_buffer lb (
        .address_a(lb_addr_a),
        .address_b(lb_addr_b),
        .clock(clk),
        .data_a(lb_write),
        .data_b(),
        .wren_a(lb_wren),
        .wren_b(0),
        .q_a(),
        .q_b(lb_read)
    );

    enum logic [3:0] {
        DRAWING,
        READING,
        WAIT_STATE,
        WRITING,
        WRITING_1,
        WAITING
    } state;

    logic   [8:0]   lb_x;
    logic   [9:0]   lb_y;
    logic   [31:0]  data;

    always_comb begin
        if (DrawY >= 480)
            lb_y = 0;
        else
            lb_y = DrawY + 1;
    end

    always_ff @(posedge clk) begin
        avalon_master_read      <= avalon_master_read;
        avalon_master_address   <= avalon_master_address;
        lb_x                    <= lb_x;
        lb_wren                 <= 0;
        lb_addr_a               <= lb_addr_a;
        lb_write                <= lb_write;
        data                    <= data;
        if (reset) begin
            avalon_master_read  <= 0;
            state               <= DRAWING;
        end
        else begin
            unique case (state)
                DRAWING: begin
                    avalon_master_read  <= 0;
                    lb_x                <= 0;
                    if (blank == 0)
                        state <= READING;
                    else                    
                        state <= DRAWING;
                end
                READING: begin
                    if (lb_x < 320) begin
                        avalon_master_read      <= 1;
                        avalon_master_address   <= VGA_START + lb_y * 1280 + lb_x * 4;
                        state                   <= WAIT_STATE;
                    end
                    else
                        state <= WAITING;
                end
                WAIT_STATE: begin
                    if (~avalon_master_waitrequest & avalon_master_readdatavalid) begin
                        avalon_master_read      <= 0;
                        data                    <= avalon_master_readdata;
                        state                   <= WRITING;
                    end
                    else
                        state <= WAIT_STATE;
                end
                WRITING: begin
                    lb_x                <= lb_x + 1;
                    lb_wren             <= 1;
                    lb_addr_a           <= lb_x * 2;
                    lb_write        <= data[15:0];
                    state               <= WRITING_1;
                end
                WRITING_1: begin
                    lb_wren             <= 1;
                    lb_addr_a           <= lb_addr_a + 1;
                    lb_write            <= data[31:16];
                    state               <= READING;
                end
                WAITING: begin
                    avalon_master_read      <= 0;
                    lb_x                    <= 0;
                    if (blank == 0)
                        state <= WAITING;
                    else
                        state <= DRAWING;
                end
                default: begin
                    avalon_master_read  <= 0;
                    lb_x                <= 0;
                    state               <= DRAWING;
                end
            endcase
        end
    end

    always_comb  begin
        lb_addr_b = 0;
        if (blank != 0)
            lb_addr_b = DrawX;
    end

    always_ff @(posedge clk) begin
        red     <= 0;
        green   <= 0;
        blue    <= 0;
        if (blank != 0) begin
            red     <= lb_read[11:8];
            green   <= lb_read[ 7:4];
            blue    <= lb_read[ 3:0];
        end
    end

endmodule
