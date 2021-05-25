#include "alctver.h"
#include "vlib.h"
#include "vmac.h"
#include "bester.h"
#include "promoter.h"

class Stage2: public module
{
public:

	Signal bestc, besta, best1;
	Signal bestcn, bestan, best1n;
    Signal fa1;
    Bester bstc;
    Bester bsta;
    Promoter prom1;

	void operator()
	(
		Signal hitc, Signal hita, Signal patb,
		Signal bestqc, Signal bestqa,
		Signal hitcr, Signal hitar, Signal patbr,
		Signal PromoteColl, 
		Signal best1r, Signal best1nr, Signal fa1r, 
		Signal clk
	);
};



