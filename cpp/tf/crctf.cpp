#include "crc.h"


unsigned long crc22(unsigned short int *d, int length)
{
    int i, j, t;
    unsigned long crc = 0, ncrc;
    unsigned short w;

    for (j = 0; j < length; j++)
    {
        w = d[j] & 0x7fff;
        for (i = 15; i >= 0; i--)
        {
            t = ((w >> i) & 1) ^ ((crc >> 21) & 1);
            ncrc = (crc << 1) & 0x3ffffc;
            ncrc |= (t ^ (crc & 1)) << 1;
            ncrc |= t;
            crc = ncrc;
        }
    }
    return crc;
}


Signal d;
Signal crcr;
Signal calc;
Signal dav;
Signal clk;


crc crc_;

unsigned short data[100];

int main()
{
	int i, j;

	for (j = 0; j < 55; j++)
	{
		
		if (j == 0)
		{
			calc = 0;
			d = 0x5555;
		}
		else if (j == 1)
		{
			calc = 0;
			d = 0xaaaa;
		}
		else if (j < 50)
		{
			calc = 1;
			d = j+10000;
			data[j-2] = j+10000;
		}
		else if (j == 50)
		{
			calc = 2;
		}
		else if (j == 51)
		{
			calc = 3;
		}
		else if (j > 51)
		{
			calc = 0;
		}


		for (i = 0; i < 2; i++)
		{
			clk = i & 1;
			
			beginsystem
				
				Wire_(d, 18, 0);
				Wire_(crcr, 18, 0);
				Wire_(calc, 1, 0);
				Wire (dav);
				Wire (clk);
				

				crc_.init("crc", "crc_");
				crc_
				(		
					d,
					crcr,
					calc,
					dav,
					clk
				);
				
//				cout << "*";
			endsystem
//			cout << "\n";
		}
		cout << hex << calc << " " << d << hex << " " << crcr << " " << crc_.ncrc << "\n";
	}
	cout << crc22(data, 47) << "\n";
	cout << crc22(data, 48) << "\n";
	return 0;
}
