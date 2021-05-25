
#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#include "patterner_rl.h"


class CheckNeighbors_rl : public module
{
public:
	Signal cnta, cntc;
	Signal cntan, cntcn;
	void operator()
	(
		Signal vc, Signal sc, Signal vcr, 
		Signal vctop, Signal sctop, 
		Signal vcbot, Signal scbot,

		Signal va, Signal sa, Signal varr, 
		Signal vatop, Signal satop, 
		Signal vabot, Signal sabot, 

		Signal trig_stop,
		Signal clk
	);

	Signal er;

};


class Stage1_rl: public module 
{
public:

	Patterner_rl pt[LYWG];
	CheckNeighbors_rl chn[LYWG];
	void operator()
	(
	    Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
		Signal collmask,
		Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
		Signal acc_pretrig, Signal acc_trig,
		Signal qca0, Signal qca1, Signal qca2, Signal qca3, 
		Signal qa0,  Signal qa1,  Signal qa2,  Signal qa3, 
		Signal trig_stop,
    	Signal clk
	);

	Signal hitca, hita, vaci, vai, vac, va;
#ifdef COLLPATB
	Signal hitcb, vbc;
#endif
};

