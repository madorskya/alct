

#include "dav.h"
#define DAVDEL 15 // dav bit delay

void dav::operator()
(
	Signal l1a,
	Signal l1a_num,
	Signal valid,
	Signal l1a_window,
	Signal davalid,
	Signal clk
)
{
initio

	Input (l1a);
	Input_(l1a_num, 7, 0);
	Input (valid);
	Input (l1a_window);
	OutReg (davalid);
	Input (clk);

beginmodule

	Reg_(davmem, 255, 0); // this register shows if L1A with certain number has valid tracks
	Reg__(l1a_sh, 8, 0, DAVDEL-1, 0); // delays L1A number and L1A until all tracks are collected in davmem
	Reg (l1ad);
	Reg_(l1a_numd, 7, 0);
	Reg_(i, 4, 0);
	Reg (l1ar);
	

	always (posedge (clk))
	begin
		
		// valid comes during l1a_window - remember it for current l1a_num
		If (l1a_window && valid)
			davmem(l1a_num) = "1'b1";

		// pick up delayed l1a and l1a_num from tail of delay line
		(l1ad, l1a_numd) = l1a_sh[0];

		davalid = davmem(l1a_numd) && l1ad; // if there were tracks received for this l1a_num - set davalid
		If (l1ad) davmem(l1a_numd) = "1'b0"; // remove that bit from register, to avoid using it again

		// shift the l1a delay line
		For (i = 0, i < DAVDEL-1, i++)
		begin
			l1a_sh[i] = l1a_sh[i+1];
		end
		// put l1a and l1a_num into the head of the delay line
		l1a_sh[DAVDEL-1] = (l1ar, l1a_num);

		l1ar = l1a; // delay l1a by one clock so it matches its number

	end

endmodule
}
