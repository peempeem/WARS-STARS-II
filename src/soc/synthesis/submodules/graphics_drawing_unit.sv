// VGA screen is 640 by 480 pixels

module graphics_drawing_unit (
    input   logic           CLK,
                            RESET,
    
    input   logic           AVL_READ,
                            AVL_WRITE,
                            AVL_CS,
    input   logic   [ 3:0]  AVL_BYTE_EN,
    input   logic   [11:0]  AVL_ADDR,
    input   logic   [31:0]  AVL_WRITEDATA,
    output  logic   [31:0]  AVL_READDATA,

    output  logic   [ 3:0]  red,
                            green,
                            blue,
    output logic            hs,
                            vs
);

    logic   [11:0]  gdu_ram_addr;
    logic   [31:0]  gdu_ram_data;

    gdu_ram ram (
        .address_a(AVL_ADDR),
        .address_b(gdu_ram_addr),
        .byteena_a(AVL_BYTE_EN),
        .clock(CLK),
        .data_a(AVL_WRITEDATA),
        .data_b(),
        .wren_a(AVL_WRITE),
        .wren_b(1'b0),
        .q_a(AVL_READDATA),
        .q_b(gdu_ram_data)
    );

    logic   [16:0]  gdu_vram_addr_a,
                    gdu_vram_addr_b;
    logic           gdu_vram_wren;
    logic   [63:0]  gdu_vram_w_data;
    logic   [ 7:0]  gdu_vram_byte_en;
    logic   [63:0]  gdu_vram_data_a,
                    gdu_vram_data_b;

    gdu_vram vram (
        .clk(CLK),
        .addr_a(gdu_vram_addr_a),
        .addr_b(gdu_vram_addr_b),
        .wren(gdu_vram_wren),
        .w_data(gdu_vram_w_data),
        .byte_en(gdu_vram_byte_en),
        .data_a(gdu_vram_data_a),
        .data_b(gdu_vram_data_b)
    );

    vga_display display (
        .clk(CLK),
        .reset(RESET),
        .frame(0),
        .vram_data(gdu_vram_data_b),
        .vram_addr(gdu_vram_addr_b),
        .hs(hs),
        .vs(vs),
        .red(red),
        .green(green),
        .blue(blue)
    );

    always_comb begin
        gdu_vram_w_data     = 64'hFFF;
        gdu_vram_byte_en    = 8'hFF;
        gdu_vram_addr_a     = 3890;
        gdu_vram_wren       = 1'b1;
    end

    logic   [12:0]  lt_rom_addr;
    logic   [31:0]  lt_rom_data;

    large_text lt_rom (
        .address(lt_rom_addr),
        .clock(CLK),
        .q(lt_rom_data)
    );

    logic   [10:0]  nt_rom_addr;
    logic   [ 7:0]  nt_rom_data;

    normal_text nt_rom (
        .address(nt_rom_addr),
        .clock(CLK),
        .q(nt_rom_data)
    );

endmodule

module gdu_vram (
    input   logic           clk,
    input   logic   [16:0]  addr_a,
                            addr_b,
    input   logic           wren,
    input   logic   [63:0]  w_data,
    input   logic   [7:0]   byte_en,
    output  logic   [63:0]  data_a,
                            data_b
);

    logic   [63:0]  fb0_a, fb0_b, fb1_a, fb1_b;

    framebuffer fb0 (
        .address_a(addr_a[15:0]),
        .address_b(addr_b[15:0]),
        .byteena_a(byte_en),
        .clock(clk),
        .data_a(w_data),
        .data_b(),
        .wren_a(wren & ~addr_a[16]),
        .wren_b(1'b0),
        .q_a(fb0_a),
        .q_b(fb0_b)
    );

    framebuffer fb1 (
        .address_a(addr_a[15:0]),
        .address_b(addr_b[15:0]),
        .byteena_a(byte_en),
        .clock(clk),
        .data_a(w_data),
        .data_b(),
        .wren_a(wren & addr_a[16]),
        .wren_b(1'b0),
        .q_a(fb1_a),
        .q_b(fb1_b)
    );

    always_comb begin
        if (addr_a[16])
            data_a = fb0_a;
        else
            data_a = fb1_a;
        if (addr_b[16])
            data_b = fb0_b;
        else
            data_b = fb1_b;
    end

endmodule

module vga_display (
    input   logic           clk,
                            reset,
    input   logic           frame,
    input   logic   [63:0]  vram_data,
    output  logic   [16:0]  vram_addr,
    output  logic           hs,
                            vs,
    output  logic   [ 3:0]  red,
                            green,
                            blue
);

    parameter BLANK_SIG = 1'b0;

    assign vram_addr[16] = frame;

    logic   [4:0][3:0][4:0] pixels;
    assign pixels[4:0] = vram_data[59:0];

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

    logic   [3:0]   r, g, b;

    always_ff @(posedge pixel_clk) begin
        if (blank != BLANK_SIG) begin
            red     <= r;
            green   <= g;
            blue    <= b;
        end else begin
            red     <= 4'h0;
            green   <= 4'h0;
            blue    <= 4'h0;
        end
    end

    always_ff @(posedge clk) begin
        if (blank != BLANK_SIG)
            vram_addr[15:0] <= DrawY * 128 + (DrawX / 5);
        else
            vram_addr[15:0] <= vram_addr[15:0];
    end

    logic   [2:0]   pxl;

    always_comb begin
        pxl = DrawX % 5;
        r   = pixels[pxl][0];
        g   = pixels[pxl][1];
        b   = pixels[pxl][2];
    end

endmodule
