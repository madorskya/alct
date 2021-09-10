#include "adaq.h"

Signal ly0; Signal ly1; Signal ly2; Signal ly3; Signal ly4; Signal ly5;   // layer data
Signal best1;              			// best track parameters 
Signal best2;              			// second best track parameters
Signal bxn;                            // bx number

Signal fifo_tbins;                     // length of the dump for one L1A
Signal daqp;                            // output to daq (including all service bits) 
Signal l1a_delay;                       // time back to get the raw hits corresponding to this to L1A
Signal fifo_pretrig;                     // delay of the input raw hit data to compensate for the processing and pretriggering
Signal fifo_mode;                      // bits from configuration register defining the raw hits dump mode
Signal L1A;
Signal hard_rst;
Signal l1a_internal; Signal l1a_window; Signal l1a_offset;
Signal L1AWindow; Signal l1aTP; Signal validhd;
Signal send_empty;
Signal config_report;
Signal bxn_before_reset;
Signal virtex_id;
Signal trig_reg;
Signal config_reg;
Signal hot_channel_mask;
Signal collision_mask;
Signal clk;


adaq fifo_;


int main()
{
	// assign the input data to static input signals

	fifo_tbins = 10;  
	l1a_delay = 200;   
	fifo_pretrig = 10;
	fifo_mode = 1;   
	l1a_internal = 0; 
	l1a_window = 5; 
	l1a_offset = 1;
	send_empty = 0;
	config_report = 0;
	bxn_before_reset = 0xabc;
	virtex_id = "40'b0101010101_011001100110011_100110011001100";
	trig_reg = "3'h5";
	config_reg = "69'b100110011_000100010001000_100010001000100_010001000100010_001000100010001";
	hot_channel_mask = 
	(
		(Signal)"112'h123456789abc123456789abcdef0",
				"112'h123456789abc123456789abcdef0",
				"112'h123456789abc123456789abcdef0",
				"112'h123456789abc123456789abcdef0",
				"112'h123456789abc123456789abcdef0",
				"112'h123456789abc123456789abcdef0"
	);

	collision_mask = 
	(
		(Signal)"14'h1234", "14'h4321",
		(Signal)"14'h5678", "14'h8765",
		(Signal)"14'h9abc", "14'hcba9",
		(Signal)"14'hdef0", "14'h0fed",
		(Signal)"14'h1234", "14'h4321",
		(Signal)"14'h5678", "14'h8765",
		(Signal)"14'h1234", "14'h4321",
		(Signal)"14'h5678", "14'h8765",
		(Signal)"14'h9abc", "14'hcba9",
		(Signal)"14'hdef0", "14'h0fed",
		(Signal)"14'h1234", "14'h4321",
		(Signal)"14'h5678", "14'h8765",
		(Signal)"14'h9abc", "14'hcba9",
		(Signal)"14'hdef0", "14'h0fed"
	);

	char rh[100];
	int l1anum = 0xf;
	for (int i = 0; i < 2000; i++)
	{

		L1A = 0;
		if (i == 0) hard_rst = 0; else hard_rst = 1;
		best1 = 0;
		best2 = 0;
		bxn = 0x800 + i;

		ly0 = 0;
		ly1 = 0;
		ly2 = 0;
		ly3 = 0;
		ly4 = 0;
		ly5 = 0;

		sprintf(rh, "112'h%03x%03x%03x%03x%03x%03x%03x%03x%03x", 
			l1anum, l1anum, l1anum, l1anum, l1anum, l1anum, l1anum, l1anum, l1anum);
//		cout << rh << endl;

/*		if (i%60 == 9)
		{
			best1 = l1anum/2 | 1;
			best2 = l1anum/2 | 1;
			ly0 = rh;
			ly1 = rh;
			ly2 = rh;
			ly3 = rh;
			ly4 = rh;
			ly5 = rh;
		}
		if (i%60 == 39)
		{
			best1 = l1anum/2 | 1;
			best2 = l1anum/2 | 1;
			ly0 = rh;
			ly1 = rh;
			ly2 = rh;
			ly3 = rh;
			ly4 = rh;
			ly5 = rh;
		}

		if (i%60 == 29 && (i < 0x170 || i > 0x1850))
		{
			l1anum++;
			L1A = 1;
		}
		if (i%60 == 59 && (i < 0x170 || i > 0x1850))
		{
			l1anum++;
			L1A = 1;
		}
*/
//		if (i > 10 && i < 40 && (i&1) == 0) L1A = 1;

		if (i == 3 || i == 6)
		{
			ly0 = rh;
			ly1 = rh;
			ly2 = rh;
			ly3 = rh;
			ly4 = rh;
			ly5 = rh;

			l1anum++;
		}	

		if (i == 13 || i == 14)
		{
			best1 = 2;
		}	

		if (i == 144)
		{
			L1A = (1 << 12) | (0x800 + 13); // bxn of the best track
		}

		if (i == 145)
		{
			L1A = (1 << 12) | (0x800 + 14); // bxn of the best track
		}

		if (i == 142+3)
		{
//			L1A = (1 << 12) | (0x800 + 1);
		}

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
			daqp.wire (18, 0, "daqp");
			l1a_delay.wire (7, 0, "l1a_delay"); 
			fifo_pretrig.wire (4, 0, "fifo_pretrig");
			fifo_mode.wire (1, 0, "fifo_mode");
			Wire_(L1A, BWTMARK, 0); 
			hard_rst.wire ("hard_rst");
			l1a_internal.wire ("l1a_internal"); // if ==1 then L1A will be generated internally for each valid track found
			l1a_window.wire (3, 0, "l1a_window");   // window in which to look for valid tracks in case of L1A
			l1a_offset.wire (3, 0, "l1a_offset");   // initial value for l1a counter
			L1AWindow.wire ("L1AWindow");
			l1aTP.wire ("l1aTP");
			Wire (validhd);
			Wire (send_empty);
			Wire (config_report);
			Wire_(bxn_before_reset, MXBXN-1, 0);
			Wire_(virtex_id, IDSIZE-1, 0);
			Wire_(trig_reg, 2, 0);
			Wire_(config_reg, CRSIZE-1, 0);
			Wire_(hot_channel_mask, HCMASKBITS-1, 0);
			Wire_(collision_mask, COLLMASKBITS-1, 0);
			clk.wire ("clk");
			
			Module(fifo_);
			fifo_
			(
				ly0, ly1, ly2, ly3, ly4, ly5,   // layer data
				best1,              			// best track parameters 
				best2,              			// second best track parameters
				bxn,                            // bx number
				fifo_tbins,                     // length of the dump for one L1A
				daqp,                            // output to daq (including all service bits) 
				l1a_delay,                       // time back to get the raw hits corresponding to this to L1A
				fifo_pretrig,                     // delay of the input raw hit data to compensate for the processing and pretriggering
				fifo_mode,                      // bits from configuration register defining the raw hits dump mode
				L1A,
				hard_rst,
				l1a_internal, l1a_window, l1a_offset,
				L1AWindow, l1aTP, validhd,
				send_empty,
				config_report,
				bxn_before_reset,
				virtex_id,
				trig_reg, 
				config_reg,
				hot_channel_mask,
				collision_mask, 
				clk
			);
			
			endsystem
		}
		cout << hex;
//		cout << L1A << " " << L1AWindow << " " << fifo_.L1AoneShot << " " << fifo_.L1Aempty << " " << fifo_.L1Astate << " ";
//		cout << fifo_.bestf.waddr << " " << fifo_.bestf.raddr << " " << fifo_.best1d <<  " " << fifo_.best1f << " ";
//		cout << "\t" << daqp << "\n";
//		cout << "in  " << best1 << " " << best2 << " " << bxn << " " << ly0 << endl;
/*
		cout << i << "\t" 
			 << fifo_.best1d << "\t"
			 << fifo_.best2d << "\t" 
			 << fifo_.bxnd << "\t" 
			 << fifo_.best1m << "\t"
			 << fifo_.best2m << "\t" 
			 << fifo_.bxnm << "\t" 
			 << (fifo_.lym[0] & 0xfffffff) << "\t"
			 << fifo_.l1a_proc 
			 << fifo_.best_we 
			 << fifo_.raw_we 
			 << fifo_.best_full
			 << fifo_.raw_full
			 << fifo_.l1a_fifo_re
			 << daqp(17) // dav bit
			 << "\t" 
			 << fifo_.best_adw << "\t" 
			 << fifo_.best_adwf << "\t" 
			 << fifo_.best_adr << "\t" 
			 << fifo_.raw_adr << "\t"
			 << fifo_.state << "\t"
			 << fifo_.best1m << "\t"
			 << daqp 
			 <<	endl;
*/
/*		if (i == 0x3a)
		{
			cout << "l1a_fifo dump" << endl;
			for (int mi = 0; mi < 10; mi++)
			{
				cout << mi << "   " << fifo_.l1a_fifo.mem[mi] << endl;
			}
		}
*/
//		if (fifo_.raw_full.getbool()) exit(0);
//		cout << fifo_.davgen.valor << "\t";
//		cout << fifo_.davgen.l1a_sh(0);
		cout << i << "\t" << L1A << "\t";
		cout << fifo_.bxne << "\t" << fifo_.best1e << "\t";
		cout << fifo_.al1a_maker_.l1a_out << "\t";
		cout << fifo_.have_lcts << "\t";
		cout << fifo_.best1d << "\t" << fifo_.best_we << "\t";
		cout << fifo_.l1a_procr << "\t";
		cout << fifo_.state << "\t";
		cout << fifo_.al1a_maker_.l1a_match.mem[0x80d] << "\t";
		cout << fifo_.al1a_maker_.l1a_match.mem[0x80e] << "\t";
		cout << daqp << "\n";

	}
	return 0;
}
