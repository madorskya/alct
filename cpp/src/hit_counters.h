#include "vlib.h"
#include "vmac.h"
#include "alctver.h"

#define COUNT_TIME "32'd400_800_000" // 10 sec at 40.08 MHz

class hit_counters: public module
{
 public:
	void operator()
    (
		Signal best1, // {valid, wgn}
		Signal best2, 
 
		Signal counters, 

		Signal clk
	);

	Signal tcnt, best1r, best2r;
	memory cnt;

};
