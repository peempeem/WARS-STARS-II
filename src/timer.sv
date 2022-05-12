module Timer (
    input   logic           clk0,
                            clk1,
                            reset,

    input   logic   [ 1:0]  avalon_slave_address,
    input   logic           avalon_slave_read,
    output  logic   [31:0]  avalon_slave_readdata,
    input   logic           avalon_slave_write,
    input   logic   [31:0]  avalon_slave_writedata
);

    logic   [31:0]  enable;
    logic   [31:0]  count_low;
    logic   [31:0]  count_high;
    logic   [63:0]  counter;

    assign count_low    = counter[31: 0];
    assign count_high   = counter[63:32];

    always_ff @(posedge clk1) begin
        if (enable[0])
            counter <= counter + 1;
        else
            counter <= 0;
    end

    always_ff @(posedge clk0) begin
        enable                  <= enable;
        avalon_slave_readdata   <= avalon_slave_readdata;
        if (reset) begin
            enable <= 0;
        end
        else begin
            unique case (avalon_slave_address)
                0: begin
                    if (avalon_slave_read)
                        avalon_slave_readdata <= enable;
                    if (avalon_slave_write)
                        enable <= avalon_slave_writedata;
                end

                1: begin
                    if (avalon_slave_read)
                        avalon_slave_readdata <= count_low;
                end

                2: begin
                    if (avalon_slave_read)
                        avalon_slave_readdata <= count_high;
                end

                3: begin
                    if (avalon_slave_read)
                        avalon_slave_readdata <= 0;
                end
            endcase
        end
    end

endmodule
