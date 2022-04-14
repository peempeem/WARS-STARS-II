////////////////////////////////////////
//
//		WAR STARS 2 Top Level Module
//
////////////////////////////////////////


module WS2 (
	// Master Clock
	input		logic				MAX10_CLK1_50,
	
	// FPGA Input Buttons
	input		logic	[ 1:0]	KEY,
	
	// FPGA Input Switches
	input		logic	[ 9:0]	SW,
	
	// FPGA Leds
	output	logic	[ 9:0]	LEDS,
	
	// HEX Displays
	output	logic	[ 7:0]	HEX0,
									HEX1,
									HEX2,
									HEX3,
									HEX4,
									HEX5,
	
	// SDRAM
	output	logic				DRAM_CLK,
									DRAM_CKE,
	output	logic	[12:0]	DRAM_ADDR,
	output	logic	[ 1:0]	DRAM_BA,
	inout		logic	[15:0]	DRAM_DQ,
	output	logic				DRAM_LDQM,
									DRAM_UDQM,
									DRAM_CS_N,
									DRAM_WE_N,
									DRAM_CAS_N,
									DRAM_RAS_N,
	
	// VGA
	output	logic				VGA_HS,
	output	logic				VGA_VS,
	output	logic	[ 3:0]	VGA_R,
	output	logic	[ 3:0]	VGA_G,
	output	logic	[ 3:0]	VGA_B,
	
	// Arduino
	inout		logic	[15:0]	ARDUINO_IO,
	inout		logic				ARDUINO_RESET_N
);

	// SPI wires
	logic SPI0_CS_N,
			SPI0_SCLK,
			SPI0_MISO,
			SPI0_MOSI;
	
	assign SPI0_MISO = ARDUINO_IO[12];
	
	// USB wires
	logic	USB_GPX,
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
	HexDriver(1'b0, 4'bX, HEX0);
	HexDriver(1'b0, 4'bX, HEX1);
	HexDriver(1'b0, 4'bX, HEX2);
	HexDriver(1'b0, 4'bX, HEX3);
	HexDriver(1'b0, 4'bX, HEX4);
	HexDriver(1'b0, 4'bX, HEX5);
	
endmodule
