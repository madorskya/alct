

#include "dav06.h"
#if (defined(ALCT576) && defined(MIRROR)) || (defined(ALCT672) && !defined(MIRROR))
	#define SHRL 9 // on Misha's request, email 2019-09-29
#else
	#define SHRL 11 
#endif

void dav::operator()
(
	Signal l1a,
	Signal davalid,
	Signal clk
)
{
initio

	Input (l1a);
	OutReg (davalid);
	Input (clk);

beginmodule

	Reg_(l1a_sh, SHRL-1, 0);

	always (posedge (clk))
	begin
		
		davalid = l1a_sh(0);

		l1a_sh = (l1a,   l1a_sh(SHRL-1,1));		
	end

endmodule
}
