#include "dlls.h"

#ifndef S6
void dll::operator() 
(
	Signal CLKIN, 
	Signal CLK0, 
	Signal CLK2X, 
	Signal clksh,
	Signal clock_lac, 
	Signal locked
)
{
initio
	CLKIN.input("CLKIN");
	CLK0.output("CLK0"); 
	CLK2X.output("CLK2X"); 
	Output (clksh);
	Output (clock_lac);
	locked.output("locked");

beginmodule
	clk2x.wire("clk2x"); // this wire is clocked from the test fixture

	assign CLK0 = CLKIN;
	assign locked = 1;
	assign CLK2X = clk2x;
	assign clksh = CLKIN;
	assign clock_lac = CLKIN;

endmodule
}
#else
void dll::operator()
(
	Signal clkb,
	Signal clk,
	Signal clksh,
	Signal clksh_inv,
	Signal clk2xp,
	Signal clk2xsh,
	Signal clock_lac,
	Signal locked
)
{
initio
	Input (clkb);
	Output (clk);
	Output (clksh);
	Output (clksh_inv);
	Output (clk2xp);
	Output (clk2xsh);
	Output (clock_lac);
	Output (locked);
beginmodule
	clk2x.wire("clk2x"); // this wire is clocked from the test fixture

	assign clk = clkb;
	assign clksh = clkb;
	assign clksh_inv = !clkb;
	assign clk2xp = clk2x;
	assign clk2xsh = clk2x;
	assign locked = 1;
	assign clock_lac = clk;
endmodule
}
#endif


void dll2::operator() 
(
	Signal CLKIN, 
	Signal CLK0,
	Signal CLKSH, 
	Signal rst
)
{
initio
	CLKIN.input("CLKIN");
	Output (CLK0);
	CLKSH.output("CLKSH");
	rst.input("rst");
beginmodule
	assign CLK0 = CLKIN;
	assign CLKSH = CLKIN;
endmodule
}



