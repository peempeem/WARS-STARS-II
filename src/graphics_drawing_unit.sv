module graphics_drawing_unit (
    input   logic           clk0,
                            clk1,
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

    input   logic   [10:0]  avalon_slave_address,
    input   logic           avalon_slave_read,
    output  logic   [31:0]  avalon_slave_readdata,
    input   logic           avalon_slave_write,
    input   logic   [31:0]  avalon_slave_writedata
);

    logic   [10:0]  ram_addr;
    logic   [31:0]  ram_read,
                    ram_write;
    logic           ram_wren;

    gdu_ram ram (
        .address_a(avalon_slave_address),
        .address_b(ram_addr),
        .clock(clk0),
        .data_a(avalon_slave_writedata),
        .data_b(ram_write),
        .wren_a(avalon_slave_write),
        .wren_b(ram_wren),
        .q_a(avalon_slave_readdata),
        .q_b(ram_read)
    );

    logic   [31:0]  sprite_data [5];
    logic   [31:0]  current_frame;
    logic           blitter_start,
                    blitter_done;

    Blitter blitter (
        .clk                (clk1),
        .reset              (reset),
        .frame_address      (current_frame),
        .sprite_address     (sprite_data[0]),
        .sprite_dims        (sprite_data[1]),
        .sprite_xy          (sprite_data[2]),
        .sprite_startxy     (sprite_data[3]),
        .sprite_endxy       (sprite_data[4]),
        .address            (avalon_master_address),
        .burstcount         (avalon_master_burstcount),
        .byteenable         (avalon_master_byteenable),
        .read               (avalon_master_read),
        .readdata           (avalon_master_readdata),
        .readdatavalid      (avalon_master_readdatavalid),
        .waitrequest        (avalon_master_waitrequest),
        .response           (avalon_master_response),
        .write              (avalon_master_write),
        .writedata          (avalon_master_writedata),
        .writeresponsevalid (avalon_master_writeresponsevalid),
        .start              (blitter_start),
        .done               (blitter_done)
    );

    // Layout of each sprite in onchip memory
    // address
    // width, height
    // screen_x, screen_y
    // start_x, start_y
    // end_x, end_y

    enum logic [3:0] {
        ENTRY,
        START,
        LOAD_FRAME,
        CHECK_SPRITE,
        READ_SPRITE,
        WAIT_STATE,
        DISPLAY_SPRITE,
        WAIT_DISPLAY_SPRITE,
        NEXT_SPRITE,
        DONE
    } state, return_state;

    logic   [ 2:0]  data_count;
    logic   [ 7:0]  sprite_count;

    always_ff @(posedge clk0) begin
        ram_addr        <= ram_addr;
        ram_wren        <= 0;
        data_count      <= data_count;
        sprite_count    <= sprite_count;
        return_state    <= return_state;
        blitter_start   <= 0;
        if (reset)
            state <= ENTRY;
        else begin
            unique case (state)
                ENTRY: begin
                    ram_addr        <= 0;
                    return_state    <= START;
                    state           <= WAIT_STATE;
                end

                START: begin
                    if (ram_read[0] == 1) begin
                        ram_addr        <= 1;
                        return_state    <= LOAD_FRAME;
                        state           <= WAIT_STATE;
                    end
                    else
                        state <= ENTRY;
                end

                LOAD_FRAME: begin
                    current_frame   <= ram_read;
                    ram_addr        <= 2;
                    sprite_count    <= 0;
                    return_state    <= CHECK_SPRITE;
                    state           <= WAIT_STATE;
                end

                CHECK_SPRITE: begin
                    if (ram_read != 0) begin
                        sprite_data[0]  <= ram_read;
                        ram_addr        <= ram_addr + 1;
                        data_count      <= 1;
                        return_state    <= READ_SPRITE;
                        state           <= WAIT_STATE;
                    end
                    else begin
                        ram_addr        <= ram_addr + 5;
                        sprite_count    <= sprite_count + 1;
                        state           <= NEXT_SPRITE;
                    end
                end

                READ_SPRITE: begin
                    sprite_data[data_count] <= ram_read;
                    ram_addr                <= ram_addr + 1;
                    data_count              <= data_count + 1;
                    if (data_count == 4) begin
                        return_state    <= DISPLAY_SPRITE;
                        state           <= WAIT_STATE;
                    end
                    else begin
                        return_state    <= READ_SPRITE;
                        state           <= WAIT_STATE;
                    end
                end

                WAIT_STATE: begin
                    state <= return_state;
                end

                DISPLAY_SPRITE: begin
                    sprite_count    <= sprite_count + 1;
                    blitter_start   <= 1;
                    state           <= WAIT_DISPLAY_SPRITE;
                end

                WAIT_DISPLAY_SPRITE: begin
                    if (blitter_done)
                        state <= NEXT_SPRITE;
                    else
                        state <= WAIT_DISPLAY_SPRITE;
                end

                NEXT_SPRITE: begin
                    if (sprite_count >= 64)
                        state <= DONE;
                    else
                        state <= CHECK_SPRITE;
                end

                DONE: begin
                    ram_addr        <= 0;
                    ram_wren        <= 1;
                    ram_write       <= 0;
                    return_state    <= ENTRY;
                    state           <= WAIT_STATE;
                end

                default: begin
                    state <= ENTRY;
                end
            endcase
        end
    end

