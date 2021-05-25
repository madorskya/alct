#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class RawHitMemory: public module
{
public:

	void operator()
	(
		Signal addra,
		Signal addrb,
		Signal addrblock,
		Signal fifo_tbins,
		Signal canwrite,
		Signal clka,
		Signal clkb,
		Signal dina,
		Signal doutb,
		Signal wea
	);

	memory mem;
};


