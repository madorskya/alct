#include "vlib.h"
#include "vmac.h"
#include "daq_memory_2clk.h"
#include "delay_line.h"
#include "delay_line_valor.h"
#include "daq_fifo_2clk.h"
#include "l1a_maker.h"
#include "crc_gbtx.h"

// Optical DAQ readout (GBTX / GTP links).
// Data are collected in 40M TTC clock domain (clk), 
// readout state machine runs in 160M link clock domain (clk160)
// and produces one 28-bit word per clk160 cycle.
// See Table 11 in ALCT firmware document for the format.
class daq_gbtx:public module
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
		Signal daq_gbtx_valid,
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
		Signal clk,
		Signal clk160
	);
	
	// clk domain
	delay_line raw_delay;
	delay_line_valor best_delay;
	delay_line l1a_dly;
	l1a_maker l1a_maker_;	
	// clk -> clk160 crossing
	daq_fifo_2clk l1a_fifo, l1a_bxn_fifo;
	daq_memory_2clk best_memory, raw_memory;
	// clk160 domain
	crc_gbtx crcgen;

	Signal lyd[6], lyzd;
    Signal best1d, best2d,  bxnd;
    Signal best1e, best2e,  bxne;
	Signal shower_e, shower_d;
	Signal l1a_int_delay;
	Signal l1a_proc, best_we, raw_we, valor;
	Signal l1a_procr, bxnr;
	Signal best_adw, raw_adw, best_full, raw_full;
	Signal l1a_in_count;
	Signal rst_cnt;
	Signal l1a_procd, l1a_in_countd, best_fulld, raw_fulld, best_adwd, raw_adwd, bxnrd, l1a_fifo_we;

	Signal l1a_fifo_re, l1a_fifo_empty, l1a_fifo_full;
	Signal l1a_bxn_fifo_empty, l1a_bxn_fifo_full;
	Signal l1a_in_countf, best_fullf, raw_fullf, best_adwf, raw_adwf, bxn_l1a;

	Signal best1m, best2m, bxnm, shower_m;
	Signal lym[6], lyzm;
	Signal best_adr, best_adb, raw_adr, raw_adb;

	Signal readout_count, state, idle_cnt, wphase;
	Signal best_cnt, lct_bins, raw_bins, lct_bins_report;
	memory lyt;
	Signal lzt, raw_cnt, ly_cnt, wg_cnt, ly_zero;
	Signal crccalc, daqw, frame_count, daqv_w;
    Signal fwver;
};
