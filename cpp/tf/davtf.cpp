#include "dav.h"

Signal l1a;
Signal l1a_num;
Signal valid;
Signal l1a_window;
Signal davalid;
Signal clk;


dav dav_;

int main()
{
	int i, j;

	for (j = 0; j < 30; j++)
	{
		
		l1a = j == 1 || j == 3;
		valid = j == 3;
		l1a_window = 3;
		
		
		for (i = 0; i < 2; i++)
		{
			clk = i;
			
			beginsystem
				
				Wire (l1a);
				Wire (valid);
				Wire_ (l1a_num, 7, 0);
				Wire (l1a_window);
				Wire (davalid);
				Wire (clk);
				
				dav_.init("dav", "dav_");
				dav_
				(	
					l1a,       
					l1a_num,
					valid,     
					l1a_window,
					davalid,   
					clk
				);
				
//				cout << "*";
			endsystem

		}
		cout << dec << j << " " << l1a << " " << valid << " " << davalid << " " << hex;
		cout << "\n";
	}
	return 0;
	
}
