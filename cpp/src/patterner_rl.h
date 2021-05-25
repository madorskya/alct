#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class Patterner_rl: public module
{
public:
	
	Signal lya0cm, lya4cm, lya5cm;
	Signal lya1cm, lya3cm;
	Signal lya2cm;

	Signal lyb0cm, lyb4cm, lyb5cm;
	Signal lyb1cm, lyb3cm;
	Signal lyb2cm;

	Signal ly0am, ly4am, ly5am, ly1am, ly3am, ly2am;
	
#ifdef COLLPATB
	Signal sumbc, bxbc;
#endif
	Signal sumac, suma;
	Signal bxac, bxa;

	
	void operator() 
	(
		Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
		Signal collmask, 
		Signal sacp, Signal vacp,
#ifdef COLLPATB
		Signal sbcp, Signal vbcp,
#endif
		Signal sa, Signal va,
		Signal drifttime, 
		Signal pretrig, Signal trig, 
		Signal acc_pretrig, Signal acc_trig, 
		Signal trig_mode, 
		Signal clk
	);
};

