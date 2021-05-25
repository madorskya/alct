#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class crc: public module
{
public:
	void operator()
	(		
		Signal d,
		Signal crc,
		Signal calc,
		Signal dav,
		Signal clk
	);

	Signal ncrc, i, t;
};

