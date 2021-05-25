#include "bestfifo.h"

void bestfifo::operator()
(
	Signal clk,       
	Signal sinit,     
	Signal din,       
	Signal wr_en,     
	Signal rd_en,     
	Signal dout,      
	Signal empty,     
	Signal data_count
)
{
initio
	clk.input("clk");
	sinit.input("sinit");
	din.input(49, 0, "din");
	wr_en.input("wr_en");
	rd_en.input("rd_en");
	dout.output(49, 0, "dout", makereg);
	empty.output("empty", makereg);
	data_count.output(7, 0, "data_count", makereg);
beginmodule

	waddr.reg(7, 0, "waddr");
	raddr.reg(7, 0, "raddr");
	mem.reg(49, 0, 255, 0, "mem");

	always (posedge (clk))
	begin
		If (sinit)
		begin
			waddr = 0;
			raddr = 0;
		end
		Else
		begin
			If (wr_en)
			begin
				mem[waddr] = din;
				waddr++;
			end

			If (rd_en && !empty)
			begin
				dout = mem[raddr];
				raddr++;
			end
		end

		data_count = waddr - raddr;
		empty = ifelse (waddr == raddr, 1, 0);
	end

endmodule
}
