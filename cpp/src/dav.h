#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class dav: public module
{
public:
	void operator()
	(
		Signal l1a,
		Signal l1a_num,
		Signal valid,
		Signal l1a_window,
		Signal davalid,
		Signal clk
	);


	memory l1a_sh;
	Signal davmem, i, l1ad, l1a_numd, l1ar;

};
