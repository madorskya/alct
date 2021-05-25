#include "stage2.h"


void Stage2::operator()
(
	Signal hitc, Signal hita, Signal patb,
	Signal bestqc, Signal bestqa,
	Signal hitcr, Signal hitar, Signal patbr,
	Signal PromoteColl, 
	Signal best1r, Signal best1nr, Signal fa1r, 
	Signal clk
)
{
initio
	hitc.input (HITSBITS-1, 0, "hitc");
	hita.input (HITSBITS-1, 0, "hita"); 
	patb.input (LYWG-1,     0, "patb");
	bestqc.input(BEST2*QBITS-1, 0, "bestqc");
	bestqa.input(BEST2*QBITS-1, 0, "bestqa");
	hitcr.output(HITSBITS-1, 0, "hitcr", makereg); 
	hitar.output(HITSBITS-1, 0, "hitar", makereg); 
	patbr.output(LYWG-1,     0, "patbr", makereg);
	PromoteColl.input("PromoteColl");
	best1r.output(QBITS-1,   0, "best1r", makereg);
	best1nr.output(KEYBITS-1,        0, "best1nr", makereg);
	fa1r.output                ("fa1r", makereg);
	clk.input                 ("clk");

beginmodule

	bestc.wire(QBITS-1, 0, "bestc");
	besta.wire(QBITS-1, 0, "besta");
	best1.wire(QBITS-1, 0, "best1");
	bestcn.wire(KEYBITS-1, 0, "bestcn");
	bestan.wire(KEYBITS-1, 0, "bestan");
	best1n.wire(KEYBITS-1, 0, "best1n");
	fa1.wire("fa1");

	bstc.init("Bester", "bstc");
	bsta.init("Bester", "bsta");
	prom1.init("Promoter", "prom1");

	comment(" //find the best in collision and accelerator patterns")
    bstc (hitc, bestqc, bestc, bestcn);
    bsta (hita, bestqa, besta, bestan);
    
	comment(" //promote the best according to the PromoteColl bit")
    prom1 (bestc, bestcn, besta, bestan, PromoteColl, best1, best1n, fa1);

	comment(" //clock the outputs out")
	always (posedge (clk))
	begin
    	best1r   = best1;
    	best1nr  = best1n;
    	fa1r     = fa1;

    	hitar    = hita;
    	hitcr    = hitc;
    	patbr    = patb;
    end
endmodule
}

