// VGA screen is 640 by 480 pixels
`define DATA_OFFSET 2
`define NUM_SPRITES 32
`define SPRITE_DATA 5

module graphics_drawing_unit (
    input   logic           clk,
                            reset,

    output  logic   [31:0]  avalon_master_address,
    output  logic   [ 4:0]  avalon_master_burstcount,
    output  logic           avalon_master_read,
    input   logic   [31:0]  avalon_master_readdata,
    input   logic           avalon_master_readdatavalid,
                            avalon_master_waitrequest,
    output  logic           avalon_master_write,
    output  logic   [31:0]  avalon_master_writedata,

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
        .clock(clk),
        .data_a(avalon_slave_writedata),
        .data_b(ram_write),
        .wren_a(avalon_slave_write),
        .wren_b(ram_wren),
        .q_a(onchip_rdata),
        .q_b(ram_read)
    );

    logic   [31:0]  sprite_data [5];
    logic   [31:0]  current_frame;
    logic           blitter_start,
                    blitter_done;

    Blitter blitter (
        .clk(clk),
        .reset(reset),
        .sprite_address(sprite_data[0]),
        .sprite_dims(sprite_data[1]),
        .sprite_xy(sprite_data[2]),
        .sprite_startxy(sprite_data[3]),
        .sprite_endxy(sprite_data[4]),
        .frame_address(current_frame),
        .address(avalon_master_address),
        .burstcount(avalon_master_burstcount),
        .read(avalon_master_read),
        .readdata(avalon_master_readdata),
        .readdatavalid(avalon_master_readdatavalid),
        .waitrequest(avalon_master_waitrequest),
        .write(avalon_master_write),
        .writedata(avalon_master_writedata),
        .start(blitter_start),
        .done(blitter_done)
    );

    // Layout of each sprite in onchip memory
    // used
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
        NEXT_FRAME
    } state, return_state;

    logic   [ 2:0]  data_count;
    logic   [ 7:0]  sprite_count;

    always_ff @(posedge clk) begin
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
                    ram_addr        <= `DATA_OFFSET;
                    sprite_count    <= 0;
                    return_state    <= CHECK_SPRITE;
                    state           <= WAIT_STATE;
                end

                CHECK_SPRITE: begin
                    if (ram_read[0]) begin
                        ram_addr        <= ram_addr + 1;
                        data_count      <= 0;
                        return_state    <= READ_SPRITE
                        state           <= WAIT_STATE;
                    end
                    else begin
                        ram_addr        <= ram_addr + 6;
                        sprite_count    <= sprite_count + 1;
                        state           <= NEXT_SPRITE;
                    end
                end

                READ_SPRITE: begin
                    sprite_data[data_count] <= ram_read;
                    ram_addr                <= ram_addr + 1;
                    data_count              <= data_count + 1;
                    if (data_count == `SPRITE_DATA - 1)
                        state <= DISPLAY_SPRITE;
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
                end

                WAIT_DISPLAY_SPRITE: begin
                    if (blitter_done)
                        state <= NEXT_SPRITE;
                    else
                        state <= WAIT_DISPLAY_SPRITE;
                end

                NEXT_SPRITE: begin
                    if (sprite_count == `NUM_SPRITES)
                        state <= CHECK_SPRITE;
                    else
                        state <= NEXT_FRAME;
                end

                NEXT_FRAME: begin
                    ram_addr
                    ram_wren = 1;
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

    input   logic   [31:0]  sprite_address,
                            sprite_dims,
                            sprite_xy
                            sprite_startxy,
                            sprite_endxy,
    
    input   logic   [31:0]  frame_address,
    
    output  logic   [31:0]  address,
    output  logic   [ 4:0]  burstcount,
    output  logic           read,
    input   logic   [31:0]  readdata,
    input   logic           readdatavalid,
                            waitrequest,
    output  logic           write,
    output  logic   [31:0]  writedata,

    input   logic           start,
    output  logic           done
);



endmodule