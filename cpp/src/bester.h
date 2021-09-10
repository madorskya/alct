#include "alctver.h"
#include "vlib.h"
#include "vmac.h"
#include "bestquality.h"

#ifndef _BESTER_H_
#define _BESTER_H_



class bestof : public module
{
public:
	int ninp; // parameter - number of inputs

	void operator()
	(
		Signal hits,
		Signal bestq,
		Signal bestn
	);
};


class Bester : public module
{
public:

	bestquality bq[BEST2];
	bestquality bqm;
	bestof bo[BEST2];
	bestof bom;
	Signal bestn[BEST2];
	Signal bestq;
	Signal maxnr;
	void operator()
	(
		Signal hits,
		Signal bestq,
		Signal max,
		Signal maxn
	);
};

#endif
