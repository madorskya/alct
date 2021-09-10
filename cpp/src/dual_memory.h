#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class dual_memory: public module
{
 public:
	void operator()
	(
		Signal ada,
		Signal adb,
		Signal dina,
		Signal dinb,
		Signal douta,
		Signal doutb,
		Signal wea,
		Signal web,
		Signal clka,
		Signal clkb
	);

	memory mem;
	Signal adar, adbr;
	int bwad, bwd;
};
