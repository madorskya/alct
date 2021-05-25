#ifndef __DELAY_LINE_VALOR_H__
#define __DELAY_LINE_VALOR_H__

#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class delay_line_valor: public module
{
public:
	void operator()
	(
		Signal din,
		Signal dout,
		Signal delay,
		Signal we,
		Signal l1a,
		Signal l1a_window,
		Signal valid,
		Signal valorr,
		Signal trig_stop,
		Signal clk
	);

	memory mem;
	Signal val, advr, valout, val_sh, valor;
	Signal adw, adr, adrr;
	int bwd, bwad; // data and address width
    int ram_style; // 1-block or 0-distributed
	int max_l1a_window; // maximum possible l1a window, should be 16
};

#endif
