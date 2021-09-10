#include "alct.h"

Signal lctin;
Signal valid, amu, lctSpec_FirstFr, dduSpec_LastFr;
Signal quality;
Signal keyp;
Signal bxn_wrFifo;
Signal daqData;
Signal seq_seu;
Signal cs_dly, settst_dly, rs_dly, din_dly, dout_dly, clk_dly;
Signal tck2, tms2, tdi2, tdo2;
Signal clk40sh, clk80;
Signal mx_oe;
Signal L1A_SyncAdb, ext_inject_trig;
Signal rsrvd_out, activeFeb_cfgDone;
Signal jstate;
Signal tst_pls;
//Signal InputEnTP, l1aTP, l1awindowTP;
Signal TP0, TP1;
Signal AsyncAdb;
Signal tp_strt_ext;
Signal alct_sn;
Signal mc_sn;
Signal clkp;
Signal ccb_brcstm;
Signal bstr1_subaddr;
Signal dout_bx0;
Signal seq_cmd02;
Signal seq_cmd13;
Signal rsv_in02;
Signal rsv_in13;
#ifdef S6
	Signal adc_sck;		// Serial clock
	Signal adc_sdi;		// Serial data to ADC
	Signal adc_ncs;		// Chip select active low
	Signal adc_sdo;		// Serial data from ADC
	Signal adc_eoc;		// End of conversion
#endif
	// parallel GBT inteface
#ifdef LX100
 Signal elink_p;
 Signal elink_n;
 Signal gbt_tx_datavalid;
 Signal gbt_clk40_p;
 Signal gbt_clk40_n;
 Signal gbt_txrdy;
#endif

 // serial link
#ifdef LX150T
 Signal tx_p;
 Signal tx_n;
 Signal refclk_p;
 Signal refclk_n;
#endif

Signal sl_cn_done;
alct alct_;

