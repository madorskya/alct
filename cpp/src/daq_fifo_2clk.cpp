#include "daq_fifo_2clk.h"

void daq_fifo_2clk::operator()
(
	Signal din,       
	Signal dout,      
	Signal wen,     
	Signal ren,     
	Signal reset,     
	Signal empty,     
	Signal full,
	Signal wclk,
	Signal rclk
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
	Input (wclk);
	Input (rclk);

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
    Reg (empty_r);

    always (posedge (wclk))
	begin

		If (wen && !full)
		begin
			mem[waddr] = din;
			waddr++;
		end

		If (reset)
		begin
			waddr = 0;
		end

		full = (waddr + 1) == raddr;
		empty = waddr == raddr;
	end

    always (posedge (rclk))
	begin

		If (ren && !empty_r)
		begin
			raddrr = raddr;
			raddr++;
		end

		If (reset)
		begin
			raddr = 0;
		end
 	    empty_r = waddr == raddr;
	end
	  
	  

	assign dout = mem[raddrr];
endmodule
}
