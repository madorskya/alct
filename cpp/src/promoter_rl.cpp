
#include "promoter_rl.h"

void promoter_rl::operator()
(
	Signal wc1, Signal qc1, Signal vc1,
	Signal wc2, Signal qc2, Signal vc2, 
	Signal wa1, Signal qa1, Signal va1,
	Signal wa2, Signal qa2, Signal va2,

	Signal bw1, Signal bq1, Signal fa1, Signal bv1,
	Signal bw2, Signal bq2, Signal fa2, Signal bv2,

	Signal p, 
	Signal clk
)
{
initio

	Input_(wc1, KEYBITS-1, 0); // two best collision tracks (wg, q, v)
	Input_(qc1, QBITS-1, 0);
	Input (vc1);
	
	Input_(wc2, KEYBITS-1, 0);
	Input_(qc2, QBITS-1, 0);
	Input (vc2);

	Input_(wa1, KEYBITS-1, 0); // two best accelerator tracks (wg, q, v)
	Input_(qa1, QBITS-1, 0);
	Input (va1);

	Input_(wa2, KEYBITS-1, 0);
	Input_(qa2, QBITS-1, 0);
	Input (va2);

	OutReg_(bw1, KEYBITS-1, 0); // first selected track, with acc bit
	OutReg_(bq1, QBITS-1, 0);
	OutReg (fa1);
	OutReg (bv1);

	OutReg_(bw2, KEYBITS-1, 0); // second selected track, with acc bit
	OutReg_(bq2, QBITS-1, 0);
	OutReg (fa2);
	OutReg (bv2);
	
	Input (p); // promote collision flag
	Input (clk);

beginmodule

	Reg (c1c2); // comparison results
	Reg (c1a1);
	Reg (c1a2);
	Reg (c2a1);
	Reg (c2a2);
	Reg (a1a2);
		 			 
	Reg_(rc1, 2, 0); // track ranks
	Reg_(rc2, 2, 0);
	Reg_(ra1, 2, 0);
	Reg_(ra2, 2, 0);

	Reg(b1c1); // first best flags
	Reg(b1c2);
	Reg(b1a1);
	Reg(b1a2);

	Reg(b2c1); // second best flags
	Reg(b2c2);
	Reg(b2a1);
	Reg(b2a2);

	Reg_(wc1and1, KEYBITS-1, 0); // AND of track parameters and first best flags
	Reg_(qc1and1, QBITS-1, 0); 
	Reg_(wc2and1, KEYBITS-1, 0); 
	Reg_(qc2and1, QBITS-1, 0);
 					   
	Reg_(wa1and1, KEYBITS-1, 0); 
	Reg_(qa1and1, QBITS-1, 0); 
	Reg_(wa2and1, KEYBITS-1, 0); 
	Reg_(qa2and1, QBITS-1, 0); 
					   
	Reg_(wc1and2, KEYBITS-1, 0); // AND of track parameters and second best flags
	Reg_(qc1and2, QBITS-1, 0); 
	Reg_(wc2and2, KEYBITS-1, 0); 
	Reg_(qc2and2, QBITS-1, 0);
 					   
	Reg_(wa1and2, KEYBITS-1, 0); 
	Reg_(qa1and2, QBITS-1, 0); 
	Reg_(wa2and2, KEYBITS-1, 0); 
	Reg_(qa2and2, QBITS-1, 0); 

	Reg (vc1and1); 
	Reg (vc2and1);  
	Reg (va1and1);  
	Reg (va2and1);  
	Reg (vc1and2); 
	Reg (vc2and2);  
	Reg (va1and2);  
	Reg (va2and2); 

	Reg (vc1r);
	Reg (vc2r);
	Reg (va1r);
	Reg (va2r);

	Reg_ (qc1r, QBITS-1, 0);
	Reg_ (qc2r, QBITS-1, 0);
	Reg_ (qa1r, QBITS-1, 0);
	Reg_ (qa2r, QBITS-1, 0);

	Reg_ (wc1r, KEYBITS-1, 0);
	Reg_ (wc2r, KEYBITS-1, 0);
	Reg_ (wa1r, KEYBITS-1, 0);
	Reg_ (wa2r, KEYBITS-1, 0);

	always (posedge (clk))
	begin
		// compare qualities with each other, when some are equal use promote flag to resolve
		c1c2 = qc1 >= qc2 && vc1 >= vc2; 
		a1a2 = qa1 >= qa2 && va1 >= va2;
		c1a1 = (qc1 > qa1 && !p) || (qc1 >= qa1 && p && vc1 >= va1);
		c1a2 = (qc1 > qa2 && !p) || (qc1 >= qa2 && p && vc1 >= va2);
		c2a1 = (qc2 > qa1 && !p) || (qc2 >= qa1 && p && vc2 >= va1);
		c2a2 = (qc2 > qa2 && !p) || (qc2 >= qa2 && p && vc2 >= va2);

		rc1 = ( c1c2 ,  c1a1 ,  c1a2); // assemble track ranks based on comparison results
		rc2 = (!c1c2 ,  c2a1 ,  c2a2);
		ra1 = ( a1a2 , !c1a1 , !c2a1);
		ra2 = (!a1a2 , !c1a2 , !c2a2);

		vc1r = vc1; // delay all params by one clock to match q comparators results
		vc2r = vc2;
		va1r = va1;
		va2r = va2;

		wc1r = wc1;
		qc1r = qc1;

		wc2r = wc2;
		qc2r = qc2;

		wa1r = wa1;
		qa1r = qa1;

		wa2r = wa2;
		qa2r = qa2;

	end

    always 
	(
		rc1  or rc2  or ra1  or ra2  or
		vc1r or vc2r or va1r or va2r or 
		wc1r or qc1r or wc2r or qc2r or 
		wa1r or qa1r or wa2r or qa2r
	) 
    begin

		
		bw1 = 0; bq1 = 0; fa1 = 0;
		bw2 = 0; bq2 = 0; fa2 = 0;
		
		// best track will have rank 7 (three 1s)
		b1c1 = rc1 == 7; // generate first best flags
		b1c2 = rc2 == 7;	
		b1a1 = ra1 == 7;
		b1a2 = ra2 == 7;

		// second best will have two 1s (rank 6, 5, or 3)
		b2c1 = rc1 == 6 || rc1 == 5 || rc1 == 3; // generate second best flags
		b2c2 = rc2 == 6 || rc2 == 5 || rc2 == 3;
		b2a1 = ra1 == 6 || ra1 == 5 || ra1 == 3;
		b2a2 = ra2 == 6 || ra2 == 5 || ra2 == 3;

		vc1and1 = (b1c1 && vc1r); 
		wc1and1 = ifelse(b1c1 && vc1r , wc1r , 0); // make AND between track parameters and first and second best flags
		qc1and1 = ifelse(b1c1 && vc1r , qc1r , 0); // and take into account valid flags
							 									
		vc2and1 = (b1c2 && vc2r); 
		wc2and1 = ifelse(b1c2 && vc2r , wc2r , 0);
		qc2and1 = ifelse(b1c2 && vc2r , qc2r , 0);
		   		  	   	     		
		va1and1 = (b1a1 && va1r); 
		wa1and1 = ifelse(b1a1 && va1r , wa1r , 0);
		qa1and1 = ifelse(b1a1 && va1r , qa1r , 0);
							 									
		va2and1 = (b1a2 && va2r); 
		wa2and1 = ifelse(b1a2 && va2r , wa2r , 0);
		qa2and1 = ifelse(b1a2 && va2r , qa2r , 0);
		   			   	    
		vc1and2 = (b2c1 && vc1r); 
		wc1and2 = ifelse(b2c1 && vc1r , wc1r , 0);
		qc1and2 = ifelse(b2c1 && vc1r , qc1r , 0);
							 									
		vc2and2 = (b2c2 && vc2r); 
		wc2and2 = ifelse(b2c2 && vc2r , wc2r , 0);
		qc2and2 = ifelse(b2c2 && vc2r , qc2r , 0);
		   		  	   	 	 		  	    
		va1and2 = (b2a1 && va1r); 
		wa1and2 = ifelse(b2a1 && va1r , wa1r , 0);
		qa1and2 = ifelse(b2a1 && va1r , qa1r , 0);
							 									
		va2and2 = (b2a2 && va2r); 
		wa2and2 = ifelse(b2a2 && va2r , wa2r , 0);
		qa2and2 = ifelse(b2a2 && va2r , qa2r , 0);

		// OR all first and second best, and let the winner win.
		bv1 = vc1and1 | vc2and1 | va1and1 | va2and1;
		bw1 = wc1and1 | wc2and1 | wa1and1 | wa2and1;
		bq1 = qc1and1 | qc2and1 | qa1and1 | qa2and1;
		fa1 = b1a1 || b1a2;

		bv2 = vc1and2 | vc2and2 | va1and2 | va2and2;
		bw2 = wc1and2 | wc2and2 | wa1and2 | wa2and2;
		bq2 = qc1and2 | qc2and2 | qa1and2 | qa2and2;
		fa2 = b2a1 || b2a2;

    end

endmodule
}

