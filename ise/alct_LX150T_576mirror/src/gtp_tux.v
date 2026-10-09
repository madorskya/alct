// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : mador
// File name : gtp_tux.v
// Timestamp : Fri Oct  9 16:20:54 2026

module gtp_tux
(
    daqo,
    daq_gbtx_valid,
    clk,
    tx_p,
    tx_n,
    refclk_p,
    refclk_n,
    reset,
    clk160
);

    input [27:0] daqo;
    input daq_gbtx_valid;
    input clk;
    output [1:0] tx_p;
    output [1:0] tx_n;
    input refclk_p;
    input refclk_n;
    input reset;
    output clk160;

	optical_lx150t gtp
	(
		.clock     (clk),
		.daq_word  (daqo),
		.daq_valid (daq_gbtx_valid),
		.tx_p      (tx_p),
		.tx_n      (tx_n),
		.refclk_p  (refclk_p),
		.refclk_n  (refclk_n),
		.reset_i   (reset),
		.clk160    (clk160)
	);
endmodule
