// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : gbtx.v
// Timestamp : Fri Oct  9 16:18:35 2026

module gbtx
(
    daq_word,
    daq_gbtx_valid,
    elink_p,
    elink_n,
    gbt_tx_datavalid,
    gbt_clk40_p,
    gbt_clk40_n,
    gbt_txrdy,
    clk160
);

    input [27:0] daq_word;
    input daq_gbtx_valid;
    output [13:0] elink_p;
    output [13:0] elink_n;
    output gbt_tx_datavalid;
    input gbt_clk40_p;
    input gbt_clk40_n;
    input gbt_txrdy;
    output clk160;

    assign gbt_tx_datavalid = 1'd1;
    wire gbt_clk160;
    wire [13:0] elink;
    reg [13:0] el0_r;
    reg [13:0] el1_r;
	dll_gbtx dllg (.CLK_IN1_P(gbt_clk40_p), .CLK_IN1_N(gbt_clk40_n), .CLK_OUT1(gbt_clk160), .RESET(0), .LOCKED());
	OBUFDS elink_buf[13:0] (.I(elink), .O(elink_p), .OB(elink_n));
	ODDR2 elink_oddr[13:0] (.D0(el0_r), .D1(el1_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(elink));
    always @(posedge gbt_clk160) 
    begin
        el0_r = daq_word[13:0];
        el1_r = daq_word[27:14];
    end
    assign clk160 = gbt_clk160;
endmodule
