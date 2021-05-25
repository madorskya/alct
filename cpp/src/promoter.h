#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#ifndef _PROMOTER_H_
#define _PROMOTER_H_

class Promoter: public module
{
public:
	Signal mode;
	
	void operator()
	(
		Signal bestc, 
		Signal bestcn, 
		Signal besta, 
		Signal bestan, 
		Signal PromoteColl, 
		Signal best, 
		Signal bestn, 
		Signal fa		
	);

};

#endif