endmodule

module Blitter (
    input   logic           clk,
                            reset,

    input   logic   [31:0]  frame_address,
                            sprite_address,
                            sprite_dims,
                            sprite_xy,
                            sprite_startxy,
                            sprite_endxy,
    
    output  logic   [31:0]  address,
    output  logic   [ 3:0]  burstcount,
                            byteenable,
    output  logic           read,
    input   logic   [31:0]  readdata,
    input   logic           readdatavalid,
                            waitrequest,
    input   logic   [ 1:0]  response,
    output  logic           write,
    output  logic   [31:0]  writedata,
    input   logic           writeresponsevalid,
    
    input   logic           start,
    output  logic           done
);   

    enum logic [3:0] {
        START,
        CHECK_BOUNDS,
        READING,
        READING_ACK,
        READING_WAIT,
        WRITING,
        WRITING_ACK,
        NEXT
    } state;

    logic   [15:0]  sx,
                    sy,
                    dx,
                    dy;
    
    logic   [1:0][15:0] pixels;
    logic   [1:0]       transparency;

    logic   [31:0]  buffer [8];
    logic   [ 3:0]  buffer_idx,
                    write_bytes;

    assign pixels = buffer[buffer_idx];
    assign transparency = {pixels[1][12], pixels[0][12]};

    always_ff @(posedge clk) begin
        read        <= read;
        write       <= write;
        address     <= address;
        byteenable  <= byteenable;
        writedata   <= writedata;
        burstcount  <= burstcount;
        dx          <= dx;
        dy          <= dy;
        sx          <= sx;
        sy          <= sy;
        write_bytes <= write_bytes;
        if (reset) begin
            read    <= 0;
            write   <= 0;
            state   <= START;
        end
        else begin
            unique case (state)
                START: begin
                    state   <= START;
                    done    <= 1;
                    if (start) begin
                        done    <= 0;
                        dx      <= sprite_xy[31:16] & 16'hFFFE;
                        dy      <= sprite_xy[15: 0];
                        sx      <= sprite_startxy[31:16] & 16'hFFFE;
                        sy      <= sprite_startxy[15: 0];
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
                    burstcount  <= 8;
                    buffer_idx  <= 0;
                    address     <= sprite_address + (sy * (sprite_dims[31:16] >> 1) + (sx >> 1)) * 4;
                    read        <= 1;
                    byteenable  <= 4'b1111;
                    state       <= READING_ACK;
                end

                READING_ACK: begin
                    write_bytes <= 0;
                    if (waitrequest)
                        state <= READING_ACK;
                    else begin
                        read    <= 0;
                        state   <= READING_WAIT;
                    end
                end

                READING_WAIT: begin
                    if (readdatavalid) begin
                        buffer[buffer_idx]  <= readdata;
                        sx                  <= sx + 2;
                        buffer_idx          <= buffer_idx + 1;
                        if (sx + 2 <= (sprite_endxy[31:16] & 16'hFFFE))
                            write_bytes <= write_bytes + 1;
                        if (buffer_idx + 1 >= burstcount) begin
                            buffer_idx  <= 0;
                            state       <= WRITING;
                        end
                        else
                            state <= READING_WAIT;
                    end
                    else
                        state <= READING_WAIT;
                end

                WRITING: begin
                    address     <= frame_address + (dy * 320 + (dx >> 1)) * 4;
                    burstcount  <= 1;
                    write       <= 1;
                    writedata   <= buffer[buffer_idx];
                    state       <= WRITING_ACK;
                    unique case (transparency)
                        2'b00:  byteenable <= 4'b0000;
                        2'b01:  byteenable <= 4'b0011;
                        2'b10:  byteenable <= 4'b1100;
                        2'b11:  byteenable <= 4'b1111;
                    endcase
                end

                WRITING_ACK: begin
                    if (waitrequest)
                        state <= WRITING_ACK;
                    else begin
                        buffer_idx <= buffer_idx + 1;
                        dx <= dx + 2;
                        if ((buffer_idx + 1 >= write_bytes)  | (dx >= 640)) begin
                            write <= 0;
                            state <= NEXT;
                        end
                        else
                            state <= WRITING;
                    end
                end

                NEXT: begin
                    state <= CHECK_BOUNDS;
                    if ((sx >= (sprite_endxy[31:16] & 16'hFFFE)) | (dx >= 640)) begin
                        dx <= sprite_xy[31:16] & 16'hFFFE;
                        sx <= sprite_startxy[31:16] & 16'hFFFE;
                        dy <= dy + 1;
                        sy <= sy + 1;
                        if (sy + 1 >= sprite_endxy[15:0]) begin
                            done    <= 1;
                            state   <= START;
                        end
                    end
                end

                default: state <= START;
            endcase
        end
    end

endmodule
