#include "alctver.h"
#include "vlib.h"
#include "stage0.h"
#include "stage1_rl.h"
#include "collider.h"
#include "promoter_rl.h"
#include "shower.h"

class trigger_rl: public module
{
public:
   
    Signal ly0, ly1, ly2, ly3, ly4, ly5;
    Signal ly0m, ly1m, ly2m, ly3m, ly4m, ly5m;

	Signal qca0, qca1, qca2, qca3, qa0, qa1, qa2, qa3;	   
	Signal bwc1w, bwc2w, bqc1w, bqc2w, bwa1w, bwa2w, bqa1w, bqa2w;
	Signal bvc1w, bvc2w, bva1w, bva2w;


	Stage0  ExtendPulses;
	Stage1_rl FindPatterns;
	collider cc, ca;
	promoter_rl pr;
        shower shower_detector;

	void operator()
	(
		Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
		Signal collmask,                          
		Signal PromoteColl,                       
		Signal hp, Signal hnp, Signal hfap, Signal hpatbp, Signal hv,
		Signal lp, Signal lnp, Signal lfap, Signal lpatbp, Signal lv,  
                Signal shower_int, Signal shower_oot,
		Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
		Signal acc_pretrig, Signal acc_trig,
		Signal actv_feb_fg,
		Signal trig_stop,
		
		Signal input_disr,
		Signal ext_trig_en,
		Signal ext_trig2,
		Signal inject,
		Signal ext_inject2,
		Signal HCmask,

		Signal clk
	);

   	Signal bw1, bq1, fa1, bw2, bq2, fa2, bv1, bv2;

};


