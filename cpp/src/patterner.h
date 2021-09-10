#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class Patterner: public module
{
public:
	
	Signal lya0cm, lya4cm, lya5cm;
	Signal lya1cm, lya3cm;
	Signal lya2cm;

	Signal lyb0cm, lyb4cm, lyb5cm;
	Signal lyb1cm, lyb3cm;
	Signal lyb2cm;

	Signal ly0am, ly4am, ly5am, ly1am, ly3am, ly2am;
	
	Signal sac, sbc, scl, scll, sal;
	Signal sumac, sumbc;
	Signal suma;
	Signal bxac, bxbc, bxa;
	Signal patbl;

	
	void operator() 
	(
		Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
		Signal collmask, 
		Signal scp,
		Signal sap,
		Signal patb,
		Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode, 
		Signal clk
	);
};

