#include "dual_memory.h"

// must be read-before-write operation, so we can read the L1A time marker and 
// simultaneously write zero for it
void dual_memory::operator()
(
	Signal ada,
	Signal adb,
	Signal dina,
	Signal dinb,
	Signal douta,
	Signal doutb,
	Signal wea,
	Signal web,
	Signal clka,
	Signal clkb
)
{
initio

	Input_(ada, bwad-1, 0); // address A
	Input_(adb, bwad-1, 0); // address B
	Input_(dina,  bwd-1, 0); // write data A
	Input_(dinb,  bwd-1, 0); // write data B
	OutReg_(douta, bwd-1, 0); // read data A
	OutReg_(doutb, bwd-1, 0); // read data B
	Input (wea); // write enable A
	Input (web); // write enable B
	Input (clka);
	Input (clkb);

beginmodule
	Reg__(mem, bwd-1, 0, (1<<bwad)-1, 0); 
	comment ("// synthesis attribute ram_style of mem is block")

    always (posedge (clka))
    begin
		douta = mem[ada]; // read before write
        If (wea) mem[ada] = dina; 
	end

	always (posedge (clkb)) // need two always blocks for xst to infer memory block
    begin
		doutb = mem[adb];
        If (web) mem[adb] = dinb;
    end


#ifdef _VDEBUG
	if (posedge(clka).getbool() && (ada == adb).getbool())
	{
		cerr << "dual_memory: Possible problem: simultaneously accessing the same memory address: " << ada << "\n";
	}
#endif

endmodule
}
