#include "hit_counters.h"



void hit_counters::operator()
(
	Signal best1, // {valid, wgn}
	Signal best2, 
 
	Signal counters, 

	Signal clk
)
{

	char hcbnd[] = HCBND; // hit counter boundaries are different for each type of ALCT
	int lwg, hwg;
	int j;

initio

	Input_(best1, BESTBITS-1, 0);
	Input_(best2, BESTBITS-1, 0);

	OutReg_(counters, BWHC*HCNUM-1, 0); // all counters as one register

	Input (clk);

beginmodule

	Reg_(tcnt, BWHC-1, 0); // time counter
	Reg__(cnt, BWHC-1, 0, HCNUM-1, 0); // hit counters
	Reg_(best1r, BESTBITS-1, 0);
	Reg_(best2r, BESTBITS-1, 0);

	always (posedge (clk))
	begin


		If (tcnt == COUNT_TIME) 
		begin
			tcnt = 0;

			for (j = 0; j < HCNUM; j++)
			{
				counters((j+1)*BWHC-1, j*BWHC) = cnt[j];
				cnt[j] = 0;
			}

		end
		Else
		begin

			for (j = 0; j < HCNUM; j++)
			{
				lwg = (j == 0) ? 0 : hcbnd[j-1];
				hwg = hcbnd[j];

				If 
				(
					(
						best1r(KEYBITS) && 
						best1r(KEYBITS-1, 0) >= lwg &&
						best1r(KEYBITS-1, 0) <  hwg
					) ||
					(
						best2r(KEYBITS) && 
						best2r(KEYBITS-1, 0) >= lwg &&
						best2r(KEYBITS-1, 0) <  hwg
					)
				) cnt[j]++;
			}

			tcnt++;
		end

		best1r = best1;
		best2r = best2;
	end

endmodule
}
