#include "stage1_rl.h"

void CheckNeighbors_rl::operator()
(
	Signal vc, Signal sc, Signal vcr, 
	Signal vctop, Signal sctop, 
	Signal vcbot, Signal scbot,

	Signal va, Signal sa, Signal varr, 
	Signal vatop, Signal satop, 
	Signal vabot, Signal sabot, 

	Signal trig_stop,
	Signal clk
)
{
initio

	Input (vc);
	sc.input   (QBITS-1, 0, "sc");
	OutReg (vcr);
	Input (vctop);
	sctop.input(QBITS-1, 0, "sctop");
	Input (vcbot);
	scbot.input(QBITS-1, 0, "scbot");

	Input (va);
	sa.input   (QBITS-1, 0, "sa");
	OutReg (varr);
	Input (vatop);
	satop.input(QBITS-1, 0, "satop");
	Input (vabot);
	sabot.input(QBITS-1, 0, "sabot");

	Input (trig_stop);
	clk.input  ("clk");

beginmodule

	parameter KILLTIME("KILLTIME", 4);

	cntc.reg(2, 0, "cntc");
	cnta.reg(2, 0, "cnta");
	cntcn.reg(2, 0, "cntcn");
	cntan.reg(2, 0, "cntan");

	always (sc or sctop or scbot or sa or satop or sabot or cntc or cnta or vc or va or vctop or vcbot or vatop or vabot)
	begin

		 //check the collision patterns
		cntcn = cntc;
		 //initially, take valid flag to output
		vcr = vc;					

#ifdef KILLEQUAL
        If ((sc <= sctop && vctop && vc) || (sc < scbot && vcbot && vc))
#else
        If ((sc <  sctop && vctop && vc) || (sc < scbot && vcbot && vc))
#endif
		begin
			 //if one of the neighbors is larger than me, kill myself. If top neighbor is equal to me, kill myself.
			vcr = 0;			
		end

		If (cntc > 0)
		begin
			 //if the counter still have not run out, kill myself
			vcr = 0;					
			 //decrease the counter
			cntcn = cntc - 1;						
		end

		If ((vctop || vcbot) && !vc)
		begin			
			 //set the counter, so I will be killed for the KILLTIME clocks
			cntcn = KILLTIME;				
		end
				
		 //check the accelerator patterns
		cntan = cnta;

		varr = va;					

#ifdef KILLEQUAL
        If ((sa <= satop && vatop && va) || (sa < sabot && vabot && va))
#else
        If ((sa <  satop && vatop && va) || (sa < sabot && vabot && va))
#endif
		begin
			 //if one of the neighbors is larger than me, kill myself. If top neighbor is equal to me, kill myself.
			varr = 0;						
		end

		If (cnta > 0)
		begin
			 //if the counter still have not ran out, kill myself
			varr = 0;
			 //decrease the counter
			cntan = cnta - 1;						
		end

		If ((vatop || vabot) && !va)
		begin			
			 //set the counter, so I will be killed for the KILLTIME clocks
			cntan = KILLTIME;				
		end

	end

	always (posedge (clk))
	begin
		If (trig_stop)
		begin
			cntc = 0;
			cnta = 0;
		end
		Else
		begin
			cnta = cntan;
			cntc = cntcn;
		end
	end
	

endmodule
}


