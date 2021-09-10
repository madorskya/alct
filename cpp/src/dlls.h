// DLL component. Includes the reset counter.
#include "alctver.h"
#include "vlib.h"
#include "vmac.h"


class dll: public module
{
public:
#ifndef S6
	void operator() 
	(
		Signal CLKIN, 
		Signal CLK0, 
		Signal CLK2X, 
		Signal clksh,
		Signal clock_lac,
		Signal locked
	);
#else
    void operator()
    (
		Signal clkb,
		Signal clk,
		Signal clksh,
		Signal clksh_inv,
		Signal clk2x,
		Signal clk2xsh,
		Signal clock_lac,
		Signal locked
	);
#endif

	Signal clk2x;
};


class dll2: public module
{
public:
	void operator() 
	(
		Signal CLKIN, 
		Signal CLK0,
		Signal CLKSH, 
		Signal rst
	);
};


