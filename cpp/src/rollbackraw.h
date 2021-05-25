#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class RollBackRaw: public module
{
public:
	void operator()
	(
		Signal DIN,
		Signal DOUT,
		Signal RollBack,
		Signal we,
		Signal CLK
	);

	memory mem;
	Signal ADDRA, ADDRB;
	int dwidth; // data width
};

