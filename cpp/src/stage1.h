
#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#include "patterner.h"

class Stage1: public module 
{
public:

	Patterner pt[LYWG];
	void operator()
	(
	    Signal ly0p, Signal ly1p, Signal ly2p, Signal ly3p, Signal ly4p, Signal ly5p,
		Signal collmask,
	    Signal hitc, Signal hita, Signal patb,
		Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
	    Signal clk
	);
};

