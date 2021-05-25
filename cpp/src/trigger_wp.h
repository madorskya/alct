#include "alctver.h"
#include "vlib.h"
#include "stage0.h"
#include "patterner_rl.h"

class trigger_wp: public module
{
public:
   
    Signal ly0, ly1, ly2, ly3, ly4, ly5;

	Stage0  ExtendPulses;
	Patterner_rl patterner;

	void operator()
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
	);

   	Signal bq1, bq2, bv1, bv2;
   	Signal bq1r, bq2r, bv1r, bv2r;

};


