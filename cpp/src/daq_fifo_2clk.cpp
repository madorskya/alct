#include "daq_fifo_2clk.h"

// binary to gray code conversion for 8-bit values
#define BIN2GRAY(a) ((a) ^ (Signal(1,0), (a)(7,1)))

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
	Input (wen);   // wclk domain
	Input (ren);   // rclk domain
	Input (reset);
	OutReg (empty); // rclk domain
	OutReg (full);  // wclk domain
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
	Reg_(wgray , 7, 0) ; // gray-coded write pointer, wclk domain
	Reg_(rgray , 7, 0) ; // gray-coded read pointer, rclk domain
	Reg_(wgray_r1, 7, 0); // write pointer synchronizer, rclk domain
	Reg_(wgray_r2, 7, 0);
	Reg_(rgray_r1, 7, 0); // read pointer synchronizer, wclk domain
	Reg_(rgray_r2, 7, 0);
	Reg_ (waddr_n, 7, 0); // next write address, for full flag
	comment ("// synthesis attribute ASYNC_REG of wgray_r1 is TRUE")
	comment ("// synthesis attribute ASYNC_REG of wgray_r2 is TRUE")
	comment ("// synthesis attribute ASYNC_REG of rgray_r1 is TRUE")
	comment ("// synthesis attribute ASYNC_REG of rgray_r2 is TRUE")

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

		wgray = BIN2GRAY(waddr);
		waddr_n = waddr + 1;

		rgray_r2 = rgray_r1;
		rgray_r1 = rgray;

		full = BIN2GRAY(waddr_n) == rgray_r2;
	end

    always (posedge (rclk))
	begin

		If (ren && !empty)
		begin
			raddrr = raddr;
			raddr++;
		end

		If (reset)
		begin
			raddr = 0;
		end

		rgray = BIN2GRAY(raddr);

		wgray_r2 = wgray_r1;
		wgray_r1 = wgray;

 	    empty = wgray_r2 == rgray;
	end

	assign dout = mem[raddrr];
endmodule
}
