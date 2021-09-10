#include "promoter_rl.h"
#include "tfmac.h"

Signal wc1; Signal qc1; 
Signal wc2; Signal qc2;
Signal wa1; Signal qa1; 
Signal wa2; Signal qa2;
Signal bw1; Signal bq1; Signal fa1;
Signal bw2; Signal bq2; Signal fa2;
Signal p;

promoter_rl pr;

int main()
{
	SIMULATE_TQ
	{
		FIRST_TQ;

		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 5, qc1 = 3, wc2 = 0, qc2 = 0, wa1 = 9, qa1 = 3, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 1, wc1 = 5, qc1 = 3, wc2 = 0, qc2 = 0, wa1 = 9, qa1 = 3, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 5, qc1 = 3, wc2 = 0, qc2 = 0, wa1 = 9, qa1 = 2, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;
		NEXT_TQ p = 0, wc1 = 0, qc1 = 0, wc2 = 0, qc2 = 0, wa1 = 0, qa1 = 0, wa2 = 0, qa2 = 0;

		beginsystem

		Wire_(wc1, KEYBITS-1, 0); // two best collision tracks (wg and q)
		Wire_(qc1, QBITS-1, 0);
		Wire_(wc2, KEYBITS-1, 0);
		Wire_(qc2, QBITS-1, 0);

		Wire_(wa1, KEYBITS-1, 0); // two best accelerator tracks (wg and q)
		Wire_(qa1, QBITS-1, 0);
		Wire_(wa2, KEYBITS-1, 0);
		Wire_(qa2, QBITS-1, 0);

		Wire_(bw1, KEYBITS-1, 0); // first selected track, with acc bit
		Wire_(bq1, QBITS-1, 0);
		Wire (fa1);

		Wire_(bw2, KEYBITS-1, 0); // second selected track, with acc bit
		Wire_(bq2, QBITS-1, 0);
		Wire (fa2);
	
		Wire (p); // promote collision flag

		pr.init("promoter_rl", "pr");
		pr
		(
			wc1, qc1, 
			wc2, qc2,
			wa1, qa1, 
			wa2, qa2,
			bw1, bq1, fa1,
			bw2, bq2, fa2,
			p
		);
		
		endsystem
		PRINT_TQ;
		cout <<	bw1 << ptab << bq1 << ptab << fa1 << ptab << bw2 << ptab << bq2 << ptab << fa2 << endl;

	}
}