int main()
{
	int i, j, k, l;
	Signal* lct[] = {lctptr};
	char lctname[] = "lct _";

	cout << "RVALS = " << RVALS << " Sizeofrval = " << Sizeofrval << endl << flush;
	
	for (j = 0; j < 15; j++)
	{
		
//		for (i = 0; i < LCTS; i++)
//		{
//			*lct[i] = j;
//		}
/*		
		tck2 = 0;
		tms2 = 0;
		tdi2 = 0;
		
		L1A_SyncAdb = 0;
		ext_inject_trig = 0;
		AsyncAdb    = 0;
		ccb_brcstm = 0;
		bstr1_subaddr = 0;
*/		
		
		for (i = 0; i < 2; i++)
		{
/*			clkp = i;

			if (i == 1 && j == 3) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 3;   // bits 3:0 of command
				bstr1_subaddr = 1;
			}
			if (i == 0 && j == 3) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 0; // bits 7:4 of command
				bstr1_subaddr = 0;
			}
			
			if (i == 1 && j == 5) 
			{
				L1A_SyncAdb = 1;
				ccb_brcstm = 2;   // bits 3:0 of command
				bstr1_subaddr = 1;
			}
			if (i == 0 && j == 5) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 3; // bits 7:4 of command
				bstr1_subaddr = 0;
			}
			
			if (i == 1 && j == 7) 
			{
				L1A_SyncAdb = 1;
				ccb_brcstm = 1;   // bits 3:0 of command
				bstr1_subaddr = 1;
			}
			if (i == 0 && j == 7) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 0; // bits 7:4 of command
				bstr1_subaddr = 0;
			}
			
			if (i == 1 && j == 9) 
			{
				L1A_SyncAdb = 1;
				ccb_brcstm = 6;   // bits 3:0 of command
				bstr1_subaddr = 1;
			}
			if (i == 0 && j == 9) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 0; // bits 7:4 of command
				bstr1_subaddr = 0;
			}
			
			if (i == 1 && j == 11) 
			{
				L1A_SyncAdb = 1;
				ccb_brcstm = 7;   // bits 3:0 of command
				bstr1_subaddr = 1;
			}
			if (i == 0 && j == 11) 
			{
				L1A_SyncAdb = 0;
				ccb_brcstm = 0; // bits 7:4 of command
				bstr1_subaddr = 0;
			}
			
*/
			for (l = 1; l >= 0; l--) // this is how dll doubles the frequency
			{
//				alct_.dll2x.clk2x = l;
				
				beginsystem
				
				
				for (k = 0; k < LCTS; k++)
				{
					lctname[3] = '0' + k;
					lct[k]->wire(47, 0, lctname);
				}
				
				valid.wire("valid");
				amu.wire("amu");
				lctSpec_FirstFr.wire("lctSpec_FirstFr");
				dduSpec_LastFr.wire("dduSpec_LastFr");
				
				quality.wire(1, 0, "quality");
				keyp.wire(6, 0, "keyp");
				bxn_wrFifo.wire(2, 0, "bxn_wrFifo");
				daqData.wire(6, 0, "daqData");
				seq_seu.wire(1, 0, "seq_seu");
				cs_dly.wire(DLYLNGRP-1, 0, "cs_dly");
				settst_dly.wire("settst_dly");
				rs_dly.wire("rs_dly");
				din_dly.wire("din_dly"); 
				dout_dly.wire(DLYLNGRP-1, 0, "dout_dly");
				clk_dly.wire("clk_dly"); 
				
				tck2.wire("tck2"); // for TCK2 set the attribute DONT USE in the Global Buffers section of the synthesis constraints
				tms2.wire("tms2");
				tdi2.wire("tdi2"); 
				tdo2.wire("tdo2");
				
				clk40sh.wire(1, 0, "clk40sh");
				clk80.wire(1, 0, "clk80");
				
				mx_oe.wire("mx_oe");
				L1A_SyncAdb.wire("L1A_SyncAdb");
				ext_inject_trig.wire("ext_inject_trig");
				
				rsrvd_out.wire(1, 0, "rsrvd_out");
				activeFeb_cfgDone.wire("activeFeb_cfgDone");
				jstate.wire(3, 0, "jstate");
				tst_pls.wire("tst_pls");                     // test pulse wire
//				InputEnTP.wire("InputEnTP");
//				l1aTP.wire("l1aTP");
//				l1awindowTP.wire("l1awindowTP");
				Wire_ (TP0, 15, 0);
				Wire_ (TP1, 31, 0);
				clkp.wire("clkp");
				AsyncAdb.wire("AsyncAdb");
				tp_strt_ext.wire("tp_strt_ext");
				alct_sn.wire("alct_sn");
				mc_sn.wire("mc_sn");
				
				Wire_(ccb_brcstm, 3, 0);
				Wire (bstr1_subaddr);
				Wire (dout_bx0);
				
				Wire (seq_cmd02);
				Wire (seq_cmd13);
				Wire (rsv_in02);
				Wire (rsv_in13);

				Wire (sl_cn_done);

#ifdef S6
				Wire (adc_sck);		// Serial clock
				Wire (adc_sdi);		// Serial data to ADC
				Wire (adc_ncs);		// Chip select active low
				Wire (adc_sdo);		// Serial data from ADC
				Wire (adc_eoc);		// End of conversion
#endif
#ifdef LX100
	Wire_ (elink_p, 13, 0);
	Wire_ (elink_n, 13, 0);
	Wire  (gbt_tx_datavalid);
	Wire  (gbt_clk40_p);
	Wire  (gbt_clk40_n);
	Wire  (gbt_txrdy);
#endif

#ifdef LX150T
    Wire (tx_p);
    Wire (tx_n);
    Wire (refclk_p);
    Wire (refclk_n);
#endif

				
				alct_.init(ALCTNAME, "alct_");
				alct_
				(		
					lctin,
					valid, amu, lctSpec_FirstFr, dduSpec_LastFr,
					quality,
					keyp,
					bxn_wrFifo,
					daqData,
					seq_seu,
					cs_dly, settst_dly, rs_dly, din_dly, dout_dly, clk_dly,
					tck2, tms2, tdi2, tdo2,
					clk40sh, clk80,
					mx_oe,
					L1A_SyncAdb, ext_inject_trig,
					rsrvd_out, activeFeb_cfgDone,
					jstate,
					tst_pls,
//					InputEnTP, l1aTP, l1awindowTP,
					TP0, TP1,
					AsyncAdb,
					tp_strt_ext,
					alct_sn,
					mc_sn,
					ccb_brcstm, bstr1_subaddr, dout_bx0,
					seq_cmd02, seq_cmd13,
					rsv_in02,  rsv_in13, 
#ifdef S6
					adc_sck,		// Serial clock
					adc_sdi,		// Serial data to ADC
					adc_ncs,		// Chip select active low
					adc_sdo,		// Serial data from ADC
					adc_eoc,		// End of conversion
#endif
#ifdef LX100
					elink_p,
					elink_n,
					gbt_tx_datavalid,
					gbt_clk40_p,
					gbt_clk40_n,
					gbt_txrdy,
#endif
                            // serial link
#ifdef LX150T
                    tx_p,
                    tx_n,
                    refclk_p,
                    refclk_n,
#endif
                    sl_cn_done,
					clkp
				);
				
//				cout << "*";
				endsystem
//				cout << "\n";
			}
/*			
		    cout << clkp << " " 
				 << alct_.L1A << " " 
				 << alct_.ReadOutFifo.hard_rst_temp << " " 
				 << alct_.ReadOutFifo.L1Ar << " " 
				 << alct_.l1aTP << " "
				 << alct_.l1a_cnt_reset << "\n";
*/
		}
		cout << alct_.l1aTP << " " 
//			 << alct_.ReadOutFifo.L1Ar << " " 
			 << alct_.SyncAdb << " "
			 << hex 
			 << TP0(3) << " "
			 << TP0(4) << " "
			 << TP0(5) << " "
			 << TP0(6) << " "
			 << TP0(7) << " "
			 << alct_.brcst_str1 << " "
			 << alct_.ccb_brcst << "\n";
	}
	return 0;
	
}
