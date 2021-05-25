#include "RawHitMemory.h"

void RawHitMemory::operator()
(
	Signal addra,
	Signal addrb,
	Signal addrblock,
	Signal fifo_tbins,
	Signal canwrite,
	Signal clka,
	Signal clkb,
	Signal dina,
	Signal doutb,
	Signal wea
)
{
initio
	addra.input(7, 0, "addra");
	addrb.input(7, 0, "addrb");
	addrblock.input(7, 0, "addrblock");
	fifo_tbins.input(4, 0, "fifo_tbins");
	canwrite.output("canwrite");
	clka.input("clka");
	clkb.input("clkb");
	dina.input(HCMASKBITS-1, 0, "dina");
	doutb.output(HCMASKBITS-1, 0, "doutb", makereg);
	wea.input("wea");

beginmodule

	assign canwrite = ifelse ((addrblock - addra > fifo_tbins + 10) || (addrblock == addra), 1, 0); 
	comment("// set to 1 if there is space for one more readout or memory is empty")

#ifndef VGEN
	mem.reg(HCMASKBITS-1, 0, 255, 0, "mem"); 

	always (posedge (clka)) 
	begin 
		If (wea) mem[addra] = dina; 
	end 

	always (posedge (clkb))
	begin
		doutb = mem[addrb]; 
	end

#ifdef _VDEBUG
	if (posedge(clka).getbool() && posedge(clkb).getbool() && (addra == addrb).getbool())
	{
		cerr << "RawHitMemory: Possible problem: simultaneously reading and writing the same memory address: " << addra << "\n";
	}
#endif

#else

	printv
	("\
	rawmem rm0\n\
	(\n\
        .addra(addra),\n\
        .addrb(addrb),\n\
        .clka(clka),\n\
        .clkb(clkb),\n\
        .dina(dina[191:0]),\n\
        .doutb(doutb[191:0]),\n\
        .wea(wea)\n\
	);\n\
	rawmem rm1\n\
	(\n\
        .addra(addra),\n\
        .addrb(addrb),\n\
        .clka(clka),\n\
        .clkb(clkb),\n\
        .dina(dina[383:192]),\n\
        .doutb(doutb[383:192]),\n\
        .wea(wea)\n\
	);"\
	);
  
#endif

endmodule
}
