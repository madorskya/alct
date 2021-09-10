#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class dav: public module
{
public:
	void operator()
	(
		Signal l1a,
		Signal davalid,
		Signal clk
	);


	Signal l1a_sh;

};
