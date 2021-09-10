#include "stage1a.h"

void CheckNeighbors::operator()
(
	Signal sc, Signal scr, Signal sctop, Signal scbot,
	Signal sa, Signal sar, Signal satop, Signal sabot, 
	Signal clk
)
{
initio

	sc.input   (QBITS-1, 0, "sc");
	scr.output  (QBITS-1, 0, "scr", makereg);
	sctop.input(QBITS-1, 0, "sctop");
	scbot.input(QBITS-1, 0, "scbot");
	sa.input   (QBITS-1, 0, "sa");
	sar.output  (QBITS-1, 0, "sar", makereg);
	satop.input(QBITS-1, 0, "satop");
	sabot.input(QBITS-1, 0, "sabot");
	clk.input  ("clk");

beginmodule

	parameter KILLTIME("KILLTIME", 4);

	cntc.reg(2, 0, "cntc");
	cnta.reg(2, 0, "cnta");
	cntcn.reg(2, 0, "cntcn");
	cntan.reg(2, 0, "cntan");


	always (sc or sctop or scbot or sa or satop or sabot or cntc or cnta)
	begin
		comment(" //check the collision patterns")
		cntcn = cntc;
#ifdef KILLEQUAL
        If ((sc <= sctop && sctop != 0) || (sc < scbot)) 
#else
		If (sc < sctop || sc < scbot)
#endif
		begin
			comment(" //if one of the neighbors is larger than me, kill myself. If top neighbor is equal to me, kill myself.")
			scr = 0;						
			comment(" //set the counter, so I will be killed for the KILLTIME clocks")
			cntcn = KILLTIME;				
		end
		Else
		begin
			If (cntc > 0)
			begin
				comment(" //if the counter still have not ran out, kill myself")
				scr = 0;					
				comment(" //decrease the counter")
				cntcn = cntc - 1;						
			end
			Else
			begin
				comment(" //OK, no larger neighbors now or KILLTIME clocks in the past")
				scr = sc;					
			end
		end
				
		comment(" //check the accelerator patterns")
		cntan = cnta;
#ifdef KILLEQUAL
        If ((sa <= satop && satop != 0) || (sa < sabot)) 
#else
		If (sa < satop || sa < sabot)
#endif
		begin
			comment(" //if one of the neighbors is larger than me, kill myself. If top neighbor is equal to me, kill myself.")
			sar = 0;						
			comment(" //set the counter, so I will be killed for the KILLTIME clocks")
			cntan = KILLTIME;				
		end
		Else
		begin
			If (cnta > 0)
			begin
				comment(" //if the counter still have not ran out, kill myself")
				sar = 0;					
				comment(" //decrease the counter")
				cntan = cnta - 1;						
			end
			Else
			begin
				comment(" //OK, no larger neighbors now or KILLTIME clocks in the past")
				sar = sa;					
			end
		end
	end

	always (posedge (clk))
	begin
		cnta = cntan;
		cntc = cntcn;
	end
	

endmodule
}

void Stage1a::operator()
(
	Signal hitc, Signal hita, Signal patb,
	Signal hitcr, Signal hitar, Signal patbr,
	Signal bestqcr, Signal bestqar, 
	Signal clk
)
{
initio

    hitc.input (HITSBITS-1, 0, "hitc"); 
    hita.input (HITSBITS-1, 0, "hita"); 
    patb.input (LYWG-1,     0, "patb");
    hitcr.output(HITSBITS-1, 0, "hitcr", makereg); 
    hitar.output(HITSBITS-1, 0, "hitar", makereg); 
    patbr.output(LYWG-1,     0, "patbr", makereg);
	bestqcr.output(BEST2*QBITS-1, 0, "bestqcr", makereg);
	bestqar.output(BEST2*QBITS-1, 0, "bestqar", makereg);
    clk.input  ("clk");

beginmodule    

    hitci.wire(HITSBITS-1, 0, "hitci"); 
    hitai.wire(HITSBITS-1, 0, "hitai"); 
	bestqc.wire(BEST2*QBITS-1, 0, "bestqc");
	bestqa.wire(BEST2*QBITS-1, 0, "bestqa");
	int i;

	comment(" //create the cancellation logic for each key WG")
	for (i = 0; i < LYWG; i++)
	{
		chn[i].init("CheckNeighbors", "chn", i);
		chn[i]
		(
			hitc (i*QBITS + QBITS-1, i*QBITS),
			hitci(i*QBITS + QBITS-1, i*QBITS),
			(i == 0)      ? ZEROQ : hitc ((i-1)*QBITS + QBITS-1, (i-1)*QBITS),
			(i == LYWG-1) ? ZEROQ : hitc ((i+1)*QBITS + QBITS-1, (i+1)*QBITS),

			hita (i*QBITS + QBITS-1, i*QBITS),
			hitai(i*QBITS + QBITS-1, i*QBITS),
			(i == 0)      ? ZEROQ : hita ((i-1)*QBITS + QBITS-1, (i-1)*QBITS),
			(i == LYWG-1) ? ZEROQ : hita ((i+1)*QBITS + QBITS-1, (i+1)*QBITS),

			clk
		);
	}
        
	comment("// find the best quality for collision and accelerator tracks in groups of 8")
	for (i = 0; i < BEST2; i++)
	{
		bqc[i].init("bestquality", "bqc", i);
		bqc[i].ninp = BEST1;
		bqc[i]
		(
			hitci (i*BEST1*QBITS+BEST1*QBITS-1, i*BEST1*QBITS), 			
			bestqc (i*QBITS+QBITS-1, i*QBITS)
		);

		bqa[i].init("bestquality", "bqa", i);
		bqa[i].ninp = BEST1;
		bqa[i]
		(
			hitai (i*BEST1*QBITS+BEST1*QBITS-1, i*BEST1*QBITS), 			
			bestqa (i*QBITS+QBITS-1, i*QBITS)
		);
	}


    always (posedge (clk)) 
	begin
		patbr = patb;
		hitcr = hitci;
		hitar = hitai;
		bestqcr = bestqc;
		bestqar = bestqa;
	end
    
endmodule
}
