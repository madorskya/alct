#include "stage0.h"
/*
void srl16v::operator()
(
	output q,
	input a,
	input clk,
	input d
)
{
initio
	q.init("q");
	a.init(3, 0, "a");
	clk.init("clk");
	d.init("d");
beginmodule
	comment("standard SRL16 element model - should be synthesized as SRL16")
	r.init(15, 0, "r");

	always (posedge (clk))
	begin
		r = (r(14, 0), d);
	end

	assign q = r(a);
endmodule
}

void fdrev::operator()
(
	output q,
	input c,
	input ce,
	input d,
	input r
)
{
initio
	q.init("q", makereg);
	c.init("c");
	ce.init("ce");
	d.init("d");
	r.init("r");
beginmodule
	comment("standard FDRE element model - should be synthesized as FDRE")
	always (posedge (c))
	begin
		If(r)  q = 0;
		Else If(ce) q = d;
	end

endmodule
}

void fdv::operator()
(
	output q,
	input c,
	input d
)
{
initio
	q.init("q", makereg);
	c.init("c");
	d.init("d");
beginmodule
	comment("standard FD element model - should be synthesized as FD")
	always (posedge (c))
	begin
		q = d;
	end

endmodule
}

  */

void OneShot::operator()
(
	Signal ly, 
	Signal lyr, 
	Signal trig_stop,
	Signal clk
)
{
initio
	ly.input("ly");
	lyr.output("lyr", makereg);
	Input (trig_stop);
	clk.input("clk");
beginmodule

	lyd.reg("lyd");
	lys.reg("lys");
	sri.reg(ExTime, 0, "sri");
//	sr.reg("srl16v", "sr");
//	flop.reg("fdrev", "flop");
//	iflop.reg("fdv", "iflop");

	comment(" //this seems to be the most economic way to create one shot in Xilinx. It takes advantage of the SRL16 element, which is the 16-bit shifter built on 1 (one) LUT")
	comment(" //to understand how it works, try to draw the shematics.")
/*	sr (lys, "4'h5", clk, sri);	
	flop (lyr, clk, sri, "1'b1", lys);
	iflop (lyd, clk, ly);
	
	assign sri = ly & !lyd;*/

	always (posedge (clk))
	begin
		If (lys) lyr = 0;
		Else If (sri(0)) lyr = 1;

		lys = sri(ExTime);
		sri = (sri(ExTime-1, 0), (Signal)"1'b0");
		sri(0) = ly & !lyd;
		lyd = ly;

	end
	
endmodule
}

void LyOneShot::operator()
(
	Signal ly, 
	Signal lyr,
	Signal trig_stop, 
	Signal clk
)
{
initio
	ly.input(LYWG-1, 0, "ly");
	lyr.output(LYWG-1, 0, "lyr");
	Input (trig_stop);
	clk.input("clk");
beginmodule
	comment(" //this module one-shots the whole layer")
	for(int i = 0; i < LYWG; i++)
	{
		sh[i].init("OneShot", "sh", i);	
		sh[i](ly(i), lyr(i), trig_stop, clk);
	}
endmodule
}

void Stage0::operator()
(
	Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
	Signal lyr0, Signal lyr1, Signal lyr2, Signal lyr3, Signal lyr4, Signal lyr5,
	Signal trig_stop,
	Signal clk
)
{
initio
    ly0.input(LYWG-1, 0, "ly0");
	ly1.input(LYWG-1, 0, "ly1");
	ly2.input(LYWG-1, 0, "ly2");
	ly3.input(LYWG-1, 0, "ly3");
	ly4.input(LYWG-1, 0, "ly4");
	ly5.input(LYWG-1, 0, "ly5");
    lyr0.output(LYWG-1, 0, "lyr0");
	lyr1.output(LYWG-1, 0, "lyr1");
	lyr2.output(LYWG-1, 0, "lyr2");
	lyr3.output(LYWG-1, 0, "lyr3");
	lyr4.output(LYWG-1, 0, "lyr4");
	lyr5.output(LYWG-1, 0, "lyr5");
	Input (trig_stop);
	clk.input("clk");
beginmodule

	comment(" //this module one-shots the whole chamber")
	for (int i = 0; i < LYN; i++) lsh[i].init("LyOneShot", "lsh", i);

	lsh[0] (ly0, lyr0, trig_stop, clk);
	lsh[1] (ly1, lyr1, trig_stop, clk);
	lsh[2] (ly2, lyr2, trig_stop, clk);
	lsh[3] (ly3, lyr3, trig_stop, clk);
	lsh[4] (ly4, lyr4, trig_stop, clk);
	lsh[5] (ly5, lyr5, trig_stop, clk);

endmodule
}
