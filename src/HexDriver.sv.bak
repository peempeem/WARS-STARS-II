module HexDriver (
	input logic En,
	input logic [3:0] In,
	output logic [7:0] Out
);
	
	always_comb begin
		if (~En)
			Out = 8'b11111111;
		else begin
			unique case (In)
				4'b0000: Out = 8'b11000000; // '0'
				4'b0001: Out = 8'b11111001; // '1'
				4'b0010: Out = 8'b10100100; // '2'
				4'b0011:	Out = 8'b10110000; // '3'
				4'b0100:	Out = 8'b10011001; // '4'
				4'b0101:	Out = 8'b10010010; // '5'
				4'b0110:	Out = 8'b10000010; // '6'
				4'b0111:	Out = 8'b11111000; // '7'
				4'b1000:	Out = 8'b10000000; // '8'
				4'b1001:	Out = 8'b10010000; // '9'
				4'b1010:	Out = 8'b10001000; // 'A'
				4'b1011:	Out = 8'b10000011; // 'b'
				4'b1100:	Out = 8'b11000110; // 'C'
				4'b1101:	Out = 8'b10100001; // 'd'
				4'b1110:	Out = 8'b10000110; // 'E'
				4'b1111:	Out = 8'b10001110; // 'F'
			endcase
		end
	end

endmodule
