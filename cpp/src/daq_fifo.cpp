#include "daq_fifo.h"

void daq_fifo::operator()
(
	Signal din,       
	Signal dout,      
	Signal wen,     
	Signal ren,     
	Signal reset,     
	Signal empty,     
	Signal full,
	Signal clk      
)
{
initio
	Input_(din, bwd-1, 0);
	Output_(dout, bwd-1, 0);
	Input (wen);
	Input (ren);
	Input (reset);
	OutReg (empty);
	OutReg (full);
	Input (clk);

beginmodule

	Reg__(mem, bwd-1, 0, (1<<bwad)-1, 0);

#if defined VGEN
	if (distributed)
	 	comment("// synthesis attribute ram_style of mem is distributed")
	else
 		comment("// synthesis attribute ram_style of mem is block")
#endif

	Reg_(waddr , 7, 0) ;
	Reg_(raddr , 7, 0) ;
	Reg_(raddrr, 7, 0) ;

	always (posedge (clk))
	begin

		If (wen && !full)
		begin
			mem[waddr] = din;
			waddr++;
		end

		If (ren && !empty)
		begin
			raddrr = raddr;
			raddr++;
		end

		If (reset)
		begin
			waddr = 0;
			raddr = 0;
		end

		full = (waddr + 1) == raddr;
		empty = waddr == raddr;
		
	end

	assign dout = mem[raddrr];
endmodule
}
