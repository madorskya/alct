// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : gbtx.v
// Timestamp : Thu Feb  3 17:20:01 2022

module gbtx
(
    daq_word,
    clk,
    elink_p,
    elink_n,
    gbt_tx_datavalid,
    gbt_clk40_p,
    gbt_clk40_n,
    gbt_txrdy,
    rst
);

    input [18:0] daq_word;
    input clk;
    output [13:0] elink_p;
    output [13:0] elink_n;
    output gbt_tx_datavalid;
    input gbt_clk40_p;
    input gbt_clk40_n;
    input gbt_txrdy;
    input rst;

    wire gbt_clk40;
    wire gbt_clk160;
    wire [13:0] elink;
    wire [13:0] el0;
    wire [13:0] el1;
    reg [13:0] el0_r;
    reg [13:0] el1_r;
    wire dv;
    reg dv_r;
	dll_gbtx dllg (.CLK_IN1_P(gbt_clk40_p), .CLK_IN1_N(gbt_clk40_n), .CLK_OUT1(gbt_clk160), .RESET(0), .LOCKED());
	OBUFDS elink_buf[13:0] (.I(elink), .O(elink_p), .OB(elink_n));
	ODDR2 elink_oddr[13:0] (.D0(el0_r), .D1(el1_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(elink));
	ODDR2 dval_oddr (.D0(dv_r), .D1(dv_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(gbt_tx_datavalid));
	gbtx_fifo gbtxf (
		.rst      (rst),
		.wr_clk   (clk),
		.rd_clk   (gbt_clk160),
		.din      ({5'b00000, daq_word[18:10], 4'b1000, daq_word[9:0]}),
		.wr_en    (~daq_word[18]),
		.rd_en    (1'b1),
		.dout     ({el1, el0}),
		.full     (),
		.valid    (dv),
		.empty    ()
	);
    always @(posedge gbt_clk160) 
    begin
        el0_r = el0;
        el1_r = el1;
        dv_r = dv;
    end
endmodule
