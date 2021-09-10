#include "rollbackraw.h"

void RollBackRaw::operator()
(
	Signal DIN,
	Signal DOUT,
	Signal RollBack,
	Signal we,
	Signal CLK
)
{
initio
	DIN.input(dwidth-1, 0, "DIN");
	DOUT.output(dwidth-1, 0, "DOUT", makereg);
	RollBack.input(7, 0, "RollBack");
	we.input("we");
	CLK.input("CLK");

beginmodule

#ifndef VGEN
	mem.reg(dwidth-1, 0, 255, 0, "mem"); 

	ADDRA.reg(7, 0, "ADDRA");
	ADDRB.reg(7, 0, "ADDRB");

	always (posedge (CLK)) 
	begin 
		If (we) mem[ADDRA] = DIN; 
		DOUT = mem[ADDRB]; 
		ADDRB = ADDRA - RollBack + 1;
		ADDRA = ADDRA + 1;
	end 

#ifdef _VDEBUG
	if (posedge(CLK).getbool() && (ADDRA == ADDRB).getbool())
	{
		cerr << "RollBackMemory: Possible problem: simultaneously reading and writing the same memory address: " << ADDRA << "\n";
	}
#endif

#else
	ADDRA.reg(7, 0, "ADDRA");
	ADDRB.reg(7, 0, "ADDRB");

	always (posedge (CLK)) 
	begin 
		ADDRB = ADDRA - RollBack + 1;
		ADDRA = ADDRA + 1;
	end 

	printv
	("\n\
	rollbackmem rb0\n\
	(\n\
        .addra(ADDRA),\n\
        .addrb(ADDRB),\n\
        .clka (CLK),\n\
        .clkb (CLK),\n\
        .dina (DIN[191:0]),\n\
        .doutb(DOUT[191:0]),\n\
        .wea  (we)\n\
    );\n\
	rollbackmem rb1\n\
	(\n\
        .addra(ADDRA),\n\
        .addrb(ADDRB),\n\
        .clka (CLK),\n\
        .clkb (CLK),\n\
        .dina (DIN[383:192]),\n\
        .doutb(DOUT[383:192]),\n\
        .wea  (we)\n\
    );"
	);
#endif
endmodule
}
