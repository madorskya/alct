#include "patterner_rl.h"

void Patterner_rl::operator() 
(
	Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
	Signal collmask, 
	Signal sacp, Signal vacp,
#ifdef COLLPATB
	Signal sbcp, Signal vbcp,
#endif
	Signal sa, Signal va,
	Signal drifttime, 
	Signal pretrig, Signal trig, 
	Signal acc_pretrig, Signal acc_trig, 
	Signal trig_mode, 
	Signal clk
)
{
initio
	Input_(ly0, 2, 0);
	Input_(ly1, 1, 0);
	Input (ly2); 
	Input_(ly3, 1, 0);
	Input_(ly4, 2, 0); 
	Input_(ly5, 2, 0);
	Input_(collmask, 27, 0);
	OutReg_(sacp, 1, 0); // pattern a quality
	OutReg (vacp); // pattern a valid
#ifdef COLLPATB
	OutReg_(sbcp, 1, 0); // pattern b quality
	OutReg (vbcp); // pattern b valid
#endif
	OutReg_(sa, 1, 0); // acc track quality
	OutReg (va); // acc track valid
	Input_(drifttime, 2, 0);
	Input_(pretrig, 2, 0);
	Input_(trig, 2, 0);
	Input_(acc_pretrig, 2, 0);
	Input_(acc_trig, 2, 0);
	Input_(trig_mode, 1, 0);
	Input (clk);

beginmodule
	
	lya0cm.reg(2, 0, "lya0cm");
	lya1cm.reg(1, 0, "lya1cm");
	lya2cm.reg      ("lya2cm"); 
	lya3cm.reg(1, 0, "lya3cm");
	lya4cm.reg(2, 0, "lya4cm"); 
	lya5cm.reg(2, 0, "lya5cm");
	
	lyb0cm.reg(2, 0, "lyb0cm");
	lyb1cm.reg(1, 0, "lyb1cm");
	lyb2cm.reg      ("lyb2cm"); 
	lyb3cm.reg(1, 0, "lyb3cm");
	lyb4cm.reg(2, 0, "lyb4cm"); 
	lyb5cm.reg(2, 0, "lyb5cm");

	ly0am.reg("ly0am");
	ly4am.reg("ly4am");
	ly5am.reg("ly5am");
	ly1am.reg("ly1am");
	ly3am.reg("ly3am"); 
	ly2am.reg("ly2am");
	
	sumac.reg (2, 0, "sumac"); 
#ifdef COLLPATB
	sumbc.reg (2, 0, "sumbc"); 
#endif
	suma.reg(2, 0, "suma");
	bxac.reg(2, 0, "bxac");
#ifdef COLLPATB
	bxbc.reg(2, 0, "bxbc");
#endif
	bxa.reg (2, 0, "bxa");

#ifdef COLLPATB
	always 
	(
		ly0 or ly1 or ly2 or ly3 or ly4 or ly5 or 
		collmask or drifttime or pretrig or trig or trig_mode or 
		bxac or bxbc or 
		bxa or acc_pretrig or acc_trig
	)	
#else
	always 
	(
		ly0 or ly1 or ly2 or ly3 or ly4 or ly5 or 
		collmask or drifttime or pretrig or trig or trig_mode or 
		bxac or 
		bxa or acc_pretrig or acc_trig
	)	
#endif
	begin
#ifdef MIRROR
		ly0am = ly0(0);
		ly1am = ly1(0);
		ly2am = ly2   ;
		ly3am = ly3(1);
		ly4am = ly4(2);
		ly5am = ly5(2);
#else
		ly0am = ly0(2);
		ly1am = ly1(1);
		ly2am = ly2   ;
		ly3am = ly3(0);
		ly4am = ly4(0);
		ly5am = ly5(0);
#endif
		
		comment(" //count the bits in the accelerator muon pattern")
		suma = 
			ifelse(ly0am  ,"3'b1", "3'b0") + 
			ifelse(ly1am  ,"3'b1", "3'b0") + 
			ifelse(ly2am  ,"3'b1", "3'b0") + 
			ifelse(ly3am  ,"3'b1", "3'b0") + 
			ifelse(ly4am  ,"3'b1", "3'b0") + 
			ifelse(ly5am  ,"3'b1", "3'b0") ;
		sa = ifelse (suma >= 3, suma - 3, 0); 
		va = bxa == drifttime && suma >= acc_trig; // track valid when number of layers more or equal to required, and drifttime passed

		comment(" //mask out all bits which are not used in the collision pattern a")
		lya0cm = ly0 & collmask(2,0);
		lya1cm = ly1 & collmask(4,3);
		lya2cm = ly2 & collmask(5);
		lya3cm = ly3 & collmask(7,6);
		lya4cm = ly4 & collmask(10,8);
		lya5cm = ly5 & collmask(13,11);
		
		comment(" //count the bits in the collision pattern a")
		sumac = 
			ifelse((lya0cm(0) | lya0cm(1) | lya0cm(2))                         ,"3'b1", "3'b0")	+ 	
  			ifelse(            (lya1cm(0) | lya1cm(1))                         ,"3'b1", "3'b0")	+
			ifelse(                         lya2cm							   ,"3'b1", "3'b0") +	
			ifelse(                        (lya3cm(0) | lya3cm(1))			   ,"3'b1", "3'b0") +
			ifelse(                        (lya4cm(0) | lya4cm(1) | lya4cm(2)) ,"3'b1", "3'b0")	+ 	
			ifelse(                        (lya5cm(0) | lya5cm(1) | lya5cm(2)) ,"3'b1", "3'b0");
		sacp = ifelse(sumac >= 3, sumac - 3, 0);
		vacp = bxac == drifttime && sumac >= trig;

#ifdef COLLPATB
		comment(" //mask out all bits which are not used in the collision pattern b")
		lyb0cm = ly0 & collmask(16,14);
		lyb1cm = ly1 & collmask(18,17);
		lyb2cm = ly2 & collmask(19);
		lyb3cm = ly3 & collmask(21,20);
		lyb4cm = ly4 & collmask(24,22);
		lyb5cm = ly5 & collmask(27,25);
		
		comment(" //count the bits in the collision pattern b")
		sumbc = 
			ifelse((lyb0cm(0) | lyb0cm(1) | lyb0cm(2))						  ,"3'b1", "3'b0")	+ 	
  			ifelse(            (lyb1cm(0) | lyb1cm(1))						  ,"3'b1", "3'b0")	+
			ifelse(                         lyb2cm							  ,"3'b1", "3'b0")	+
			ifelse(                        (lyb3cm(0) | lyb3cm(1))			  ,"3'b1", "3'b0")	+
			ifelse(                        (lyb4cm(0) | lyb4cm(1) | lyb4cm(2)),"3'b1", "3'b0")	+ 	
			ifelse(                        (lyb5cm(0) | lyb5cm(1) | lyb5cm(2)),"3'b1", "3'b0");
		sbcp = ifelse(sumbc >= 3, sumbc - 3, 0);
		vbcp = bxbc == drifttime && sumbc >= trig;
#endif		

		comment(" //trig_mode == 3 kills coll muon if there is acc muon")
		If (trig_mode == 3 && va)
		begin
			vacp = 0;
#ifdef COLLPATB
			vbcp = 0;
#endif
		end
	end

	always (posedge (clk))	
	begin
		comment(" //BX counters are reset by the number of hits < 2, they stop counting at maximum count.")
		bxac = ifelse((sumac <     pretrig || trig_mode == 1), 0, ifelse((bxac == 7), 7, (bxac + 1))); comment(" //trig_mode == 1 kills coll muon")
#ifdef COLLPATB
		bxbc = ifelse((sumbc <     pretrig || trig_mode == 1), 0, ifelse((bxbc == 7), 7, (bxbc + 1))); comment(" //trig_mode == 1 kills coll muon")
#endif
		bxa  = ifelse((suma  < acc_pretrig || trig_mode == 2), 0, ifelse((bxa  == 7), 7, (bxa  + 1))); comment(" //trig_mode == 2 kills acc  muon")
	end
	
endmodule
}
