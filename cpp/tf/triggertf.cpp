#include "trigger.h"

 Signal ly0p; 
 Signal ly1p; 
 Signal ly2p; 
 Signal ly3p; 
 Signal ly4p; 
 Signal ly5p;

Signal collmask;                          
Signal HCmask;                            
Signal PromoteColl;                       

Signal hp; 
Signal hnp; 
Signal hfap; 
Signal hpatbp;             

Signal lp; 
Signal lnp; 
Signal lfap; 
Signal lpatbp; 
            
Signal drifttime; 
Signal pretrig; 
Signal trig; 
Signal trig_mode;
Signal input_disp;                         
Signal actv_feb_fg;
Signal clk;

trigger trigger_;
#define LAT 10

int main()
{
	int k;
	int w1[LAT+1], w2[LAT+1];
	collmask = 0;
	collmask = ~collmask;

	HCmask = 0;
	HCmask = ~HCmask;

	PromoteColl = 0;
	drifttime = 3;
	pretrig	  = 2;
	trig      = 3;
	trig_mode = 2;
	input_disp= 0;

	char lyt[120] = "112'b";
	lyt[118] = 0;
	char lye[120] = "112'b";
	lye[118] = 0;
	for (k = 0; k < 112; k++) lye[k+5] = '0';
	for (k = 0; k < LAT+1; k++)
	{
		w1[k] = 0;
		w2[k] = 0;
	}


	for (int i = 0; i < 1000; i++)
	{

		for (k = 0; k < LAT; k++)
		{
			w1[k] = w1[k+1];
			w2[k] = w2[k+1];
		}
		for (k = 0; k < 112; k++) lyt[k+5] = '0';
		w1[LAT] = rand() % 112;
		w2[LAT] = rand() % 112;
		lyt[w1[LAT]+5] = '1';		
		lyt[w2[LAT]+5] = '1';
		ly0p = (rand()&1) ? lyt : lye;		
		ly1p = (rand()&1) ? lyt : lye;		
		ly2p = (rand()&1) ? lyt : lye;		
		ly3p = (rand()&1) ? lyt : lye;		
		ly4p = (rand()&1) ? lyt : lye;		
		ly5p = (rand()&1) ? lyt : lye;		



		for (int j = 0; j < 2; j++)
		{
			clk = j;
			beginsystem
				
			ly0p.wire(LYWG-1, 0, "ly0p"); 
			ly1p.wire(LYWG-1, 0, "ly1p"); 
			ly2p.wire(LYWG-1, 0, "ly2p"); 
			ly3p.wire(LYWG-1, 0, "ly3p"); 
			ly4p.wire(LYWG-1, 0, "ly4p"); 
			ly5p.wire(LYWG-1, 0, "ly5p");
			collmask.wire(COLLMASKBITS-1, 0, "collmask");
			HCmask.wire(HCMASKBITS-1, 0, "HCmask");
			PromoteColl.wire("PromoteColl");
			
			hp.wire(QBITS-1, 0, "hp");
			hnp.wire(KEYBITS-1, 0, "hnp"); 
			hfap.wire("hfap"); 
			hpatbp.wire("hpatbp");
			
			lp.wire(QBITS-1, 0, "lp"); 
			lnp.wire(KEYBITS-1, 0, "lnp"); 
			lfap.wire("lfap"); 
			lpatbp.wire("lpatbp");
			
			drifttime.wire(2, 0, "drifttime");
			pretrig.wire(2, 0, "pretrig");
			trig.wire(2, 0, "trig");
			trig_mode.wire(1, 0, "trig_mode");
			input_disp.wire("input_disp");
			Wire (actv_feb_fg);
			clk.wire("clk");

			trigger_.init("trigger", "trigger_");
			trigger_
			(
				ly0p, ly1p, ly2p, ly3p, ly4p, ly5p,
				collmask,                          
				PromoteColl,                       
				hp, hnp, hfap, hpatbp,             
				lp, lnp, lfap, lpatbp,             
				drifttime, pretrig, trig, trig_mode,
				actv_feb_fg,
				clk
			);

//			cout << "*";
			endsystem
//			cout << "\n";
		}
//		cout << hex << ly0p << " " << trigger_.ly0i << " " << trigger_.ly0 << "\n";
//		cout << trigger_.ExtendPulses.lsh[0].sh[3].lyd << " ";
//		cout << trigger_.ExtendPulses.lsh[0].sh[3].sri << " ";
//		cout << trigger_.ExtendPulses.lsh[0].sh[3].lys << "\n";

		cout << hex;

//		cout << trigger_.FindPatterns.pt[3].sumac << " " << trigger_.FindPatterns.pt[3].sac << " ";
//		cout << trigger_.FindPatterns.pt[3].scl << " " << trigger_.FindPatterns.pt[3].scll << "\n";
//		cout << trigger_.FindPatterns.pt[3].ly0am << " ";
//		cout << trigger_.FindPatterns.pt[3].ly1am << " ";
//		cout << trigger_.FindPatterns.pt[3].ly2am << " ";
//		cout << trigger_.FindPatterns.pt[3].ly3am << " ";
//		cout << trigger_.FindPatterns.pt[3].ly4am << " ";
//		cout << trigger_.FindPatterns.pt[3].ly5am << "\n";

//		cout << trigger_.hitc << "\n";
//		cout << trigger_.hita << "\n";
//		cout << trigger_.hitct << "\n";
//		cout << trigger_.hitat << "\n";

//		cout << trigger_.GetBestPattern.bestc << " " << trigger_.GetBestPattern.bestcn << "\n";
//		cout << trigger_.best1 << " " << trigger_.best1n << "\n";
//		cout << trigger_.bestqc1 << "\n";
//		cout << trigger_.GetBestPattern.prom1.mode << "\n";


//		cout << trigger_.best1 << " " << trigger_.best1n << " " << trigger_.fa1 << "\n";
//		cout << trigger_.best2 << " " << trigger_.best2n << " " << trigger_.fa2 << "\n";

//		cout << trigger_.hitc << " " << trigger_.hitct << "\n";
//		cout << trigger_.KillNeighbours.chn[23].cntcn << " "  
//			 << trigger_.KillNeighbours.chn[22].cntcn << " "  
//			 << trigger_.KillNeighbours.chn[21].cntcn << " "  
//			 << trigger_.KillNeighbours.chn[20].cntcn << "\n";

		cout << 111-w1[0] << " " << 111-w2[0] << "\t";

		cout << hp << " " << hnp << " " << /*hfap << " " << hpatbp << */"     ";
		cout << lp << " " << lnp << " " << /*lfap << " " << lpatbp << */"\n";
	}
	return 0;
}

//			ly0p = "112'h0000_0000_0000_0000_0000_0040_0000";
//			ly1p = "112'h0000_0000_0000_0000_0000_0040_0000";
//			ly2p = "112'h0000_0000_0000_0000_0000_0080_0000";
//			ly3p = "112'h0000_0000_0000_0000_0000_0080_0000";
//			ly4p = "112'h0000_0000_0000_0000_0000_0080_0000";
//			ly5p = "112'h0000_0000_0000_0000_0000_0000_0000";
