#include "alctver.h"
#include "vlib.h"
#include "vmac.h"
#include "bestquality.h"


class CheckNeighbors : public module
{
public:
	Signal cnta, cntc;
	Signal cntan, cntcn;
	void operator()
	(
		Signal sc, Signal scr, Signal sctop, Signal scbot,
		Signal sa, Signal sar, Signal satop, Signal sabot, 
		Signal clk
	);
};

class Stage1a : public module
{
public:
	CheckNeighbors chn[LYWG];
	bestquality bqa[BEST2], bqc[BEST2];
	Signal hitci, hitai;
	Signal bestqc, bestqa;

	void operator()
	(
		Signal hitc, Signal hita, Signal patb,
		Signal hitcr, Signal hitar, Signal patbr,
		Signal bestqcr, Signal bestqar, 
		Signal clk
	);
};




