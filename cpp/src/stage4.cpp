#include "stage4.h"


void Stage4::operator()
(
	Signal hitcrr, Signal hitarr, Signal patbrr, Signal patbrrr,
	Signal PromoteColl, 
	Signal best1r, Signal best1nr, Signal fa1r, 
	Signal best1rr, Signal best1nrr, Signal fa1rr, 
	Signal best2r, Signal best2nr, Signal fa2r, 
	Signal bestqc, Signal bestqa,
	Signal clk
)
{
initio
 
	hitcrr.input (HITSBITS-1,	0, "hitcrr");
	hitarr.input (HITSBITS-1,	0, "hitarr");
	patbrr.input (LYWG-1,		0, "patbrr");
	patbrrr.output (LYWG-1,		0, "patbrrr", makereg);

	PromoteColl.input (			   "PromoteColl");

	best1r.input (QBITS-1,		0, "best1r");
	best1nr.input (KEYBITS-1,	0, "best1nr");
	fa1r.input	(				   "fa1r");

	best1rr.output(QBITS-1,		0, "best1rr", makereg);
	best1nrr.output(KEYBITS-1,	0, "best1nrr",makereg);
	fa1rr.output (				   "fa1rr", makereg);

	best2r.output(QBITS-1,		0, "best2r", makereg);
	best2nr.output (KEYBITS-1,	0, "best2nr", makereg);
	fa2r.output	(				   "fa2r", makereg);

	bestqc.input(BEST2*QBITS-1, 0, "bestqc");
	bestqa.input(BEST2*QBITS-1, 0, "bestqa");

	clk.input	(				   "clk");

beginmodule
	best2.wire(QBITS-1, 0, "best2");
	best2n.wire(KEYBITS-1, 0, "best2n");
	fa2.wire("fa2");
    bestc.wire(QBITS-1, 0, "bestc");
	besta.wire(QBITS-1, 0, "besta");
    bestcn.wire(KEYBITS-1, 0, "bestcn");
	bestan.wire(KEYBITS-1, 0, "bestan");

	bstc.init("Bester", "bstc");
	bsta.init("Bester", "bsta");
	prom1.init("Promoter", "prom1");
	
    comment(" //find the best in collision and accelerator patterns")
    bstc (hitcrr, bestqc, bestc, bestcn);
    bsta (hitarr, bestqa, besta, bestan);
    
	comment(" //promote the best according to the PromoteColl bit")
    prom1 (bestc, bestcn, besta, bestan, PromoteColl, best2, best2n, fa2);

	comment(" //clock the outputs out")
	always (posedge (clk))
	begin
    	best2r	 = best2;
    	best2nr  = best2n;
    	fa2r	 = fa2;

    	best1rr	 = best1r;
    	best1nrr = best1nr;
    	fa1rr    = fa1r;
    	
    	patbrrr  = patbrr;
    end

endmodule
}

