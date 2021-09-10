#include "synchro.h"

#define ttc_command(opcode) ((ccb_cmd == opcode) && ccb_cmd_strobe)

void synchro::operator()
(
	Signal hard_rst,
	Signal os_enable,
	Signal ccb_cmd,
	Signal ccb_cmd_strobe,
	Signal ccb_bc0,
	Signal bxn_offset,
	Signal lhc_cycle_sel,
	Signal bxn_counter,
	Signal l1a_cnt_reset,
	Signal fmm_trig_stop,
	Signal ttc_l1reset,
	Signal ttc_bc0,
	Signal ttc_start_trigger,
	Signal ttc_stop_trigger,
	Signal bxn_before_reset, // request from TY/Wesley 10/24/2005
	Signal clk
)
{
initio
	Input(hard_rst);
	Input(os_enable); // output storage enable flag, means user is running JTAG test
	Input_(ccb_cmd, 5, 0);
	Input(ccb_cmd_strobe);
	Input(ccb_bc0);
	Input_(bxn_offset, MXBXN-1, 0);
	Input(lhc_cycle_sel); // 1 -> 3564, 0 -> 924

	OutReg_(bxn_counter, MXBXN-1, 0);
	OutReg(l1a_cnt_reset);
	OutReg(fmm_trig_stop);

	Output(ttc_l1reset);  
	Output(ttc_bc0);
	Output(ttc_start_trigger);
	Output(ttc_stop_trigger);

	OutReg_(bxn_before_reset, MXBXN-1, 0);

	Input(clk);

beginmodule

	Wire_(lhc_cycle, MXBXN-1, 0);
	Reg_(fmm_sm, 1, 0);

	assign lhc_cycle = ifelse(lhc_cycle_sel, LHC_CYCLE, BEAM_TEST_CYCLE);

	assign ttc_l1reset       = ttc_command(TTC_L1RESET);
#define ttc_resync ttc_l1reset
	assign ttc_bc0           = ttc_command(TTC_BC0);
	assign ttc_start_trigger = ttc_command(TTC_START_TRIGGER);
	assign ttc_stop_trigger  = ttc_command(TTC_STOP_TRIGGER);

	always (posedge (clk))
	begin  
		fmm_trig_stop = 0;
		l1a_cnt_reset = 0;

	    If (!hard_rst) 
		begin
			fmm_sm = fmm_stop_trigger;
			fmm_trig_stop = 1; // stops triggers
			l1a_cnt_reset = 1; // resets DAQ
			bxn_counter = bxn_offset;       // reset counter    
		end
	    Else   
		begin
			begincase (fmm_sm) 
					
				case1(fmm_stop_trigger)
				begin
					fmm_trig_stop = 1; // stops triggers
					If (ttc_l1reset) fmm_sm = fmm_resync;
					Else If (ttc_start_trigger) fmm_sm = fmm_wait_bc0;
				end
					
				case1(fmm_resync)
				begin
					fmm_trig_stop = 1; // stops triggers
					l1a_cnt_reset = 1; // resets DAQ
					bxn_counter = bxn_offset;       // reset counter    
					fmm_sm = fmm_wait_bc0;
				end
					
				case1(fmm_wait_bc0)
				begin
					fmm_trig_stop = 1; // stops triggers
					If (ttc_bc0) fmm_sm = fmm_run;
					Else If (ttc_resync) fmm_sm = fmm_resync;
					Else If (ttc_stop_trigger) fmm_sm = fmm_stop_trigger; // request from Jay 5/4/2007
				end
					
				case1(fmm_run)
				begin
					If (ttc_stop_trigger) fmm_sm = fmm_stop_trigger;
					Else If (ttc_resync) fmm_sm = fmm_resync;
				end
					
				Default
					fmm_sm = fmm_stop_trigger;
			endcase

			// take care of bxn counter
			If (ttc_bc0)   // request from TY/Wesley 10/24/2005
			begin
				bxn_before_reset = bxn_counter; // remember what was bxn counter before reset
				bxn_counter = bxn_offset;       // reset counter    
			end
			Else If (bxn_counter == lhc_cycle - 1)	bxn_counter = 0; // overflow at selected terminal count
			Else					bxn_counter ++;

			If (os_enable) fmm_trig_stop = 0; // unstop trigger if user is running JTAG tests

		end 
	end

endmodule
}
