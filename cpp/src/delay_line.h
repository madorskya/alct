#ifndef __DELAY_LINE_H__
#define __DELAY_LINE_H__

#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class delay_line: public module
{
public:
	void operator()
	(
		Signal din,
		Signal dout,
		Signal delay,
		Signal we,
		Signal trig_stop,
		Signal clk
	);

	memory mem;
	Signal adw, adr, adrr;
	int bwd, bwad; // data and address width
    int ram_style; // 1-block or 0-distributed
};

#endif
