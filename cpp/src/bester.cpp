#include "bester.h"


void bestof::operator()
(
	Signal hits,
	Signal bestq,
	Signal bestn
)
{
initio
	hits.input(ninp*QBITS-1, 0, "hits");
	bestq.input(QBITS-1, 0, "bestq");
	bestn.output((ninp <= 8) ? 2 : 3, 0, "bestn", makereg);
beginmodule

	always (hits or bestq)
	begin
		comment(" //find which of the hits is the best")
		bestn = 0;
		for (int j = 0; j < ninp; j++)
		{
			If (bestq == hits(QBITS*j+QBITS-1, QBITS*j)) bestn = j;
		}
	end
endmodule
}


void Bester::operator()
(
	Signal hits,
	Signal bestq,
	Signal max,
	Signal maxn
)
{
initio
	hits.input(HITSBITS-1, 0, "hits");
	bestq.input(BEST2*QBITS-1, 0, "bestq");
	max.output(QBITS-1, 0, "max");
	maxn.output(KEYBITS-1, 0, "maxn", makereg);
beginmodule

	int i;
	for (i = 0; i < BEST2; i++)	bestn[i].wire(BEST1BITS-1, 0, "bestn", i);
	bestq.wire(BEST2*QBITS-1, 0, "bestq");
	maxnr.wire(BEST2BITS-1, 0, "maxnr");


	comment(" //find the best tracks in groups of 8 WGs")
	for (i = 0; i < BEST2; i++)
	{
		bo[i].init("bestof", "bo", i);
		bo[i].ninp = BEST1;
		bo[i]
		(
			hits (i*BEST1*QBITS+BEST1*QBITS-1, i*BEST1*QBITS), 
			bestq(i*QBITS+QBITS-1, i*QBITS),
			bestn[i]
		);
	}

	bqm.init("bestqualitymax", "bqm");
	bqm.ninp = BEST2;
	bqm
	(
		bestq,
		max
	);

	comment(" //find the best out of the best")
	bom.init("bestofmax", "bom");
	bom.ninp = BEST2;
	bom
	(
		bestq,
		max,
		maxnr
	);


	always (maxnr or allbestn)
	begin
		maxn = 0;
		for (i = 0; i < BEST2; i++)
		{
			If (maxnr == i) maxn = (maxnr, bestn[i]);
		}
	end
	
endmodule
}

