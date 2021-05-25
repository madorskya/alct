#include "trigger.h"


void trigger::operator()
(
	Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
	Signal collmask,                          
	Signal PromoteColl,                       
	Signal hp, Signal hnp, Signal hfap, Signal hpatbp,             
	Signal lp, Signal lnp, Signal lfap, Signal lpatbp,             
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
	Signal actv_feb_fg,
	Signal clk
)
{
initio
	ly0p.input(LYWG-1, 0, "ly0p"); 
	ly1p.input(LYWG-1, 0, "ly1p"); 
	ly2p.input(LYWG-1, 0, "ly2p"); 
	ly3p.input(LYWG-1, 0, "ly3p"); 
	ly4p.input(LYWG-1, 0, "ly4p"); 
	ly5p.input(LYWG-1, 0, "ly5p");
	collmask.input(COLLMASKBITS-1, 0, "collmask");
	PromoteColl.input("PromoteColl");

	hp.output(QBITS-1, 0, "hp", makereg);
	hnp.output(KEYBITS-1, 0, "hnp", makereg); 
	hfap.output("hfap", makereg); 
	hpatbp.output("hpatbp", makereg);

	lp.output(QBITS-1, 0, "lp", makereg); 
	lnp.output(KEYBITS-1, 0, "lnp", makereg); 
	lfap.output("lfap", makereg); 
	lpatbp.output("lpatbp", makereg);

	drifttime.input(2, 0, "drifttime");
	pretrig.input(2, 0, "pretrig");
	trig.input(2, 0, "trig");
	trig_mode.input(1, 0, "trig_mode");
	actv_feb_fg.output("actv_feb_fg", makereg);
	clk.input("clk");

beginmodule
    ly0i.reg(LYWG-1, 0, "ly0i"); 
	ly1i.reg(LYWG-1, 0, "ly1i"); 
	ly2i.reg(LYWG-1, 0, "ly2i"); 
	ly3i.reg(LYWG-1, 0, "ly3i"); 
	ly4i.reg(LYWG-1, 0, "ly4i"); 
	ly5i.reg(LYWG-1, 0, "ly5i");

    ly0.wire (LYWG-1, 0, "ly0"); 
	ly1.wire (LYWG-1, 0, "ly1"); 
	ly2.wire (LYWG-1, 0, "ly2"); 
	ly3.wire (LYWG-1, 0, "ly3"); 
	ly4.wire (LYWG-1, 0, "ly4"); 
	ly5.wire (LYWG-1, 0, "ly5");

	hitc.wire  (HITSBITS-1, 0, "hitc"); 
	hita.wire  (HITSBITS-1, 0, "hita"); 
	hitct.wire (HITSBITS-1, 0, "hitct"); 
	hitat.wire (HITSBITS-1, 0, "hitat"); 
	hitcr.wire (HITSBITS-1, 0, "hitcr"); 
	hitar.wire (HITSBITS-1, 0, "hitar"); 
	hitcrr.wire(HITSBITS-1, 0, "hitcrr"); 
	hitarr.wire(HITSBITS-1, 0, "hitarr"); 

	patb.wire   (LYWG-1, 0, "patb");
	patbt.wire  (LYWG-1, 0, "patbt");
	patbr.wire  (LYWG-1, 0, "patbr");
	patbrr.wire (LYWG-1, 0, "patbrr");
	patbrrr.wire(LYWG-1, 0, "patbrrr");

	fa1.wire  ("fa1"); 
	fa1r.wire ("fa1r");
	fa1rr.wire("fa1rr");
	fa2.wire  ("fa2");

	best1.wire   (QBITS-1, 0, "best1"); 
	best1r.wire  (QBITS-1, 0, "best1r"); 
	best1rr.wire (QBITS-1, 0, "best1rr"); 
	best2.wire   (QBITS-1, 0, "best2"); 

	best1n.wire  (KEYBITS-1, 0, "best1n"); 
	best1nr.wire (KEYBITS-1, 0, "best1nr"); 
	best1nrr.wire(KEYBITS-1, 0, "best1nrr"); 
	best2n.wire  (KEYBITS-1, 0, "best2n"); 

	bestqc1.wire (BEST2*QBITS-1, 0, "bestqc1");
	bestqa1.wire (BEST2*QBITS-1, 0, "bestqa1");
	bestqc2.wire (BEST2*QBITS-1, 0, "bestqc2");
	bestqa2.wire (BEST2*QBITS-1, 0, "bestqa2");
	

	ExtendPulses.init    ("Stage0",  "ExtendPulses");
	FindPatterns.init    ("Stage1",  "FindPatterns");
	KillNeighbours.init  ("Stage1a", "KillNeighbours");
	GetBestPattern.init  ("Stage2",  "GetBestPattern");
	MaskOutBest.init     ("Stage3",  "MaskOutBest");
	GetSecondPattern.init("Stage4",  "GetSecondPattern");


	ExtendPulses
	(
		ly0p, ly1p, ly2p, ly3p, ly4p, ly5p,
		ly0, ly1, ly2, ly3, ly4, ly5,
		clk
	);

	FindPatterns 
	(
		ly0, ly1, ly2, ly3, ly4, ly5, 
		collmask,
		hitc, hita, patb,
		drifttime, pretrig, trig, trig_mode,
		clk
	);

	KillNeighbours
	(
		hitc, hita, patb,
		hitct, hitat, patbt,
		bestqc1, bestqa1,
		clk
	);

	GetBestPattern
	(
		hitct, hitat, patbt,
		bestqc1, bestqa1,
		hitcr, hitar, patbr,
		PromoteColl, 
		best1, best1n, fa1, 
		clk
	);
	
	MaskOutBest
	(
		hitcr, hitar, patbr,
		hitcrr, hitarr, patbrr,
		best1, best1n, fa1, 
		best1r, best1nr, fa1r,
		bestqc2, bestqa2,
		clk
	);
	
	GetSecondPattern
	(
		hitcrr, hitarr, patbrr, patbrrr,
		PromoteColl, 
		best1r, best1nr, fa1r, 
		best1rr, best1nrr, fa1rr,
		best2, best2n, fa2,
		bestqc2, bestqa2,
		clk
	);
	
    always (posedge (clk))
    begin
        hp     = best1rr;
        hnp    = best1nrr;
        hfap   = fa1rr;
        hpatbp = patbrrr(best1nrr);
        
        lp     = best2;
        lnp    = best2n;
        lfap   = fa2;
        lpatbp = patbrrr(best2n);

		comment ("// if any pattern is detected, set this output. This is necessary for CFEBs")
		actv_feb_fg = ror(hitc) | ror(hita);
    end


endmodule             
}

