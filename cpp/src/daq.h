#include "vlib.h"
#include "vmac.h"
#include "daq_memory.h"
#include "delay_line.h"
#include "delay_line_valor.h"
#include "daq_fifo.h"
#include "l1a_maker.h"
#include "crc.h"
#include "dav06.h"

class daq:public module
{
 public:
	void operator()
	(
		Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,   
		Signal best1,              			
		Signal best2,              			
		Signal shower_int,
		Signal bxn,                            
		Signal fifo_tbins,                     
		Signal daqp,                            
		Signal l1a_delay,                       
		Signal fifo_pretrig,                     
		Signal fifo_mode,                      
		Signal L1A,
		Signal hard_rst,
		Signal l1a_internal, 
		Signal l1a_window, 
		Signal l1a_offset,
		Signal L1AWindow, 
		Signal l1aTP, 
		Signal validhd,
		Signal send_empty,
		Signal config_report,
		Signal bxn_before_reset,
		Signal virtex_id,
		Signal trig_reg,
		Signal config_reg,
		Signal hot_channel_mask,
		Signal collision_mask,
		Signal zero_suppress,
		Signal trig_stop,
		Signal seu_error,
		Signal clk
	);
	
	delay_line raw_delay;
	delay_line_valor best_delay;
	daq_fifo l1a_fifo, l1a_bxn_fifo;
	daq_memory best_memory, raw_memory;
	l1a_maker l1a_maker_;	

	Signal lyd[6];
    Signal best1d, best2d,  bxnd;
    Signal best1e, best2e,  bxne;

	Signal best_mem_ad, raw_mem_ad, best_mem_adf, raw_mem_adf;
	Signal l1a_fifo_re, l1a_fifo_empty, l1a_fifo_full;

	Signal l1a_proc, best_we, raw_we;
	
	Signal best1m, best2m, bxnm;
	Signal lym[6];
	Signal best_adw, best_adwf, best_adr, best_adb;
	Signal raw_adw, raw_adwf, raw_adr, raw_adb;
	Signal best_full, raw_full, best_fullf, raw_fullf;

	Signal l1a_number, readout_count, state;
	Signal l1a_int_delay, l1a_int_offset;
	Signal best_cnt, have_lcts, bxn_track;
	Signal lct_bins, raw_bins, l1a_count, lct_bins_report;

	Signal hcpart_cnt, cmpart_cnt, hot_reg;
	Signal best1t, best2t, best_first;

	memory lyt;
	Signal raw_cnt, ly_cnt, wg_cnt;

	Signal crccalc, daqw, davv, frame_count, l1a_procr;
	Signal conf_w_count, hot_w_count;
	memory config_word, collmask_word, hot_word;

	Signal bxn_l1a,	l1a_bxn_fifo_empty,	l1a_bxn_fifo_full, bxnr, ly_zero, tb_zero;
	Signal l1a_in_count, l1a_in_countf, valor, config_report, valorr;
	Signal shower_e, shower_d, shower_m, shower_t;

	dav davgen;
	crc crcgen;
};
