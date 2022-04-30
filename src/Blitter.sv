module BlitterTest (
    input   logic           clk,
                            reset,
    
    output  logic   [31:0]  avalon_master_address,
    output  logic   [ 3:0]  avalon_master_burstcount,
                            avalon_master_byteenable,
    output  logic           avalon_master_read,
    input   logic   [31:0]  avalon_master_readdata,
    input   logic           avalon_master_readdatavalid,
                            avalon_master_waitrequest,
    input   logic   [ 1:0]  avalon_master_response,
    output  logic           avalon_master_write,
    output  logic   [31:0]  avalon_master_writedata,
    input   logic           avalon_master_writeresponsevalid,

    input   logic   [ 2:0]  avalon_slave_address,
    input   logic           avalon_slave_read,
    output  logic   [31:0]  avalon_slave_readdata,
    input   logic           avalon_slave_write,
    input   logic   [31:0]  avalon_slave_writedata,

    output  logic   [15:0]  out1,
    output  logic   [ 3:0]  out2
);

    logic   [31:0] registers [8];
    logic start;

    assign start = registers[0][0];

    logic   [31:0]  frame_address,
                    sprite_address,
                    sprite_dims,
                    sprite_xy,
                    sprite_startxy,
                    sprite_endxy;

    assign frame_address    = registers[1];
    assign sprite_address   = registers[2];
    assign sprite_dims      = registers[3];
    assign sprite_xy        = registers[4];
    assign sprite_startxy   = registers[5];
    assign sprite_endxy     = registers[6];

    always_ff @(posedge clk) begin
        avalon_slave_readdata <= avalon_slave_readdata;
        if (reset)
            registers <= '{default:0};
        else begin
            if (avalon_slave_read)
                avalon_slave_readdata <= registers[avalon_slave_address];
            if (avalon_slave_write)
                registers[avalon_slave_address] <= avalon_slave_writedata;
            else if (state == DONE)
                registers[0] <= 0;
            if ((state == WRITING) | (state == WRITING_ACK))
                registers[7] <= avalon_master_address;
        end
    end

    enum logic [3:0] {
        RESET,
        START,
        CHECK_BOUNDS,
        READING,
        READING_ACK,
        READING_WAIT,
        WRITING,
        WRITING_ACK,
        NEXT,
        DONE
    } state;

    logic   [31:0]  sx,
                    sy,
                    dx,
                    dy;

    logic   [1:0][15:0] pixels;
    logic   [1:0]   transparency;

    assign pixels = avalon_master_writedata;
    assign transparency = {pixels[1][12], pixels[0][12]};
    
    assign out1 = dy;
    assign out2 = state;

    always_ff @(posedge clk) begin
        avalon_master_read          <= avalon_master_read;
        avalon_master_write         <= avalon_master_write;
        avalon_master_address       <= avalon_master_address;
        avalon_master_byteenable    <= avalon_master_byteenable;
        avalon_master_writedata     <= avalon_master_writedata;
        avalon_master_burstcount    <= 1;
        dx                          <= dx;
        dy                          <= dy;
        sx                          <= sx;
        sy                          <= sy;
        if (reset) begin
            avalon_master_read  <= 0;
            avalon_master_write <= 0;
            state               <= START;
        end
        else begin
            unique case (state)
                START: begin
                    state <= START;
                    if (start) begin
                        dx      <= 32'(sprite_xy[31:16] & 16'hFFFE);
                        dy      <= 32'(sprite_xy[15: 0]);
                        sx      <= 32'(sprite_startxy[31:16] & 16'hFFFE);
                        sy      <= 32'(sprite_startxy[15: 0]);
                        state   <= CHECK_BOUNDS;
                    end
                end

                CHECK_BOUNDS: begin
                    if ((dx < 640) & (dy < 480))
                        state <= READING;
                    else
                        state <= NEXT;
                end

                READING: begin
                    avalon_master_address       <= sprite_address + (sy * (sprite_dims[31:16] >> 1) + (sx >> 1)) * 4;
                    avalon_master_read          <= 1;
                    avalon_master_byteenable    <= 4'b1111;
                    state                       <= READING_ACK;
                end

                READING_ACK: begin
                    if (avalon_master_waitrequest)
                        state <= READING_ACK;
                    else begin
                        avalon_master_read  <= 0;
                        state               <= READING_WAIT;
                    end
                end

                READING_WAIT: begin
                    if (avalon_master_readdatavalid) begin
                        avalon_master_writedata <= avalon_master_readdata;
                        state                   <= WRITING;
                    end
                    else
                        state <= READING_WAIT;
                end

                WRITING: begin
                    avalon_master_address   <= frame_address + (dy * 320 + (dx >> 1)) * 4;
                    avalon_master_write     <= 1;
                    state                   <= WRITING_ACK;
                    unique case (transparency)
                        2'b00:  begin
                            avalon_master_write <= 0;
                            state               <= NEXT;
                        end
                        2'b01:  avalon_master_byteenable <= 4'b0011;
                        2'b10:  avalon_master_byteenable <= 4'b1100;
                        2'b11:  avalon_master_byteenable <= 4'b1111;
                    endcase
                    
                end

                WRITING_ACK: begin
                    if (avalon_master_waitrequest)
                        state <= WRITING_ACK;
                    else begin
                        avalon_master_write <= 0;
                        state               <= NEXT;
                    end
                end

                NEXT: begin
                    dx      <= dx + 2;
                    sx      <= sx + 2;
                    state   <= CHECK_BOUNDS;
                    if ((dx + 2 >= 640) | (sx + 2 >= sprite_endxy[31:16])) begin
                        dx <= 32'(sprite_xy[31:16] & 16'hFFFE);
                        sx <= 32'(sprite_startxy[31:16] & 16'hFFFE);
                        dy <= dy + 1;
                        sy <= sy + 1;
                        if ((dy + 1 >= 480) | (sy + 1 >= sprite_endxy[15:0]))
                            state <= DONE;
                        else
                            state <= CHECK_BOUNDS;
                    end
                end

                DONE: begin
                    if (start)
                        state <= DONE;
                    else
                        state = START;
                end
            endcase
        end
    end

endmodule