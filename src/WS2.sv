////////////////////////////////////////
//
//		WAR STARS 2 Top Level Module
//
////////////////////////////////////////





module WS2 (
    // Master Clock
    input   logic           MAX10_CLK1_50,
								    MAX10_CLK2_50,
    
	 output  logic   [ 1:0] aud_mclk_ctr,
	 
    // FPGA Input Buttons
    input   logic   [ 1:0]  KEY,
    
    // FPGA Input Switches
    input   logic   [ 9:0]  SW,
    
    // FPGA Leds
    output  logic   [ 9:0]  LEDS,
    
    // HEX Displays
    output  logic   [ 7:0]  HEX0,
                            HEX1,
                            HEX2,
                            HEX3,
                            HEX4,
                            HEX5,
    
    // SDRAM
    output  logic           DRAM_CLK,
                            DRAM_CKE,
    output  logic   [12:0]  DRAM_ADDR,
    output  logic   [ 1:0]  DRAM_BA,
    inout   logic   [15:0]  DRAM_DQ,
    output  logic           DRAM_LDQM,
                            DRAM_UDQM,
                            DRAM_CS_N,
                            DRAM_WE_N,
                            DRAM_CAS_N,
                            DRAM_RAS_N,
    
    // VGA
    output  logic           VGA_HS,
    output  logic           VGA_VS,
    output  logic   [ 3:0]  VGA_R,
    output  logic   [ 3:0]  VGA_G,
    output  logic   [ 3:0]  VGA_B,
    
    // Arduino
    inout   logic   [15:0]  ARDUINO_IO,
    inout   logic           ARDUINO_RESET_N
);

    // SPI wires
    logic   SPI0_CS_N,
            SPI0_SCLK,
            SPI0_MISO,
            SPI0_MOSI;
    
    assign SPI0_MISO = ARDUINO_IO[12];
    
    // USB wires
    logic   USB_GPX,
            USB_IRQ,
            USB_RST;
    
    assign USB_GPX = 1'b0;
    assign USB_IRQ = ARDUINO_IO[9];
    
    // Setup SPI/USB communication fields
    assign ARDUINO_IO[ 7] = USB_RST;
    assign ARDUINO_IO[ 8] = 1'bZ;
    assign ARDUINO_IO[ 9] = 1'bZ;
    assign ARDUINO_IO[10] = SPI0_CS_N;
    assign ARDUINO_IO[11] = SPI0_MOSI;
    assign ARDUINO_IO[12] = 1'bZ;
    assign ARDUINO_IO[13] = SPI0_SCLK;
    
    // Setup HexDrivers
    HexDriver hex0 (1'b0, 4'bX, HEX0);
    HexDriver hex1 (1'b0, 4'bX, HEX1);
    HexDriver hex2 (1'b0, 4'bX, HEX2);
    HexDriver hex3 (1'b0, 4'bX, HEX3);
    HexDriver hex4 (1'b0, 4'bX, HEX4);
    HexDriver hex5 (1'b0, 4'bX, HEX5);

    // Setup LEDS
    assign LEDS = 10'h0;
	 
	 assign i2c_serial_ARDUINO_IO[15]_in = arduino_adc_ARDUINO_IO[15];
	 assign arduino_ado_ARDUINO_IO[15]   = i2c_serial_ARDUINO_IO[15]_oe ? 1'b0 : 1 'bz;

	 assign i2c_serial_ARDUINO_IO[14]_in = arduino_adc_ARDUINO_IO[14];
	 assign arduino_adc_ARDUINO_IO[14]   = i2c_serial_ARDUINO_IO[14]_oe ? 1'b0 : 1'bz;

	 assign ARDUINO_IO[3] = aud_mclk_ctr[1];	//generate 12.5MHz CODEC mclk
		
	 always_ff @(posedge MAX10_CLK2_50) begin
aud_mclk_ctr <= aud_mclk_ctr + 1;
		end
	 
    soc soc (
        .clk_clk                (MAX10_CLK1_50),
        .reset_reset_n          (KEY[0]),
        .sdram_clk_clk          (DRAM_CLK),
        .sdram_wire_addr        (DRAM_ADDR),
        .sdram_wire_ba          (DRAM_BA),
        .sdram_wire_cas_n       (DRAM_CAS_N),
        .sdram_wire_cke         (DRAM_CKE),
        .sdram_wire_cs_n        (DRAM_CS_N),
        .sdram_wire_dq          (DRAM_DQ),
        .sdram_wire_dqm         ({DRAM_UDQM, DRAM_LDQM}),
        .sdram_wire_ras_n       (DRAM_RAS_N),
        .sdram_wire_we_n        (DRAM_WE_N),
        .spi0_MISO              (SPI0_MISO),
        .spi0_MOSI              (SPI0_MOSI),
        .spi0_SCLK              (SPI0_SCLK),
        .spi0_SS_n              (SPI0_CS_N),
        .usb_gpx_export         (USB_GPX),
        .usb_irq_export         (USB_IRQ),
        .usb_rst_export         (USB_RST),
        .vga_port_red           (VGA_R),
        .vga_port_green         (VGA_G),
        .vga_port_blue          (VGA_B),
        .vga_port_hs            (VGA_HS),
        .vga_port_vs            (VGA_VS)
    );
    
endmodule

