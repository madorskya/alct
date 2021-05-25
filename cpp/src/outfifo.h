#include "alctver.h"
#include "vlib.h"
#include "vmac.h"


class outfifo: public module
{
public:
	void operator()
	(		
		Signal din,
		Signal wren,
		Signal wrclk,
		Signal rden,
		Signal rdclk,
		Signal dout,
		Signal empty,
		Signal ext_trig_en,
		Signal NoSpaceForDAQ
	);
	Signal MoreThanHalf;
	Signal daqMsb;
	Signal hvalid;

	Signal waddr;
	Signal raddr;
	memory mem;
};
