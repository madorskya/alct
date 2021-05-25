#include "bestquality.h"

void bestquality::operator()
(
	Signal hits,
	Signal bestq
)
{
initio
	hits.input(ninp*QBITS-1, 0, "hits");
	bestq.output(QBITS-1, 0, "bestq", makereg);
beginmodule
	qb0.reg(ninp-1, 0, "qb0");
	qb1.reg(ninp-1, 0, "qb1");
	i.reg(4, 0, "i");

	always (hits)
	begin
		comment(" //put the MSBs and LSBs separately")
		For (i = 0, i < ninp, i++) 
		begin
			qb0(i) = hits(i*QBITS);
			qb1(i) = hits(i*QBITS+1);
		end
		comment(" //find the best quality")
		bestq(1) = ror(qb1);
		bestq(0) = (!ror(qb1) && ror(qb0)) || (ror(qb1 & qb0));
	end

endmodule
}

