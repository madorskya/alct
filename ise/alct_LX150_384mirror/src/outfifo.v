// This  Verilog HDL  source  file was  automatically generated
// by C++ model based on VPP library. Modification of this file
// is possible, but if you want to keep it in sync with the C++
// model,  please  modify  the model and re-generate this file.
// VPP library web-page: http://www.phys.ufl.edu/~madorsky/vpp/

// Author    : madorsky
// File name : outfifo.v
// Timestamp : Mon Apr 25 18:06:17 2022

module outfifo
(
    din,
    wren,
    wrclk,
    rden,
    rdclk,
    dout,
    empty,
    trig_info_en,
    NoSpaceForDAQ
);

    input [47:0] din;
    input wren;
    input wrclk;
    input rden;
    input rdclk;
    output [47:0] dout;
    output empty;
    input trig_info_en;
    // actually it disables writing the detected track information into the FIFO
    output NoSpaceForDAQ;

    wire MoreThanHalf;
    wire daqMsb;
    wire hvalid;
    assign NoSpaceForDAQ = MoreThanHalf;
    assign daqMsb = din[27];
    // daq(18) actually
    assign hvalid = din[38] && trig_info_en;
    // din(OSINPVIND) is validh bit

	datafifo df
	(
		.din    (din),
		.wr_en  (wren && (!daqMsb || hvalid)),
		.wr_clk (wrclk),
		.rd_en  (rden),
		.rd_clk (rdclk),
		.dout   (dout),
		.full   (),
		.empty  (empty),
		.wr_data_count(MoreThanHalf)
	);
	endmodule
