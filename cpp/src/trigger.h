#include "alctver.h"
#include "vlib.h"
#include "stage0.h"
#include "stage1.h"
#include "stage1a.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"

class trigger: public module
{
public:
    Signal  ly0i, ly1i, ly2i, ly3i, ly4i, ly5i;
    Signal ly0, ly1, ly2, ly3, ly4, ly5;
	Signal hitc, hita, patb,
		 hitct, hitat, patbt,
		 hitcr, hitar, patbr,
		 best1, best1n, fa1, 
		 hitcrr, hitarr, patbrr,
		 best1r, best1nr, fa1r,
		 best1rr, best1nrr, fa1rr,
		 best2, best2n, fa2, patbrrr;

	Signal bestqc1,	bestqa1, bestqc2, bestqa2;

	Stage0  ExtendPulses;
	Stage1  FindPatterns;
	Stage1a KillNeighbours;
	Stage2  GetBestPattern;
	Stage3  MaskOutBest;
	Stage4  GetSecondPattern;

	void operator()
	(
		Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
		Signal collmask,                          
		Signal PromoteColl,                       
		Signal hp, Signal hnp, Signal hfap, Signal hpatbp,             
		Signal lp, Signal lnp, Signal lfap, Signal lpatbp,             
		Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
		Signal actv_feb_fg,
		Signal clk
	);
};


