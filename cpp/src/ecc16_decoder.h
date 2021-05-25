#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class ecc16_decoder: public module
{
 public:
	void operator()
	(
		Signal dec_in,
		Signal parity_in,
		Signal ecc_en,
		Signal dec_out,
		Signal error
	);
	Signal syndrome_chk, mask, syndrome, error;
};