void Stage1_rl::operator()
(
    Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
	Signal collmask,
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode,
	Signal acc_pretrig, Signal acc_trig,
	Signal qca0, Signal qca1, Signal qca2, Signal qca3, 
	Signal qa0,  Signal qa1,  Signal qa2,  Signal qa3, 
	Signal trig_stop,
    Signal clk
)
{
initio

	Input_(ly0,LYWG-1, 0);
	Input_(ly1,LYWG-1, 0);
	Input_(ly2,LYWG-1, 0);
	Input_(ly3,LYWG-1, 0);
	Input_(ly4,LYWG-1, 0);
	Input_(ly5,LYWG-1, 0);
    Input_(collmask, COLLMASKBITS-1, 0);
	Input_(drifttime, 2, 0);
	Input_(pretrig, 2, 0);
	Input_(trig, 2, 0);
	Input_(trig_mode, 1, 0);
	Input_(acc_pretrig, 2, 0);
	Input_(acc_trig, 2, 0);
	OutReg_(qca0, LYWG-1, 0);
	OutReg_(qca1, LYWG-1, 0);
	OutReg_(qca2, LYWG-1, 0);
	OutReg_(qca3, LYWG-1, 0);
	OutReg_(qa0,  LYWG-1, 0);
	OutReg_(qa1,  LYWG-1, 0);
	OutReg_(qa2,  LYWG-1, 0);
	OutReg_(qa3,  LYWG-1, 0);
	Input (trig_stop);
    Input (clk);

beginmodule    

    Wire_(hitca, HITSBITS-1, 0); // qualities of patterns a found 
#ifdef COLLPATB
	Wire_(hitcb, HITSBITS-1, 0); // qualities of patterns b found
#endif
	Wire_(hita, HITSBITS-1, 0);  // qualities of accelerator patterns found

	Wire_(vac, LYWG-1, 0); // valid flags
	Wire_(vaci, LYWG-1, 0); // valid flags corrected by ghost-cacellation
#ifdef COLLPATB
	Wire_(vbc, LYWG-1, 0);
	Wire_(vbci, LYWG-1, 0);
#endif
	Wire_(va,  LYWG-1, 0);
	Wire_(vai,  LYWG-1, 0);

	int i;

	 //create pattern detectors for each WG. Two first and two last of them will have the missing inputs zeroed (no WGs beyond the chamber edge).
	for (i = 0; i < LYWG; i++)
	{
		pt[i].init("Patterner_rl", "pt", i);
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

			hitca(i*QBITS+QBITS-1, i*QBITS), vac(i),
#ifdef COLLPATB
			hitcb(i*QBITS+QBITS-1, i*QBITS), vbc(i),
#endif
			hita (i*QBITS+QBITS-1, i*QBITS), va(i),

			drifttime, 
			pretrig, 
			trig, 
			acc_pretrig, 
			acc_trig, 
			trig_mode, 
			clk
		);
	}

	 //create the cancellation logic for each key WG
	for (i = 0; i < LYWG; i++)
	{
		chn[i].init("CheckNeighbors_rl", "chn", i);
		chn[i]
		(
			vac (i),
			hitca  (i*QBITS + QBITS-1, i*QBITS),
			vaci (i),
			(i == 0)      ? "1'b0": vac (i-1),
			(i == 0)      ? ZEROQ : hitca ((i-1)*QBITS + QBITS-1, (i-1)*QBITS),
			(i == LYWG-1) ? "1'b0": vac (i+1),
			(i == LYWG-1) ? ZEROQ : hitca ((i+1)*QBITS + QBITS-1, (i+1)*QBITS),

			va (i),
			hita (i*QBITS + QBITS-1, i*QBITS),
			vai (i),
			(i == 0)      ? "1'b0": va (i-1),
			(i == 0)      ? ZEROQ : hita ((i-1)*QBITS + QBITS-1, (i-1)*QBITS),
			(i == LYWG-1) ? "1'b0": va (i+1),
			(i == LYWG-1) ? ZEROQ : hita ((i+1)*QBITS + QBITS-1, (i+1)*QBITS),

			trig_stop,
			clk
		);
	}

	 //create positional quality code for sorter in stage2
	always (posedge (clk))
	begin
		for (i = 0; i < LYWG; i++)
		{
			qca3(i) = hitca (i*QBITS + QBITS-1, i*QBITS) == 3 && vaci(i);
			qca2(i) = hitca (i*QBITS + QBITS-1, i*QBITS) == 2 && vaci(i);
			qca1(i) = hitca (i*QBITS + QBITS-1, i*QBITS) == 1 && vaci(i);
			qca0(i) = hitca (i*QBITS + QBITS-1, i*QBITS) == 0 && vaci(i);

			qa3(i)  = hita  (i*QBITS + QBITS-1, i*QBITS) == 3 && vai(i);
			qa2(i)  = hita  (i*QBITS + QBITS-1, i*QBITS) == 2 && vai(i);
			qa1(i)  = hita  (i*QBITS + QBITS-1, i*QBITS) == 1 && vai(i);
			qa0(i)  = hita  (i*QBITS + QBITS-1, i*QBITS) == 0 && vai(i);
		}
	end


endmodule
}
