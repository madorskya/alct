#include "vlib.h"
#include "vmac.h"
#include "dual_memory.h"

class al1a_maker:public module
{
 public:
	void operator()
	(
		Signal l1a_in,
		Signal track,
		Signal l1a_outp,
		Signal best_wnd,
		Signal raw_wnd,
		Signal best_full,
		Signal raw_full,
		Signal raw_we_en,
		Signal best_we,
		Signal raw_we,
		Signal l1a_int_en,
		Signal reset,
		Signal clk
	);

	Signal l1a_int,	l1ar, best_cnt,	raw_cnt, l1a_out, doutb;
	Signal track_valid, l1a_match_empty, l1a_match_emptyr;
	dual_memory l1a_match;

};


