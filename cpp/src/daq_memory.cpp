#include "daq_memory.h"

void daq_memory::operator()
(
	Signal adw,
	Signal adr,
	Signal adb,
	Signal dw,
	Signal dr,
	Signal we,
	Signal wblock,
	Signal full,
	Signal wclk,
	Signal rclk
)
{
initio

	Input_(adw, bwad-1, 0); // write address
	Input_(adr, bwad-1, 0); // read address
	Input_(adb, bwad-1, 0); // block address - cannot write past this address
	Input_(dw,  bwd-1, 0); // write data
	OutReg_(dr, bwd-1, 0); // read data
	Input (we); // write enable
	Input_(wblock, bwad-1, 0); // write block length
	Output (full); // full - cannot write
	Input (wclk);
	Input (rclk);

beginmodule
	Reg_(adrr,bwad-1, 0); // registered read address - need that so XST can infer block memory
	Reg__(mem, bwd-1, 0, (1<<bwad)-1, 0); 
	comment ("// synthesis attribute ram_style of mem is block")

 	Wire_ (diff, bwad-1, 0);
 	assign diff = adb - adw; 
    assign full = !((diff > (wblock + 10)) || (adb == adw));	

    always (posedge (wclk))
    begin
        If (we)	mem[adw] = dw;
		adrr = adr;
    end

    always (posedge (rclk))
	    dr = mem[adrr];

#ifdef _VDEBUG
	if (posedge(wclk).getbool() && (adrr == adw).getbool())
	{
		cerr << "daq_memory: Possible problem: simultaneously reading and writing the same memory address: " << adw << "\n";
	}
#endif

endmodule
}
