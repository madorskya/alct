#include "patterner.h"

void Patterner::operator() 
(
	Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
	Signal collmask, 
	Signal sc,
	Signal sa,
	Signal patb,
	Signal drifttime, Signal pretrig, Signal trig, Signal trig_mode, 
	Signal clk
)
{
initio
	ly0.input(2, 0, "ly0");
	ly1.input(1, 0, "ly1");
	ly2.input("ly2"); 
	ly3.input(1, 0, "ly3");
	ly4.input(2, 0, "ly4"); 
	ly5.input(2, 0, "ly5");
	collmask.input(27, 0, "collmask");
	sc.output(1, 0, "sc", makereg);
	sa.output(1, 0, "sa", makereg);
	patb.output("patb", makereg);
	drifttime.input(2, 0, "drifttime");
	pretrig.input(2, 0, "pretrig");
	trig.input(2, 0, "trig");
	trig_mode.input(1, 0, "trig_mode");
	clk.input("clk");

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
	
	sac.reg (2, 0, "sac"); 
	sbc.reg (2, 0, "sbc"); 
	scl.reg (2, 0, "scl"); 
	scll.reg(2, 0, "scll"); 
	sal.reg (2, 0, "sal"); 

	sumac.reg (2, 0, "sumac"); 
	sumbc.reg (2, 0, "sumbc"); 
	suma.reg(2, 0, "suma");
	bxac.reg(2, 0, "bxac");
	bxbc.reg(2, 0, "bxbc");
	bxa.reg (2, 0, "bxa");
	patbl.reg("patbl");

	always (posedge (clk))	
	begin
		ly0am = ly0(2);
		ly1am = ly1(1);
		ly2am = ly2   ;
		ly3am = ly3(0);
		ly4am = ly4(0);
		ly5am = ly5(0);
		
		comment(" //count the bits in the accelerator muon pattern")
		suma = ly0am + ly1am + ly2am + ly3am + ly4am + ly5am; 
		sal = ifelse((bxa == drifttime && suma >= trig), (suma - trig), 0);
		sa = ifelse ((sal > 3), 3, sal);


		comment(" //mask out all bits which are not used in the collision pattern a")
		lya0cm = ly0 & collmask(2,0);
		lya1cm = ly1 & collmask(4,3);
		lya2cm = ly2 & collmask(5);
		lya3cm = ly3 & collmask(7,6);
		lya4cm = ly4 & collmask(10,8);
		lya5cm = ly5 & collmask(13,11);
		
		comment(" //count the bits in the collision pattern a")
		sumac = 
			(lya0cm(0) | lya0cm(1) | lya0cm(2))							+ 	
  			            (lya1cm(0) | lya1cm(1))							+
			                         lya2cm								+	
			                        (lya3cm(0) | lya3cm(1))				+
			                        (lya4cm(0) | lya4cm(1) | lya4cm(2))	+ 	
			                        (lya5cm(0) | lya5cm(1) | lya5cm(2));
		sac = ifelse((bxac == drifttime), sumac, 0);

		comment(" //mask out all bits which are not used in the collision pattern b")
		lyb0cm = ly0 & collmask(16,14);
		lyb1cm = ly1 & collmask(18,17);
		lyb2cm = ly2 & collmask(19);
		lyb3cm = ly3 & collmask(21,20);
		lyb4cm = ly4 & collmask(24,22);
		lyb5cm = ly5 & collmask(27,25);
		
		comment(" //count the bits in the collision pattern b")
		sumbc = 
			(lyb0cm(0) | lyb0cm(1) | lyb0cm(2))							+ 	
  			            (lyb1cm(0) | lyb1cm(1))							+
			                         lyb2cm								+
			                        (lyb3cm(0) | lyb3cm(1))				+
			                        (lyb4cm(0) | lyb4cm(1) | lyb4cm(2))	+ 	
			                        (lyb5cm(0) | lyb5cm(1) | lyb5cm(2));
		sbc = ifelse((bxbc == drifttime), sumbc, 0);
		
		comment(" //select the best from collision patterns a and b, and set patb flag")
		If (sac >= sbc)
		begin
			scl = sac;
			patbl = 0;
		end
		Else
		begin
			scl = sbc;
			patbl = 1;
		end
		
		scll = ifelse((scl >= trig), (scl - trig), 0);
		comment(" //trig_mode == 3 kills coll muon if there is acc muon")
		sc = ifelse((trig_mode == 3 && sa != 0), 0, ifelse((scll > 3), 3, scll));
		patb = patbl;


		comment(" //BX counters are reset by the number of hits < 2, they stop counting at maximum count.")
		bxac = ifelse((sumac < pretrig || trig_mode == 1), 0, ifelse((bxac == 7), 7, (bxac + 1))); comment(" //trig_mode == 1 kills coll muon")
		bxbc = ifelse((sumbc < pretrig || trig_mode == 1), 0, ifelse((bxbc == 7), 7, (bxbc + 1))); comment(" //trig_mode == 1 kills coll muon")
		bxa  = ifelse((suma  < pretrig || trig_mode == 2), 0, ifelse((bxa  == 7), 7, (bxa  + 1))); comment(" //trig_mode == 2 kills acc  muon")
	end
	
endmodule
}
