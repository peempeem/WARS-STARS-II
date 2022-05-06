`define FRAME_SIZE 32'h00096000

module vga_display (
    input   logic           clk,
                            reset,
    
    output  logic   [31:0]  avalon_master_address,
    output  logic   [ 4:0]  avalon_master_burstcount,
    output  logic           avalon_master_read,
    input   logic   [31:0]  avalon_master_readdata,
    input   logic           avalon_master_readdatavalid,
                            avalon_master_waitrequest,
    
    input   logic   [ 2:0]  avalon_slave_address,
    input   logic           avalon_slave_read,
    output  logic   [31:0]  avalon_slave_readdata,
    input   logic           avalon_slave_write,
    input   logic   [31:0]  avalon_slave_writedata,

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

    logic   [31:0] registers [5];

    always_ff @(posedge clk) begin
        avalon_slave_readdata <= avalon_slave_readdata;
        if (reset)
            registers <= '{default:0};
        else begin
            if (avalon_slave_read)
                avalon_slave_readdata <= registers[avalon_slave_address];
            else if (avalon_slave_write)
                registers[avalon_slave_address] <= avalon_slave_writedata;
            if (DrawY >= 480)
                registers[4] <= 32'h1;
            else
                registers[4] <= 32'h0;
        end
    end

    logic   [ 8:0]  lb_addr_a,
                    lb_addr_b;
    logic   [31:0]  lb_write,
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

    enum logic [2:0] {
        DRAWING,
        READING,
        READING_ACK,
        WRITING,
        WAITING
    } state;

    logic   [8:0]       lb_x;
    logic   [9:0]       lb_y;
    logic   [3:0]       rd_bytes;
    logic               frame;

    always_ff @(posedge clk) begin
        avalon_master_read          <= avalon_master_read;
        avalon_master_address       <= avalon_master_address;
        avalon_master_burstcount    <= avalon_master_burstcount;
        lb_x                        <= lb_x;
        lb_y                        <= lb_y;
        lb_wren                     <= 0;
        lb_addr_a                   <= lb_addr_a;
        lb_write                    <= lb_write;
        rd_bytes                    <= rd_bytes;
        frame                       <= frame;
        if (reset) begin
            avalon_master_read  <= 0;
            state               <= DRAWING;
        end
        else begin
            unique case (state)
                DRAWING: begin
                    avalon_master_read  <= 0;
                    lb_x                <= 0;
                    if (blank == 0) begin
                        if (DrawY >= 479) begin
                            lb_y <= 0;
                        end
                        else
                            lb_y <= DrawY + 1;
                        state <= READING;
                    end
                    else
                        state <= DRAWING;
                end

                READING: begin
                    if (lb_x < 320) begin
                        if (registers[1][0])
                            avalon_master_address <= registers[0] + `FRAME_SIZE + lb_y * 1280 + lb_x * 4;
                        else
                            avalon_master_address <= registers[0] + lb_y * 1280 + lb_x * 4;
                        avalon_master_read          <= 1;
                        avalon_master_burstcount    <= 8;
                        rd_bytes                    <= 8;
                        state                       <= READING_ACK;
                    end
                    else begin
                        state <= WAITING;
                    end
                end

                READING_ACK: begin
                    if (avalon_master_waitrequest)
                        state <= READING_ACK;
                    else begin
                        avalon_master_read  <= 0;
                        state               <= WRITING;
                    end
                end

                WRITING: begin
                    if (avalon_master_readdatavalid) begin
                        lb_x        <= lb_x + 1;
                        lb_wren     <= 1;
                        lb_addr_a   <= lb_x;
                        lb_write    <= avalon_master_readdata;
                        rd_bytes    <= rd_bytes - 1;
                        if (rd_bytes == 1)
                            state <= READING;
                        else
                            state <= WRITING;
                    end
                    else
                        state <= WRITING;
                end

                WAITING: begin
                    avalon_master_read  <= 0;
                    lb_x                <= 0;
                    if ((blank == 1) | (DrawY >= lb_y))
                        state <= DRAWING;
                    else
                        state <= WAITING;
                end
                
                default: begin
                    avalon_master_read  <= 0;
                    lb_x                <= 0;
                    state               <= DRAWING;
                end
            endcase
        end
    end

    logic   [15:0]  main_color,
                    bias_color;
    logic   [ 7:0]  rgb_bias;
    logic   [ 3:0]  rgb [3];

    always_ff @(posedge clk) begin
        if (DrawY >= 479) begin
            bias_color  <= registers[2][15:0];
            rgb_bias    <= registers[3][ 7:0];
        end
    end

    always_comb  begin
        lb_addr_b = 0;
        if (blank != 0)
            lb_addr_b = DrawX[9:1];
        if (DrawX[0])
            main_color = lb_read[31:16];
        else
            main_color = lb_read[15: 0];
        rgb[0]  = main_color[ 3:0] + ((rgb_bias * (bias_color[ 3:0] - main_color[ 3:0])) / 64);
        rgb[1]  = main_color[ 7:4] + ((rgb_bias * (bias_color[ 7:4] - main_color[ 7:4])) / 64);
        rgb[2]  = main_color[11:8] + ((rgb_bias * (bias_color[11:8] - main_color[11:8])) / 64);
    end

    always_ff @(posedge pixel_clk) begin
        red     <= 0;
        green   <= 0;
        blue    <= 0;
        if (blank != 0) begin
            red     <= rgb[2];
            green   <= rgb[1];
            blue    <= rgb[0];
        end
    end

endmodule
