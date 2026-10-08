#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

// dual-clock FIFO with gray-coded pointer synchronization
// full  is in wclk domain
// empty is in rclk domain
class daq_fifo_2clk:public module
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
		Signal wclk,
		Signal rclk
	);

    Signal waddr, raddr, raddrr;
	Signal wgray, rgray, wgray_r1, wgray_r2, rgray_r1, rgray_r2, waddr_n;
	memory mem;
	int bwd, bwad; // data and address bit width
	int distributed; // if 1 use distributed memory

	daq_fifo_2clk(){distributed = 0;}; // block by default
};
