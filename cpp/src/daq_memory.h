#ifndef __DAQ_MEMORY_H__
#define __DAQ_MEMORY_H__

#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

// single-clock DAQ memory, used by legacy (copper) DAQ
class daq_memory: public module
{
 public:
	void operator()
	(
		Signal adw,
		Signal adr,
		Signal adb,
		Signal dw,
		Signal dr,
		Signal we,
		Signal wblock,
		Signal full,
		Signal clk
	);

	memory mem;
	Signal adrr, diff;
	int bwad, bwd;
};

#endif
