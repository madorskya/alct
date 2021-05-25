#include "outfifo.h"

void outfifo::operator()
(		
	Signal din,
	Signal wren,
	Signal wrclk,
	Signal rden,
	Signal rdclk,
	Signal dout,
	Signal empty,
	Signal trig_info_en,
	Signal NoSpaceForDAQ
)
{
initio

	din.input(OSSIZE-1, 0, "din");
	wren.input("wren");
	wrclk.input("wrclk");
	rden.input("rden");
	rdclk.input("rdclk");
	dout.output(OSSIZE-1, 0, "dout");
#ifdef VGEN
	empty.output("empty");
#else
	empty.output("empty", makereg);
#endif
	trig_info_en.input("trig_info_en"); comment("// actually it disables writing the detected track information into the FIFO")
	NoSpaceForDAQ.output("NoSpaceForDAQ");
	
beginmodule
#ifndef VGEN
	MoreThanHalf.reg("MoreThanHalf");
#else
	MoreThanHalf.wire("MoreThanHalf");
#endif
	daqMsb.wire("daqMsb");
	hvalid.wire("hvalid");
	
	assign NoSpaceForDAQ = MoreThanHalf;
	assign daqMsb = din(27); comment("// daq(18) actually")
	assign hvalid = din(OSINPVIND) && trig_info_en; comment("// din(OSINPVIND) is validh bit")

#ifndef VGEN

	waddr.reg(7, 0, "waddr");
	raddr.reg(7, 0, "raddr");
	mem.reg(OSSIZE-1, 0, 255, 0, "mem");

	always (posedge (wrclk))
	begin
		If (wren)
		begin
			mem[waddr] = din;
			waddr++;
		end
		empty = ifelse (waddr == raddr, 1, 0);
		MoreThanHalf = ifelse (raddr-waddr < 128, 1, 0);
	end

	always (posedge (rdclk))
	begin
		If (rden)
		begin
			dout = mem[raddr];
			raddr++;
		end
	end


#else

#ifndef S6
	printv("\n\
	datafifo df\n\
	(\n\
		.din    (din),\n\
		.wr_en  (wren && (!daqMsb || hvalid)),\n\
		.wr_clk (wrclk),\n\
		.rd_en  (rden),\n\
		.rd_clk (rdclk),\n\
		.ainit  (1'b0),\n\
		.dout   (dout),\n\
		.full   (),\n\
		.empty  (empty),\n\
		.wr_count(MoreThanHalf)\n\
	);\n\
	");
#else
	printv("\n\
	datafifo df\n\
	(\n\
		.din    (din),\n\
		.wr_en  (wren && (!daqMsb || hvalid)),\n\
		.wr_clk (wrclk),\n\
		.rd_en  (rden),\n\
		.rd_clk (rdclk),\n\
		.dout   (dout),\n\
		.full   (),\n\
		.empty  (empty),\n\
		.wr_data_count(MoreThanHalf)\n\
	);\n\
	");
#endif
#endif

endmodule
}
