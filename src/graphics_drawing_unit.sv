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

    gdu_ram ram (
        .address_a(AVL_ADDR),
        .address_b(),
        .byteena_a(AVL_BYTE_EN),
        .clock(CLK),
        .data_a(AVL_WRITEDATA),
        .data_b(),
        .wren_a(AVL_WRITE),
        .wren_b(1'b0),
        .q_a(AVL_READDATA),
        .q_b()
    );

    logic   pixel_clk,
            blank,
            sync;
    logic [9:0] DrawX,
                DrawY;
    
    vga_controller vgac (
        .Clk(CLK),
        .Reset(RESET),
        .hs(hs),
        .vs(vs),
        .pixel_clk(pixel_clk),
        .blank(blank),
        .sync(sync),
        .DrawX(DrawX),
        .DrawY(DrawY)
    );

    logic [3:0] r, g, b;

    always_ff @(posedge pixel_clk) begin
        red     <= r;
        green   <= g;
        blue    <= b;
    end

    always_comb begin
        if (blank == 1'b1) begin
            unique case (DrawX[1:0])
                2'b00: begin
                    r = 4'hF;
                    g = 4'h0;
                    b = 4'h0;
                end
                2'b01: begin
                    r = 4'h0;
                    g = 4'hF;
                    b = 4'h0;
                end
                2'b10: begin
                    r = 4'h0;
                    g = 4'h0;
                    b = 4'hF;
                end
                2'b11: begin
                    r = 4'hF;
                    g = 4'hF;
                    b = 4'hF;
                end
            endcase
        end else begin
            r = 4'h0;
            g = 4'h0;
            b = 4'h0;
        end
    end

endmodule
