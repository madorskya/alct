#include "trigger_rl.h"


void trigger_rl::operator()
(
	Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
	Signal collmask,                          
	Signal PromoteColl,                       
	Signal hp, Signal hnp, Signal hfap, Signal hpatbp, Signal hv,
	Signal lp, Signal lnp, Signal lfap, Signal lpatbp, Signal lv,            
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
	Signal acc_pretrig, Signal acc_trig,
	Signal actv_feb_fg,
	Signal trig_stop,
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

	Output_(hp, QBITS-1, 0); 
	Output_(hnp, KEYBITS-1, 0); 
	Output (hfap); 
	Output (hpatbp);
	Output (hv);

	Output_(lp, QBITS-1, 0); 
	Output_(lnp, KEYBITS-1, 0); 
	Output (lfap); 
	Output (lpatbp);
	Output (lv);

	Input_(drifttime, 2, 0);
	Input_(pretrig, 2, 0);
	Input_(trig, 2, 0);
    Input_(trig_mode, 1, 0);
	Input_(acc_pretrig, 2, 0);
	Input_(acc_trig, 2, 0);
	OutReg (actv_feb_fg);
	Input (trig_stop);
	Input (clk);

beginmodule

    Wire_(ly0, LYWG-1, 0); 
	Wire_(ly1, LYWG-1, 0); 
	Wire_(ly2, LYWG-1, 0); 
	Wire_(ly3, LYWG-1, 0); 
	Wire_(ly4, LYWG-1, 0); 
	Wire_(ly5, LYWG-1, 0);

   	Wire_(qca0, LYWG-1, 0) ;
   	Wire_(qca1, LYWG-1, 0) ;
   	Wire_(qca2, LYWG-1, 0) ;
   	Wire_(qca3, LYWG-1, 0) ;
   	Wire_(qa0 , LYWG-1, 0) ;
   	Wire_(qa1 , LYWG-1, 0) ;
   	Wire_(qa2 , LYWG-1, 0) ;
   	Wire_(qa3 , LYWG-1, 0) ;
		  	   				   
   	Wire_(bwc1w, KEYBITS-1, 0) ;
   	Wire_(bwc2w, KEYBITS-1, 0) ;
   	Wire_(bqc1w, QBITS-1, 0) ;
   	Wire_(bqc2w, QBITS-1, 0) ;
	Wire (bvc1w);
	Wire (bvc2w);

   	Wire_(bwa1w, KEYBITS-1, 0) ;
   	Wire_(bwa2w, KEYBITS-1, 0) ;
   	Wire_(bqa1w, QBITS-1, 0) ;
   	Wire_(bqa2w, QBITS-1, 0) ;
	Wire (bva1w);
	Wire (bva2w);

   	Wire_(bw1, KEYBITS-1, 0); // unregistered outputs, to be used in output multiplexor
   	Wire_(bq1, QBITS-1, 0);
   	Wire (fa1);
	Wire (bv1);
   	Wire_(bw2, KEYBITS-1, 0);
   	Wire_(bq2, QBITS-1, 0);
   	Wire (fa2);
	Wire (bv2);


	ExtendPulses.init ("Stage0",  "ExtendPulses");
	FindPatterns.init ("Stage1_rl", "FindPatterns");
	cc.init ("collider", "cc");
	ca.init ("collider", "ca");
	pr.init ("promoter_rl", "pr");


	ExtendPulses
	(
		ly0p, ly1p, ly2p, ly3p, ly4p, ly5p,
		ly0, ly1, ly2, ly3, ly4, ly5,
		trig_stop,
		clk
	);

	FindPatterns 
	(
		ly0, ly1, ly2, ly3, ly4, ly5, 
		collmask, drifttime, pretrig, trig, trig_mode, 
		acc_pretrig, acc_trig,
		qca0, qca1, qca2, qca3, 
		qa0,  qa1,  qa2,  qa3, 	
		trig_stop,
		clk
	);

	cc (qca0, qca1, qca2, qca3, bwc1w, bwc2w, bqc1w, bqc2w, bvc1w, bvc2w, clk);			 
	ca (qa0,  qa1,  qa2,  qa3,  bwa1w, bwa2w, bqa1w, bqa2w, bva1w, bva2w, clk);

	pr 
	(
		bwc1w, bqc1w, bvc1w,
		bwc2w, bqc2w, bvc2w,

		bwa1w, bqa1w, bva1w,
		bwa2w, bqa2w, bva2w,

		bw1, bq1, fa1, bv1,
		bw2, bq2, fa2, bv2,

		PromoteColl,
		clk
	);

	assign	hv	   = bv1;
    assign  hp     = bq1;
    assign  hnp    = bw1;
    assign  hfap   = fa1;
    assign  hpatbp = 0;
        
	assign	lv	   = bv2;
    assign  lp     = bq2;
    assign  lnp    = bw2;
    assign  lfap   = fa2;
    assign  lpatbp = 0;

	always (posedge (clk))
	begin

		comment ("// if any pattern is detected, set this output. This is necessary for CFEBs")
		actv_feb_fg = ror(qca0) | ror(qca1) | ror(qca2) | ror(qca3) | ror(qa0) | ror(qa1) | ror(qa2) | ror(qa3);

    end


endmodule             
}

