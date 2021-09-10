#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class bestfifo: public module
{
public:
	void operator()
	(
		Signal clk,       
		Signal sinit,     
		Signal din,       
		Signal wr_en,     
		Signal rd_en,     
		Signal dout,      
		Signal empty,     
		Signal data_count
	);

	Signal waddr, raddr;
	memory mem;
};
