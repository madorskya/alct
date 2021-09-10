#include "stage3.h"

void maskout::operator()
(
	Signal hitcr, Signal hitar, 
	Signal hitcrr, Signal hitarr,
	Signal best1n, Signal fa1
)
{
initio

	hitcr.input (HITSBITS-1, 0, "hitcr");
	hitar.input (HITSBITS-1, 0, "hitar");
	hitcrr.output(HITSBITS-1, 0, "hitcrr", makereg);
	hitarr.output(HITSBITS-1, 0, "hitarr", makereg);
	best1n.input(KEYBITS-1, 0, "best1n");
	fa1.input ("fa1");

beginmodule
	i.reg(6, 0, "i");

    always (hitcr or hitar or best1n or fa1)
    begin
		comment(" //first, put all WG qualities to the output")
		hitcrr = hitcr; 
		hitarr = hitar;

		For (i = 0, i < LYWG, i++)
		begin
			comment(" //if this is the best track WG number - mask it out")
			If (i == best1n) 
			begin
				comment(" //be careful to mask out the collision or acc track")
				If (fa1 == 0) 
				begin
					hitcrr(i*QBITS) = 0;
					hitcrr(i*QBITS+1) = 0;
				end
				Else
				begin
					hitarr(i*QBITS) = 0;
					hitarr(i*QBITS+1) = 0;
				end
			end
		end
	end    
endmodule
}

void Stage3::operator()
(
	Signal hitcr, Signal hitar, Signal patbr,
	Signal hitcrr, Signal hitarr, Signal patbrr,
	Signal best1, Signal best1n, Signal fa1, 
	Signal best1r, Signal best1nr, Signal fa1r,
	Signal bestqcr, Signal bestqar, 
	Signal clk
)
{
initio

	hitcr.input (HITSBITS-1, 0, "hitcr");
	hitar.input (HITSBITS-1, 0, "hitar");
	patbr.input (LYWG-1, 0, "patbr");
	hitcrr.output(HITSBITS-1, 0, "hitcrr", makereg);
	hitarr.output(HITSBITS-1, 0, "hitarr", makereg);
	patbrr.output(LYWG-1, 0, "patbrr", makereg);
	best1.input (QBITS-1, 0, "best1");
	best1n.input(KEYBITS-1, 0, "best1n");
	fa1.input   ("fa1");
	best1r.output(QBITS-1, 0, "best1r", makereg);
	best1nr.output(KEYBITS-1, 0, "best1nr", makereg);
	fa1r.output  ("fa1r", makereg);
	bestqcr.output(BEST2*QBITS-1, 0, "bestqcr", makereg);
	bestqar.output(BEST2*QBITS-1, 0, "bestqar", makereg);
	clk.input   ("clk");

beginmodule    

	hitci.wire(HITSBITS-1, 0, "hitci");
	hitai.wire(HITSBITS-1, 0, "hitai");
	bestqc.wire(BEST2*QBITS-1, 0, "bestqc");
	bestqa.wire(BEST2*QBITS-1, 0, "bestqa");
    
	comment("// mask out the first best trask quality")
	mo.init("maskout", "mo");
	mo
	(
		hitcr, hitar,
		hitci, hitai,
		best1n, fa1
	);


	comment("// find the best quality for collision and accelerator tracks in groups of 8")
	for (int i = 0; i < BEST2; i++)
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
		comment(" //pass thru all best track data")
		best1r   = best1;   
		best1nr  = best1n;
		fa1r     = fa1;
		patbrr   = patbr;
		hitcrr = hitci;
		hitarr = hitai;
		bestqcr = bestqc;
		bestqar = bestqa;
	end

endmodule

}
