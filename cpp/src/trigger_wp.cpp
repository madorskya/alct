#include "trigger_wp.h"


void trigger_wp::operator()
(
	Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
	Signal collmask,                          
	Signal PromoteColl,                       
	Signal hp, Signal hnp, Signal hfap, Signal hpatbp, Signal hv,
	Signal lp, Signal lnp, Signal lfap, Signal lpatbp, Signal lv,            
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
	Signal acc_pretrig, Signal acc_trig,
	Signal actv_feb_fg,
	Signal clk
)
{
initio
	Input_(ly0p, LYWG-1, 0); 
	Input_(ly1p, LYWG-1, 0); 
	Input_(ly2p, LYWG-1, 0); 
	Input_(ly3p, LYWG-1, 0); 
	Input_(ly4p, LYWG-1, 0); 
	Input_(ly5p, LYWG-1, 0);
	Input_(collmask, COLLMASKBITS-1, 0);
	Input (PromoteColl);

	OutReg_(hp, QBITS-1, 0); 
	OutReg_(hnp, KEYBITS-1, 0); 
	OutReg (hfap); 
	OutReg (hpatbp);
	OutReg (hv);

	OutReg_(lp, QBITS-1, 0); 
	OutReg_(lnp, KEYBITS-1, 0); 
	OutReg (lfap); 
	OutReg (lpatbp);
	OutReg (lv);

	Input_(drifttime, 2, 0);
	Input_(pretrig, 2, 0);
	Input_(trig, 2, 0);
    Input_(trig_mode, 1, 0);
	Input_(acc_pretrig, 2, 0);
	Input_(acc_trig, 2, 0);
	OutReg (actv_feb_fg);
	Input (clk);

beginmodule

    Wire_(ly0, LYWG-1, 0); 
	Wire_(ly1, LYWG-1, 0); 
	Wire_(ly2, LYWG-1, 0); 
	Wire_(ly3, LYWG-1, 0); 
	Wire_(ly4, LYWG-1, 0); 
	Wire_(ly5, LYWG-1, 0);

   	Wire (bv1); // unregistered outputs, to be used in output multiplexor
   	Wire_(bq1, QBITS-1, 0);
   	Wire (bv2);
   	Wire_(bq2, QBITS-1, 0);

   	Reg (bv1r); 
   	Reg_(bq1r, QBITS-1, 0);
   	Reg (bv2r);
   	Reg_(bq2r, QBITS-1, 0);

	ExtendPulses.init ("Stage0",  "ExtendPulses");
	patterner.init("Patterner_rl", "patterner");

	ExtendPulses
	(
		ly0p, ly1p, ly2p, ly3p, ly4p, ly5p,
		ly0, ly1, ly2, ly3, ly4, ly5,
		clk
	);

	patterner
	(
		(ror(ly0), "2'b0"),
		(ror(ly1), "1'b0"),
		ror(ly2),
		((Signal)"1'b0", ror(ly3)),
		((Signal)"2'b0", ror(ly4)),
		((Signal)"2'b0", ror(ly5)),

		"28'hfffffff",

		bq1, bv1, 
		bq2, bv2,
	
		drifttime, pretrig, trig, 
		acc_pretrig, acc_trig,
		trig_mode,
		clk
	);



	always (posedge (clk))
	begin

		hv	   = bv1r;
	    hp     = bq1r;
	    hnp    = 0;
	    hfap   = 0;
	    hpatbp = 0;
    
		lv	   = bv2r;
	    lp     = bq2r;
	    lnp    = 0;
	    lfap   = 1;
	    lpatbp = 0;

		bv1r = bv1;
		bq1r = ifelse(bv1, bq1, "2'b0");
		bv2r = bv2;
		bq2r = ifelse(bv2, bq2, "2'b0");

		comment ("// if any pattern is detected, set this output. This is necessary for CFEBs")
		actv_feb_fg = bv1 | bv2;

    end


endmodule             
}

