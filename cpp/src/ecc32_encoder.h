#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class ecc32_encoder:public module
{
 public:
	void operator() 
	(
		Signal enc_in,
		Signal enc_out,
		Signal parity_out,
		Signal clk
	);

	Signal enc_chk;

};
