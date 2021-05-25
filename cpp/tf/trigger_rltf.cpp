#ifdef TRIGWP
#include "trigger_wp.h"
#else
#include "trigger_rl.h"
#endif

#include "tfmac.h"

Signal ly0p; Signal ly1p; Signal ly2p; Signal ly3p; Signal ly4p; Signal ly5p;
Signal collmask;                          
Signal PromoteColl;                       
Signal hp; Signal hnp; Signal hfap; Signal hpatbp, hv;             
Signal lp; Signal lnp; Signal lfap; Signal lpatbp, lv;             
Signal drifttime; Signal pretrig; Signal trig; Signal trig_mode;
Signal acc_pretrig, acc_trig;
Signal actv_feb_fg;
Signal trig_stop;
Signal clk;

#ifdef TRIGWP
trigger_wp trl;
#else
trigger_rl trl;
#endif

#define LAT 5
#define MAX_WG LYWG // should be 112
#define WG_SHIFT 4
#define WG_APART 3
#define EMPTY_BXS 7

int main ()
{
	int i, j, ii;
//	int w1[LAT+1], w2[LAT+1];
	collmask = 0;
	collmask = ~collmask;

	PromoteColl = 1;
	drifttime = 3;
	pretrig	  = 2;
	trig      = 3; // nph_trigger = 4
	trig_mode = 0;

// trig_mode:
// 0 - coll & acc
// 1 - acc
// 2 - coll
// 3 - acc kills coll

	acc_pretrig = 2;
	acc_trig = 3;
	trig_stop = 0;
/*
	char lyt[120] = "112'b";
	lyt[117] = 0;
	char lye[120] = "112'b";
	lye[117] = 0;
	for (k = 0; k < 112; k++) lye[k+5] = '0';
	for (k = 0; k < LAT+1; k++)
	{
		w1[k] = 0;
		w2[k] = 0;
	}
*/

	for (i = ii = 0; i < 15; i++)	
	{

// the commented code fragment below is cycling thru all wiregroups
/*		for (k = 0; k < LAT; k++)
		{
			w1[k] = w1[k+1];
			w2[k] = w2[k+1];
		}
		for (k = 0; k < 112; k++) lyt[k+5] = '0';

		if (i % (EMPTY_BXS+1) == 0)
		{
			w1[LAT] = (ii*WG_SHIFT)   % MAX_WG; //rand() % MAX_WG;
			w2[LAT] = (ii*WG_SHIFT+WG_APART) % MAX_WG; //rand() % MAX_WG;
			lyt[116-w1[LAT]] = '1';		
//			lyt[116-w2[LAT]] = '1';
			ii++;
		}
		ly0p = lyt;
		ly1p = lyt;
		ly2p = lyt;
		ly3p = lyt;
		ly4p = lyt;
		ly5p = lyt;
*/

		// Slava's patterns from 5/2/08, give ghosts (?)

		if (i == 1)
		{
			ly0p = "64'b0000000000000000000000000000000000000000000000000000000000000000";
			ly1p = "64'b0000000000000000000000000000000000000000000000000000000000000000"; 
			ly2p = "64'b0000000000000000000000000000000000000000001000000000000000000000"; 
			ly3p = "64'b0000000000000000000000000000000000000000001000000000000000000000"; 
			ly4p = "64'b0000000000000000000000000000000000000000100000000000000000000000"; 
			ly5p = "64'b0000000000000000000000000000000000000001100000000000000000000000";
		}
		else if (i == 1)
		{
			ly0p = "64'b0000000000000000000000000000000000000000000001100000000000000000"; 
			ly1p = "64'b0000000000000000000000000000000000000000000010000000000000000000"; 
			ly2p = "64'b0000000000000000000000000000000000000000000100000000000000000000"; 
			ly3p = "64'b0000000000000000000000000000000000000000010000000000000000000000"; 
			ly4p = "64'b0000000000000000000000000000000000000000000000000000000000000000"; 
			ly5p = "64'b0000000000000000000000000000000000000000000000000000000000000000";
		}
/*
		if (i == 1)
		{
			ly0p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
			ly1p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
			ly2p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
			ly3p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
			ly4p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
			ly5p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000";
		}
		else if (i == 2)
		{
			ly0p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
			ly1p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000";
			ly2p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000";
			ly3p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000";
			ly4p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000";
			ly5p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000";
		}
		else if (i == 3)
		{
			ly0p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
			ly1p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
			ly2p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
			ly3p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001";
			ly4p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001";
			ly5p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001";
		}
		else if (i == 4)
		{
			ly0p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00010000";
			ly1p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000_00000000_00000000_00010000";
			ly2p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000_00000000_00000000_00010000";
			ly3p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000_00000000_00000000_00010000";
			ly4p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000_00000000_00000000_00010000";
			ly5p = "112'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001_00000000_00000000_00000000_00000000_00010000";
		}
		else if (i == 21)
		{
		}*/

		for (j = 0; j < 2; j++)
		{
			clk = j;
			beginsystem
			
			Wire_(ly0p, LYWG-1, 0); 
			Wire_(ly1p, LYWG-1, 0); 
			Wire_(ly2p, LYWG-1, 0); 
			Wire_(ly3p, LYWG-1, 0); 
			Wire_(ly4p, LYWG-1, 0); 
			Wire_(ly5p, LYWG-1, 0);
			Wire_(collmask, COLLMASKBITS-1, 0);
			Wire (PromoteColl);

			Wire_(hp, QBITS-1, 0); // registered outputs, for DAQ FIFO
			Wire_(hnp, KEYBITS-1, 0); 
			Wire (hfap); 
			Wire (hpatbp);
			Wire (hv);

			Wire_(lp, QBITS-1, 0); 
			Wire_(lnp, KEYBITS-1, 0); 
			Wire (lfap); 
			Wire (lpatbp);
			Wire (lv);

			Wire_(drifttime, 2, 0);
			Wire_(pretrig, 2, 0);
			Wire_(trig, 2, 0);
    		Wire_(trig_mode, 1, 0);
			Wire_(acc_pretrig, 2, 0);
			Wire_(acc_trig, 2, 0);
			Wire (actv_feb_fg);
			Wire (trig_stop);
			Wire (clk);

			trl.init("trigger_rl", "trl");
			trl
			(
				ly0p, ly1p, ly2p, ly3p, ly4p, ly5p,
			  	collmask,                          
				PromoteColl,                       
				hp, hnp, hfap, hpatbp, hv,            
				lp, lnp, lfap, lpatbp, lv,            
				drifttime, pretrig, trig, trig_mode,
				acc_pretrig, acc_trig,
				actv_feb_fg,
				trig_stop,
				clk
	
			);


			endsystem
		}
//   		cout << lyt << "    ";
//		cout << trl.ly0 << "  ";
		cout << hex;
//		cout << ly0p << "   ";
		cout << trl.FindPatterns.chn[20].cntcn << "   ";

        cout << trl.FindPatterns.vac(20) << "   ";
        cout << trl.FindPatterns.hitca(41,40) << "   ";
        cout << trl.FindPatterns.vac(19) << "   ";
        cout << trl.FindPatterns.hitca(39,38) << "   ";
        cout << trl.FindPatterns.vac(21) << "   ";
        cout << trl.FindPatterns.hitca(43,42) << "   ";
		cout << hv << " " << hp << " " << hnp << " " << hfap << " " << hpatbp << "     ";
		cout << lv << " " << lp << " " << lnp << " " << lfap << " " << lpatbp << "\n";

// two lines below only for WP version with single patterner inside
//		cout << trl.patterner.sumac << "  " << trl.patterner.suma << "   ";
//		cout << trl.patterner.bxac << "  " << trl.patterner.bxa << endl;

//		cout << trl.FindPatterns.va << "   " << trl.FindPatterns.vai << endl;
//		cout << trl.qa0 << "  "  << trl.qa1 << "  " << trl.qa2 << "  " << trl.qa3 << endl;
//		cout << trl.bwc1w << " " << trl.bwc2w << " " << trl.bwa1w << " " << trl.bwa2w << endl;
//		cout << trl.bv1 << " " << trl.bq1 << " " << trl.bv2 << " " << trl.bq2 << endl;
//		cout << trl.pr.rc1 << trl.pr.rc2 << trl.pr.ra1 << trl.pr.ra2 << "   ";
//		cout << trl.pr.b1a1 << trl.pr.b1a2 << trl.pr.b2a1 << trl.pr.b2a2 << endl;
//		cout << trl.FindPatterns.chn[3].cntcn << trl.FindPatterns.chn[3].cntan << endl;

/*		for (int pi = 0; pi < LYWG; pi++)
		{
			cout << trl.FindPatterns.chn[pi].er;
		}
		cout << endl;*/


	}
}

