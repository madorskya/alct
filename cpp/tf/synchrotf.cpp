#include "synchro.h"

Signal hard_rst;
Signal ccb_cmd;
Signal ccb_cmd_strobe;
Signal ccb_bc0;
Signal bxn_offset;
Signal lhc_cycle_sel;
Signal bxn_counter;
Signal l1a_cnt_reset;
Signal fmm_trig_stop;
Signal ttc_l1reset;
Signal ttc_bc0;
Signal ttc_start_trigger;
Signal ttc_stop_trigger;
Signal bxn_before_reset;
Signal clk;

synchro syncr;

string state_name[] = 
{
	"fmm_stop_trigger",
	"fmm_resync",		
	"fmm_wait_bc0",	
	"fmm_run"			
};

int main ()
{

	int i,j;

//	bxn_counter.esc_sec       = "\033""[00;31m";

	for (j = 0; j < 40; j++)
	{


		switch (j)
		{
		case 0: 
			hard_rst = 0; 
			break;

		case 3:
			ccb_cmd = TTC_L1RESET;
			ccb_cmd_strobe = 1;
			break;

		case 6:
			ccb_cmd = TTC_BC0;
			ccb_cmd_strobe = 1;
			break;

/*		case 9:
			ccb_cmd = TTC_START_TRIGGER;
			ccb_cmd_strobe = 1;
			break;
*/
/*		case 12:
		case 35:
			ccb_cmd = TTC_BC0;
			ccb_cmd_strobe = 1;
			break;
*/
			
		case 15:
			ccb_cmd = TTC_STOP_TRIGGER;
			ccb_cmd_strobe = 1;
			break;

		case 18:
			ccb_cmd = TTC_L1RESET;
			ccb_cmd_strobe = 1;
			break;

		case 21:
			ccb_cmd = TTC_BC0;
			ccb_cmd_strobe = 1;
			break;

		case 27:
			ccb_cmd = TTC_BC0;
			ccb_cmd_strobe = 1;
			break;
			
		case 30:
			ccb_cmd = TTC_STOP_TRIGGER;
			ccb_cmd_strobe = 1;
			break;

		case 33:
			ccb_cmd = TTC_START_TRIGGER;
			ccb_cmd_strobe = 1;
			break;

		case 35:
			ccb_cmd = TTC_BC0;
			ccb_cmd_strobe = 1;
			break;
			
		default:
			hard_rst = 1;
			ccb_cmd = 0;
			ccb_cmd_strobe = 0;
			ccb_bc0 = 0;
			bxn_offset = 5;
			lhc_cycle_sel = 0;
		}


		for (i = 0; i < 2; i++)
		{
			clk = i;
			beginsystem

			Wire(hard_rst);
			Wire_(ccb_cmd, 5, 0);
			Wire(ccb_cmd_strobe);
			Wire(ccb_bc0);
			Wire_(bxn_offset, 11, 0);
			Wire(lhc_cycle_sel); // 1 -> 3564, 0 -> 924

			Wire_(bxn_counter, 11, 0);
			Wire(l1a_cnt_reset);
			Wire(fmm_trig_stop);

			Wire(ttc_l1reset);  
			Wire(ttc_bc0);
			Wire(ttc_start_trigger);
			Wire(ttc_stop_trigger);
			Wire_(bxn_before_reset, MXBXN-1, 0);
			Wire(clk);

			syncr.init("synchro", "sync");
			syncr
				(
					hard_rst,
					ccb_cmd,
					ccb_cmd_strobe,
					ccb_bc0,
					bxn_offset,
					lhc_cycle_sel,
					bxn_counter,
					l1a_cnt_reset,
					fmm_trig_stop,
					ttc_l1reset,
					ttc_bc0,
					ttc_start_trigger,
					ttc_stop_trigger,
					bxn_before_reset,
					clk
				);
			endsystem
		}
		cout << j << "\t" 
			 << ttc_bc0 << "\t"
			 << bxn_counter << "\t " 
//			 << bxn_before_reset << "\n";
			 << l1a_cnt_reset << " "
			 << fmm_trig_stop << "\t"
			 << ttc_l1reset << " "
			 << ttc_bc0 << " "
			 << ttc_start_trigger << " "
			 << ttc_stop_trigger << " ";
		cout << state_name[syncr.fmm_sm.getr().r[0]] << "\n";

	}
}
