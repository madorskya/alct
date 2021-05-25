#include "collider.h"
#include "tfmac.h"

Signal qp1; Signal qp2; Signal qp3; 
Signal w1; Signal w2; 
Signal q1; Signal q2; 
Signal clk;

collider c;

int main()
{
	int i;
	SIMULATE_TQ
	{
		FIRST_TQ;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 4, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 0x44, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 8, qp2 = 4, qp3 = 0;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 16;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 17;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 32, qp2 = 8, qp3 = 2;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		NEXT_TQ qp1 = 0, qp2 = 0, qp3 = 0;
		

//		for (i = 0; i < 2; i++)
//		{
//			clk = i;
			beginsystem

			Wire_(qp1, LYWG-1, 0); // quality positional codes
			Wire_(qp2, LYWG-1, 0);
			Wire_(qp3, LYWG-1, 0);
	
			Wire_(w1, KEYBITS-1, 0); // wiregroups for 2 best tracks
			Wire_(w2, KEYBITS-1, 0);

			Wire_(q1, QBITS-1, 0); // qualities for 2 best tracks
			Wire_(q2, QBITS-1, 0);

			Wire (clk);
			
			c.init("collider","c");
			c
			(
				qp1, qp2, qp3, 
				w1, w2, 
				q1, q2, 
				clk
 			);
//			cout << "*";
			endsystem
//				cout << endl;
//		}

		PRINT_TQ;
			cout << hex << w1 << ptab << w2 << ptab << q1 << ptab << q2 << ptab 
			 << c.n3_1 << ptab << c.n3_2 << ptab << c.n2_1 << ptab << c.n2_2 << ptab << c.n1_1 << ptab << c.n1_2 << endl;

	}
}
