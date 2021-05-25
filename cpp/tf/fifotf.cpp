#include "fifo.h"

Signal ly0; Signal ly1; Signal ly2; Signal ly3; Signal ly4; Signal ly5;   // layer data
Signal best1;              			// best track parameters 
Signal best2;              			// second best track parameters
Signal bxn;                            // bx number

Signal fifo_tbins;                     // length of the dump for one L1A
Signal daq;                            // output to daq (including all service bits) 
Signal l1a_delay;                       // time back to get the raw hits corresponding to this to L1A
Signal fifo_pretrig;                     // delay of the input raw hit data to compensate for the processing and pretriggering
Signal fifo_mode;                      // bits from configuration register defining the raw hits dump mode
Signal BoardID; 
Signal CSCID; 
Signal L1A;
Signal hard_rst;
Signal l1a_internal; Signal l1a_window; Signal l1a_offset;
Signal WaitForSpaceInOutFIFO;
Signal L1AWindow; Signal l1aTP; Signal validhd;
Signal send_empty;
Signal bxn_before_reset;
Signal clk;


fifo fifo_;

int l1ahit[] = 
{
	0xCEB, 
	0x5DE,
	0x060,
	0x10B,
	0xC7B,
	0x563,
	0x13C,
	0x955,
	0xC14,
	0x390,
	0x78C,
	0x239,
	0x80B,
	0x583,
	0xB73,
	0x60B,
	0x4E9,
	0x287,
	0xDD4,
	0x4C5,
	0xCDB,
	0x068,
	0x5d0,
	0x9d7,
	0 // 0 is end marker
};

int lhsize = sizeof(l1ahit)/sizeof(int);

int main()
{
	// assign the input data to static input signals

	fifo_tbins = 1;  
	l1a_delay = 144;   
	fifo_pretrig = 10;
	fifo_mode = 1;   
	BoardID = 0; 
	CSCID = 0; 
	l1a_internal = 0; 
	l1a_window = 5; 
	l1a_offset = 1;
	WaitForSpaceInOutFIFO = 0;
	send_empty = 0;

//	int l1hn = 0; // l1a hit number

	for (int i = 0; i < 2000; i++)
	{

		L1A = 0;
		if (i == 0) hard_rst = 0; else hard_rst = 1;
		best1 = 0;
		best2 = 0;
		bxn = i;


		ly0 = 0;
		ly1 = 0;
		ly2 = 0;
		ly3 = 0;
		ly4 = 0;
		ly5 = 0;

/*		if (l1ahit[l1hn] == (i & 0xfff))
		{
			L1A = 1;
			cout << "########################## L1A bxn = " << hex << l1ahit[l1hn] << endl;
			if (l1ahit[l1hn] == 0) break; // end of L1As reached
			l1hn++;
		}
	*/

		if (i > 1 && i < 40 && i%2 == 0 && i != 20)
		{
			best1 = 3;
		}	
		if (i > 144+1 && i < 144+40 && i%2 == 0)
		{
		    L1A = 1;
		}	

//		if (i == 144-2)	L1A = 1;
//		if (i == 144+0)	L1A = 1;
//		if (i == 144+1)	L1A = 1;
//		if (i == 144+2)	L1A = 1;
//		if (i == 144+3)	L1A = 1;
//		if (i == 144+4)	L1A = 1;
//		if (i == 144+5)	L1A = 1;
//		if (i == 144+6)	L1A = 1;


		for (int j = 0; j < 2; j++)
		{
			clk = j;
			beginsystem
				
			ly0.wire(LYWG-1, 0, "ly0");
			ly1.wire(LYWG-1, 0, "ly1");
			ly2.wire(LYWG-1, 0, "ly2");
			ly3.wire(LYWG-1, 0, "ly3");
			ly4.wire(LYWG-1, 0, "ly4");
			ly5.wire(LYWG-1, 0, "ly5");
			
			best1.wire (BESTBITS-1, 0, "best1"); 
			best2.wire (BESTBITS-1, 0, "best2"); // {key, patb, amu, quality}
			bxn.wire (11, 0, "bxn");
			fifo_tbins.wire (4, 0, "fifo_tbins");
			daq.wire (18, 0, "daq");
			l1a_delay.wire (7, 0, "l1a_delay"); 
			fifo_pretrig.wire (4, 0, "fifo_pretrig");
			fifo_mode.wire (1, 0, "fifo_mode");
			BoardID.wire (2, 0, "BoardID");
			CSCID.wire (3, 0, "CSCID");
			L1A.wire ("L1A"); 
			hard_rst.wire ("hard_rst");
			l1a_internal.wire ("l1a_internal"); // if ==1 then L1A will be generated internally for each valid track found
			l1a_window.wire (3, 0, "l1a_window");   // window in which to look for valid tracks in case of L1A
			l1a_offset.wire (3, 0, "l1a_offset");   // initial value for l1a counter
			WaitForSpaceInOutFIFO.wire ("WaitForSpaceInOutFIFO"); // this is used in debugging mode, when there is not enough space in the wire FIFO
			L1AWindow.wire ("L1AWindow");
			l1aTP.wire ("l1aTP");
			Wire (validhd);
			Wire (send_empty);
			Wire_(bxn_before_reset, MXBXN-1, 0);
			clk.wire ("clk");
			
			fifo_.init("fifo", "fifo_");
			fifo_
			(
				ly0, ly1, ly2, ly3, ly4, ly5,   // layer data
				best1,              			// best track parameters 
				best2,              			// second best track parameters
				bxn,                            // bx number
				fifo_tbins,                     // length of the dump for one L1A
				daq,                            // output to daq (including all service bits) 
				l1a_delay,                       // time back to get the raw hits corresponding to this to L1A
				fifo_pretrig,                     // delay of the input raw hit data to compensate for the processing and pretriggering
				fifo_mode,                      // bits from configuration register defining the raw hits dump mode
				BoardID, 
				CSCID, 
				L1A,
				hard_rst,
				l1a_internal, l1a_window, l1a_offset,
				WaitForSpaceInOutFIFO,
				L1AWindow, l1aTP, validhd,
				send_empty,
				bxn_before_reset,
				(Signal)"3'b4", (Signal)"3'b4",
				clk
			);
			
//			cout << "*";
			endsystem
//			cout << "\n";
		}
		cout << hex;
//		cout << L1A << " " << L1AWindow << " " << fifo_.L1AoneShot << " " << fifo_.L1Aempty << " " << fifo_.L1Astate << " ";
//		cout << fifo_.bestf.waddr << " " << fifo_.bestf.raddr << " " << fifo_.best1d <<  " " << fifo_.best1f << " ";
		cout << fifo_.best1d << " " << L1AWindow << " " << fifo_.L1Anumber << "\t";
		cout << fifo_.davgen.l1a_sh[14] << " " << fifo_.davgen.l1a_sh[0] << "\t";
		cout << fifo_.davgen.l1ad << " " << fifo_.davgen.l1a_numd << "\t";
		cout << fifo_.davgen.davmem << "\t";
		cout << fifo_.davv << "\t" << fifo_.best1w;
		cout << "\t" << daq << "\n";

	}
	return 0;
}
