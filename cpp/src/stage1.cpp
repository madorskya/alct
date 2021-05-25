#include "stage1.h"

void Stage1::operator()
(
    Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
	Signal collmask,
    Signal hitc, Signal hita, Signal patb,
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
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
    collmask.input(COLLMASKBITS-1, 0, "collmask");
    hitc.output(HITSBITS-1, 0, "hitc"); 
	hita.output(HITSBITS-1, 0, "hita"); 
    patb.output(LYWG-1, 0, "patb");

	drifttime.input(2, 0, "drifttime");
	pretrig.input(2, 0, "pretrig");
	trig.input(2, 0, "trig");
	trig_mode.input(1, 0, "trig_mode");
    clk.input("clk");

beginmodule    


	comment(" //create pattern detectors for each WG. Two first and two last of them will have the missing inputs zeroed (no WGs beyond the chamber edge).")
	for (int i = 0; i < LYWG; i++)
	{
		pt[i].init("Patterner", "pt", i);
		pt[i]
		(
#ifndef MIRROR // normal version 

			(i == 0) ? (ly0(i), (Signal)"2'h0") : (i == 1) ? (ly0(i,i-1), (Signal)"1'h0") : ly0(i, i - 2),
			(i == 0) ? (ly1(i), (Signal)"1'h0") : ly1(i, i - 1),
			ly2(i),
			(i == LYWG-1) ? ((Signal)"1'h0", ly3(i)) : ly3(i + 1, i),
			(i == LYWG-1) ? ((Signal)"2'h0", ly4(i)) : (i == LYWG-2) ? ((Signal)"1'h0", ly4(i + 1, i)) : ly4(i + 2, i),
			(i == LYWG-1) ? ((Signal)"2'h0", ly5(i)) : (i == LYWG-2) ? ((Signal)"1'h0", ly5(i + 1, i)) : ly5(i + 2, i),

#else // version with mirrored trigger patterns, for the opposite side of the disk. 3/19/2005
      // will require careful pattern programming - the meaning of bits in pattern registers will change

			(i == LYWG-1) ? ((Signal)"2'h0", ly0(i)) : (i == LYWG-2) ? ((Signal)"1'h0", ly0(i+1,i)) : ly0(i+2, i),
			(i == LYWG-1) ? ((Signal)"1'h0", ly1(i)) : ly1(i+1, i),
			ly2(i),
			(i == 0) ? (ly3(i), (Signal)"1'h0") : ly3(i, i-1),
			(i == 0) ? (ly4(i), (Signal)"2'h0") : (i == 1) ? (ly4(i, i-1), (Signal)"1'h0") : ly4(i, i-2),
			(i == 0) ? (ly5(i), (Signal)"2'h0") : (i == 1) ? (ly5(i, i-1), (Signal)"1'h0") : ly5(i, i-2),

#endif

			collmask((i/WGBLK)*PATTERNBITS + PATTERNBITS - 1, (i/WGBLK)*PATTERNBITS),
			hitc(i*QBITS+QBITS-1, i*QBITS),
			hita(i*QBITS+QBITS-1, i*QBITS),
			patb(i),
			drifttime, 
			pretrig, 
			trig, 
			trig_mode, 
			clk
		);
	}

endmodule
}
