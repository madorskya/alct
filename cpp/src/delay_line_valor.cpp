#include "delay_line_valor.h"

void delay_line_valor::operator()
(
	Signal din,
	Signal dout,
	Signal delay,
	Signal we,
    Signal l1a,
	Signal l1a_window,
	Signal valid,
	Signal valorr,
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
	Input (l1a);
	Input_(l1a_window, 3, 0);
	Input 		(valid);
	OutReg (valorr);
	Input (trig_stop);
	Input (clk);

beginmodule

	Reg__(mem, bwd-1, 0, (1<<bwad)-1, 0); 
	string mem_attr = "// synthesis attribute ram_style of mem is ";
	if (ram_style == 0) mem_attr += "distributed";
	else mem_attr += "block";
 	comment(mem_attr)

	// separate 1-bit memory for valid signal
	Reg_ (val, 255, 0);
	comment("// synthesis attribute ram_style of val is distributed")

	Reg_(adw, bwad-1, 0);
	Reg_(adr, bwad-1, 0);
	if (ram_style == 1)	Reg_(adrr, bwad-1, 0);
    Reg_ (advr, 7, 0);
	Reg (valout);
	Reg_ (val_sh, max_l1a_window-1, 0);

#ifdef VGEN
	printv("initial val = 0;\n");
	printv("initial adw = 0;\n");
	printv("initial adr = 0;\n");
#endif

	Reg(valor);

	always (posedge (clk)) 
	begin 

		If (trig_stop)
		begin
			val = "255'h0";
			adw = 0;
			adr = adw - delay + 1;
		end
		Else
		begin

			valorr = valor;

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

		    valout = val(advr);
	    	If (we) val(adw) = valid;
			advr = (adw - delay + max_l1a_window) + 1 + 1;

			adw++;
			begincase (l1a_window)
				case1(1 ) begin valor = val_sh(0); end
				case1(2 ) begin valor = ror(val_sh(1 ,0)); end
				case1(3 ) begin valor = ror(val_sh(2 ,0)); end
				case1(4 ) begin valor = ror(val_sh(3 ,0)); end
				case1(5 ) begin valor = ror(val_sh(4 ,0)); end
				case1(6 ) begin valor = ror(val_sh(5 ,0)); end
				case1(7 ) begin valor = ror(val_sh(6 ,0)); end
				case1(8 ) begin valor = ror(val_sh(7 ,0)); end
				case1(9 ) begin valor = ror(val_sh(8 ,0)); end
				case1(10) begin valor = ror(val_sh(9 ,0)); end
				case1(11) begin valor = ror(val_sh(10,0)); end
				case1(12) begin valor = ror(val_sh(11,0)); end
				case1(13) begin valor = ror(val_sh(12,0)); end
				case1(14) begin valor = ror(val_sh(13,0)); end
				case1(15) begin valor = ror(val_sh(14,0)); end
				Default   begin valor = ror(val_sh(9 ,0)); end
			endcase								   

			// shift register for valid bit for max_l1a_window
			val_sh = (valout, val_sh(max_l1a_window-1,1));
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
