#include "patterner_rl.h"
#include "tfmac.h"

	Signal ly0; Signal ly1; Signal ly2; Signal ly3; Signal ly4; Signal ly5;
	Signal collmask; 
	Signal sacp;
	Signal sbcp;
	Signal sa;
	Signal drifttime; Signal pretrig; Signal trig; Signal trig_mode; 
	Signal clk;

Patterner_rl patterner_;

int main()
{
	int i;
	SIMULATE_TQ
	{
		FIRST_TQ;
		NEXT_TQ collmask = "28'b11111111111111111111_11111111111111111110", drifttime = 3, pretrig = 2, trig = 3, trig_mode = 0;
		NEXT_TQ ly0 = 1, ly1 = 0, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 
		NEXT_TQ ly0 = 1, ly1 = 1, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 
		NEXT_TQ ly0 = 1, ly1 = 1, ly2 = 1, ly3 = 2, ly4 = 4, ly5 = 4; 
		NEXT_TQ ly0 = 1, ly1 = 1, ly2 = 1, ly3 = 2, ly4 = 4, ly5 = 4; 
		NEXT_TQ ly0 = 1, ly1 = 1, ly2 = 1, ly3 = 2, ly4 = 4, ly5 = 4; 
		NEXT_TQ ly0 = 0, ly1 = 1, ly2 = 1, ly3 = 2, ly4 = 4, ly5 = 4; 
		NEXT_TQ ly0 = 0, ly1 = 0, ly2 = 1, ly3 = 2, ly4 = 4, ly5 = 4; 
		NEXT_TQ ly0 = 0, ly1 = 0, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 
		SKIP_TQ(3);
		NEXT_TQ ly0 = 4, ly1 = 0, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 
		NEXT_TQ ly0 = 4, ly1 = 0, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 
		NEXT_TQ ly0 = 4, ly1 = 0, ly2 = 1, ly3 = 1, ly4 = 1, ly5 = 1; 
		NEXT_TQ ly0 = 4, ly1 = 0, ly2 = 1, ly3 = 1, ly4 = 1, ly5 = 1; 
		NEXT_TQ ly0 = 4, ly1 = 0, ly2 = 1, ly3 = 1, ly4 = 1, ly5 = 1; 
		NEXT_TQ ly0 = 0, ly1 = 0, ly2 = 1, ly3 = 1, ly4 = 1, ly5 = 1; 
		NEXT_TQ ly0 = 0, ly1 = 0, ly2 = 1, ly3 = 1, ly4 = 1, ly5 = 1; 
		NEXT_TQ ly0 = 0, ly1 = 0, ly2 = 0, ly3 = 0, ly4 = 0, ly5 = 0; 

		for (i = 0; i < 2; i++)
		{
			clk = i;
			beginsystem

			Wire_(ly0, 2, 0);
			Wire_(ly1, 1, 0);
			Wire (ly2); 
			Wire_(ly3, 1, 0);
			Wire_(ly4, 2, 0); 
			Wire_(ly5, 2, 0);
			Wire_(collmask, 27, 0);
			Wire_(sacp, 1, 0); // pattern a quality
			Wire_(sbcp, 1, 0); // pattern b quality
			Wire_(sa, 1, 0); // acc muon quality
			Wire_(drifttime, 2, 0);
			Wire_(pretrig, 2, 0);
			Wire_(trig, 2, 0);
			Wire_(trig_mode, 1, 0);
			Wire (clk);
			
			patterner_.init("patterner_rl", "patterner_rl_");
			patterner_
			(
	 			ly0,  ly1,  ly2,  ly3,  ly4,  ly5,
	 			collmask, 
	 			sacp,
	 			sbcp,
	 			sa,
	 			drifttime,  pretrig,  trig,  trig_mode, 
	 			clk
			);


			endsystem
		}		
		PRINT_TQ;
		cout << ly0 << ly1 << ly2 << ly3 << ly4 << ly5 << ptab 
			 << sacp << ptab << sbcp << ptab << sa << ptab
			 << patterner_.bxac << patterner_.bxbc << ptab
			 << patterner_.sumac << patterner_.sumbc << ptab  
			 << patterner_.sac << patterner_.sbc << ptab 
			<< patterner_.sacll << patterner_.sbcll << endl;
	}
}
