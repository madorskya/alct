#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class daq_fifo:public module
{
public:
	void operator()
	(
		Signal din,       
		Signal dout,      
		Signal wen,     
		Signal ren,     
		Signal reset,     
		Signal empty,     
		Signal full,
		Signal clk      
	);

	Signal waddr, raddr, raddrr;
	memory mem;
	int bwd, bwad; // data and address bit width
	int distributed; // if 1 use distributed memory

	daq_fifo(){distributed = 0;}; // block by default
};
