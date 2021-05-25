#include "alctver.h"
#include "vlib.h"
#include "vmac.h"
#include "bestquality.h"

class maskout: public module
{
public:

	Signal i;

	void operator()
	(
		Signal hitcr, Signal hitar, 
		Signal hitcrr, Signal hitarr,
		Signal best1n, Signal fa1
	);
};


class Stage3: public module
{
public:

	Signal hitci, hitai, bestqc, bestqa;
	maskout mo;
	bestquality bqa[BEST2], bqc[BEST2];

	void operator()
	(
		Signal hitcr, Signal hitar, Signal patbr,
		Signal hitcrr, Signal hitarr, Signal patbrr,
		Signal best1, Signal best1n, Signal fa1, 
		Signal best1r, Signal best1nr, Signal fa1r,
		Signal bestqcr, Signal bestqar, 
		Signal clk
	);
};

