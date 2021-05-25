#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

/*
class srl16v : public module
{
public:
	Signal r;
	void operator()
	(
		Signal q,
		Signal a,
		Signal clk,
		Signal d
	);
};


class fdrev: public module 
{
public:
	void operator()
	(
		Signal q,
		Signal c,
		Signal ce,
		Signal d,
		Signal r
	);
};

class fdv: public module 
{
public:
	void operator()
	(
		Signal q,
		Signal c,
		Signal d
	);
};
  */

class OneShot : public module
{
public:
	Signal lyd;
	Signal lys, sri;	
//	srl16v sr;
//	fdrev flop;
//	fdv iflop;

	void operator()
	(
		Signal ly, 
		Signal lyr,
		Signal trig_stop,
		Signal clk
	);
};


class LyOneShot: public module
{
public:

	OneShot sh[LYWG];
	void operator()
	(
		Signal ly,
		Signal lyr,
		Signal trig_stop,
		Signal clk
	);
};


class Stage0 : public module
{
public:
	LyOneShot lsh[LYN];
	void operator()
	(
		Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
		Signal lyr0, Signal lyr1, Signal lyr2, Signal lyr3, Signal lyr4, Signal lyr5,
		Signal trig_stop,
		Signal clk
	);
};



