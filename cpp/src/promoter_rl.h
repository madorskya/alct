#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#ifndef _PROMOTER_RL_H_
#define _PROMOTER_RL_H_

class promoter_rl: public module
{
public:
	
	void operator()
	(
		Signal wc1, Signal qc1, Signal vc1,
		Signal wc2, Signal qc2,	Signal vc2,
		Signal wa1, Signal qa1, Signal va1,
		Signal wa2, Signal qa2,	Signal va2,

		Signal bw1, Signal bq1, Signal fa1, Signal bv1,
		Signal bw2, Signal bq2, Signal fa2,	Signal bv2,

		Signal p,
		Signal clk
	);

	Signal c1c2, c1a1, c1a2, c2a1, c2a2, a1a2, rc1, rc2, ra1, ra2;
	Signal b1c1, // first best flags
	b1c2,
	b1a1,
	b1a2,

	b2c1, // second best flags
	b2c2,
	b2a1,
	b2a2,

	wc1and1, // AND of track parameters and first best flags
	qc1and1, 
	wc2and1, 
	qc2and1,
 					   
	wa1and1, 
	qa1and1, 
	wa2and1, 
	qa2and1, 
					   
	wc1and2, // AND of track parameters and second best flags
	qc1and2, 
	wc2and2, 
	qc2and2,
 					   
	wa1and2, 
	qa1and2, 
	wa2and2, 
	qa2and2; 

	Signal vc1and1, vc2and1, va1and1, va2and1, vc1and2, vc2and2, va1and2, va2and2;
	Signal vc1r, vc2r, va1r, va2r;
	Signal wc1r, qc1r, wc2r, qc2r;
	Signal wa1r, qa1r, wa2r, qa2r;
};

#endif
