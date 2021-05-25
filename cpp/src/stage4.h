#include "alctver.h"
#include "vlib.h"
#include "vmac.h"
#include "bester.h"
#include "promoter.h"

class Stage4: public module
{

public:

	Signal best2;	
	Signal best2n;	
	Signal fa2;
    Signal bestc, besta; 
    Signal bestcn, bestan; 
    Bester bstc;
    Bester bsta;
    Promoter prom1;

	void operator()
	(
		Signal hitcrr, Signal hitarr, Signal patbrr, Signal patbrrr,
		Signal PromoteColl, 
		Signal best1r, Signal best1nr, Signal fa1r, 
		Signal best1rr, Signal best1nrr, Signal fa1rr, 
		Signal best2r, Signal best2nr, Signal fa2r, 
		Signal bestqc, Signal bestqa,
		Signal clk
	);
};


