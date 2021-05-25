#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#ifndef _BESTQUALITY_H_
#define _BESTQUALITY_H_

class bestquality: public module
{
public:
	Signal qb0, qb1, i;
	int ninp; // parameter - number of inputs
	void operator()
	(
		Signal hits,
		Signal bestq
	);
};

#endif
