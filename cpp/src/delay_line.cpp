#include "delay_line.h"

void delay_line::operator()
(
	Signal din,
	Signal dout,
	Signal delay,
	Signal we,
	Signal trig_stop,
	Signal clk
)
{
initio
	Input_(din, bwd-1, 0);
 	if (ram_style == 1) Output_(dout, bwd-1, 0);
	else                OutReg_(dout, bwd-1, 0);
	Input_(delay, bwad-1, 0);
	Input (we);
	Input (trig_stop);
	Input (clk);

beginmodule

	Reg__(mem, bwd-1, 0, (1<<bwad)-1, 0); 
	string mem_attr = "// synthesis attribute ram_style of mem is ";
	if (ram_style == 0) mem_attr += "distributed";
	else mem_attr += "block";
 	comment(mem_attr)

	Reg_(adw, bwad-1, 0);
	Reg_(adr, bwad-1, 0);
	if (ram_style == 1)	Reg_(adrr, bwad-1, 0);

	always (posedge (clk)) 
	begin 
		If (trig_stop)
		begin
			adw = 0;
			adr = adw - delay + 1;
		end
		Else
		begin
			if (ram_style == 0) // distributed
			{
				dout = mem[adr]; 
				If (we) mem[adw] = din;
				adr = adw - delay + 1;
 			}
			else // block
			{
		        If (we) mem[adw] = din;
				adrr = adr;
				adr = adw - delay + 1;
			}
			adw++;
		end
	end 

	if (ram_style == 1) 
	{
		assign dout = mem[adrr];
	}

#ifdef _VDEBUG
	if (posedge(clk).getbool() && (adw == adr).getbool())
	{
		cerr << "delay_line: Possible problem: simultaneously reading and writing the same memory address: " << adw << "\n";
	}
#endif

endmodule
}
