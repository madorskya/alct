#include "stage1_rl.h"
#include "tfmac.h"

Signal ly0; Signal ly1; Signal ly2; Signal ly3; Signal ly4; Signal ly5;
Signal collmask;
Signal drifttime; Signal pretrig; Signal trig; Signal trig_mode;
Signal clk;
Signal qca1; Signal qca2; Signal qca3; 
Signal qa1; Signal qa2; Signal qa3; 

Stage1_rl s;

int main()
{

	int i;
	SIMULATE_TQ
	{
		FIRST_TQ;
		NEXT_TQ collmask = "28'b11111111111111111111_11111111111111111111", drifttime = 3, pretrig = 2, trig = 3, trig_mode = 0;
		NEXT_TQ ly0 = "112'b00000", ly1 = "112'b00000", ly2 = "112'b00000", ly3 = "112'b00000", ly4 = "112'b00000", ly5 = "112'b00000";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b0010", ly1 = "112'b00100", ly2 = "112'b00100", ly3 = "112'b00100", ly4 = "112'b00100", ly5 = "112'b00100";
		NEXT_TQ ly0 = "112'b00000", ly1 = "112'b00000", ly2 = "112'b00000", ly3 = "112'b00000", ly4 = "112'b00000", ly5 = "112'b00000";
		SKIP_TQ(5);
		for (i = 0; i < 2; i++)
		{
			clk = i;
		
			beginsystem
			
			Wire_(ly0,LYWG-1, 0);
			Wire_(ly1,LYWG-1, 0);
			Wire_(ly2,LYWG-1, 0);
			Wire_(ly3,LYWG-1, 0);
			Wire_(ly4,LYWG-1, 0);
			Wire_(ly5,LYWG-1, 0);
    		Wire_(collmask, COLLMASKBITS-1, 0);
			Wire_(drifttime, 2, 0);
			Wire_(pretrig, 2, 0);
			Wire_(trig, 2, 0);
			Wire_(trig_mode, 1, 0);
			Wire_(qca1, LYWG-1, 0);
			Wire_(qca2, LYWG-1, 0);
			Wire_(qca3, LYWG-1, 0);
			Wire_(qa1,  LYWG-1, 0);
			Wire_(qa2,  LYWG-1, 0);
			Wire_(qa3,  LYWG-1, 0);
      		Wire (clk);
			
			s.init("Stage1_rl", "Pattern_GCL");
			s
			(
    			ly0, ly1, ly2, ly3, ly4, ly5,
				collmask,
				drifttime, pretrig, trig, trig_mode,
				qca1, qca2, qca3, 
			   	qa1, qa2, qa3, 
    			clk
			);


			endsystem
		}
		cout << hex << s.hitcai << ptab << s.hitai << ptab << qca1 << " " << qca2 << " " << qca3 << " " << ptab << qa1 << " " << qa2 << " " << qa3 << " " << endl;
	}
}
