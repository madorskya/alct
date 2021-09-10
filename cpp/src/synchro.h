#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#define LHC_CYCLE 3564
#define BEAM_TEST_CYCLE 924

#define TTC_BC0				1 	
#define TTC_L1RESET         3
#define TTC_START_TRIGGER   6 
#define TTC_STOP_TRIGGER    7 

#define fmm_stop_trigger 0
#define fmm_resync		 1
#define fmm_wait_bc0	 2
#define fmm_run			 3

class synchro: public module
{
public:
	void operator()
	(
		Signal hard_rst,
		Signal os_enable,
		Signal ccb_cmd,
		Signal ccb_cmd_strobe,
		Signal ccb_bc0,
		Signal bxn_offset,
		Signal lhc_cycle,
		Signal bxn_counter,
		Signal l1a_cnt_reset,
		Signal fmm_trig_stop,
		Signal ttc_l1reset,
		Signal ttc_bc0,
		Signal ttc_start_trigger,
		Signal ttc_stop_trigger,
		Signal bxn_before_reset,
		Signal clk
	);

	Signal lhc_cycle;
	Signal fmm_sm;
};

