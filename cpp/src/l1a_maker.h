#include "vlib.h"
#include "vmac.h"
#include "delay_line.h"

class l1a_maker:public module
{
 public:
	void operator()
	(
		Signal l1a_in,
		Signal valor,
		Signal track,
		Signal l1a_outp,
		Signal best_wnd,
		Signal raw_wnd,
		Signal l1a_fifo_full,
		Signal best_full,
		Signal raw_full,
		Signal raw_we_en,
		Signal best_we,
		Signal raw_we,
		Signal l1a_int_en,
		Signal l1a_in_count,
		Signal l1a_offset,
		Signal send_empty,
		Signal reset,
		Signal clk
	);

	Signal l1a_int,	l1ar, best_cnt,	raw_cnt, l1a_out;
	delay_line l1a_delay;


};


