
#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

#include "dlls.h"
#include "jtag.h"

#ifdef DAQ01
#include "fifo.h"
#endif

#ifdef DAQ06
#include "daq.h"
#endif

#include "outfifo.h"
#ifdef RED_LAT
#ifdef TRIGWP
#include "trigger_wp.h"
#else
#include "trigger_rl.h"
#endif
#else
#include "trigger.h"
#endif

#include "synchro.h"
#include "hit_counters.h"
#include "loopback.h"
#include "ecc16_decoder.h"
#include "ecc32_encoder.h"
#include "gbtx.h"
#include "gtp_tux.h"

class Flip: public function
{
public:
	Signal operator()(Signal d);
};


class alct: public module
{
public:
	void operator()
	(		
		lctinput,
		Signal valid, Signal amu, Signal lctSpec_FirstFr, Signal dduSpec_LastFr,
		Signal quality,
		Signal keyp,
		Signal bxn_wrFifo,
		Signal daqData,
		Signal seq_seu,
		Signal cs_dly, Signal settst_dlyp, Signal rs_dly, Signal din_dly, Signal dout_dly, Signal clk_dly,
		Signal tck2, Signal tms2, Signal tdi2, Signal tdo2,
		Signal clk40sh, Signal clk80,
		Signal mx_oe,
		Signal L1A_SyncAdb, Signal ext_inject_trig,
		Signal rsrvd_out, Signal activeFeb_cfgDone,
		Signal jstate,
		Signal tst_pls,
		Signal TP0, Signal TP1,
		Signal AsyncAdb,
		Signal tp_strt_ext,
		Signal alct_sn,
		Signal mc_sn,
		Signal ccb_brcstm, Signal bstr1_subaddr, Signal dout_bx0,
		Signal seq_cmd02, Signal seq_cmd13,
		Signal rsv_in02,  Signal rsv_in13, 
#ifdef S6
		Signal adc_sck,		// Serial clock
		Signal adc_sdi,		// Serial data to ADC
		Signal adc_ncs,		// Chip select active low
		Signal adc_sdo,		// Serial data from ADC
		Signal adc_eoc,		// End of conversion
#endif
		// parallel GBT inteface
#ifdef LX100
		Signal elink_p,
		Signal elink_n,
		Signal gbt_tx_datavalid,
		Signal gbt_clk40_p,
		Signal gbt_clk40_n,
		Signal gbt_txrdy,
#endif
		// serial link
#ifdef LX150T
		Signal tx_p,
		Signal tx_n,
		Signal refclk_p,
		Signal refclk_n,
#endif
		Signal sl_cn_done,
		Signal clkp
	);

	Signal valid, amu, lctSpec_FirstFr, dduSpec_LastFr;
	Signal quality;
	Signal keyp;
	Signal bxn_wrFifo;
	Signal daqData;
	Signal seq_seu;

	Signal collmask;
	Signal HCmask;
	Signal hmt_thresholds;
	Signal PromoteColl;
	Signal drifttime;
	Signal pretrig;
	Signal trig;
    Signal trig_mode;
    Signal fifo_tbins;
	
    Signal h, l;
    Signal hn, ln;
    Signal hfa, hpatb, lfa, lpatb;
    Signal ConfgReg;
    Signal bxn; // global bx number
    Signal daqo; // output of the L1A fifo, including control bits
    Signal fifo_mode;
    Signal BoardID;
    Signal l1a_delay; // time from trigger to L1A
    Signal L1A, SyncAdb, SyncAdb1; // demuxed L1A and SyncAdb inputs
    
    Signal clk, clk2x, input_dis, tck2b;
    Signal OSdata;
    Signal OSre;
    Signal z20; // dummy
    Signal locked; // first dll locked
    Signal clk2xsh;
    Signal TstPlsEn;   // test pulse enable from JTAG interface
    Signal tst_pls_en; // Signalisters for TstPlsEn rising edge detection
    Signal l1a_internal; // if ==1 then the board will generate L1A for itself on each trigger.
   	Signal l1a_window;   // window in which to look for valid tracks in case of L1A
    Signal l1a_offset;   // initial value for l1a counter
    Signal fifo_pretrig; // time bins before pretrigger (included in fifo_tbins)
    Signal ext_trig_en; // external trigger enable. If set to 1 and there is no ext_trig input, input data are masked out
	Signal NoSpaceForDAQ; // tells DAQ state machine, that there is no space in output buffer for the next DAQ readout. Used only when output FIFO is enabled.
	Signal Counters; // used for raw hit fifo debugging
	Signal hard_rst; 
	Signal hrstcnt;
	Signal ly[LYN];
	Signal input_disr;
	Signal clkb;
	Signal settst_dly;
	Signal inject;
	Signal ext_inject;
	Signal ext_trig;
	Signal trig_info_en;
	Signal tst_plss;
	Signal TrigReg;
	Signal SNout, SNin;
	Signal sn_select;
	Signal actv_feb_fg;
	Signal ccb_brcst, subaddr_str, brcst_str1, bx0, dout_str;
	Signal ccb_brcstd, brcst_str1d, bx0d;
	Signal l1a_cnt_reset;
	Signal lhc_cycle_sel;
	Signal ttc_l1reset,	ttc_bxreset, ttc_bx0, ttc_start_trigger, ttc_stop_trigger;
	Signal l1aTP, l1awindowTP;
	Signal validh, validl, validhd;
	Signal fmm_trig_stop;
	Signal send_empty;
	Signal bxc_offset;
	Signal bxn_before_reset;
	Signal clk2x_buff;
	Signal virtex_id;
	Signal clksh;
	Signal clksh_inv;
	Signal hcounters;
#ifdef DAQ06
	Signal config_report;
#endif

	Signal os_enable, ConfgRegx, acc_pretrig, acc_trig;
	Signal seq_cmd, seq_cmd_r, rsv_in;
	Signal alct_tx_1st_tpat, alct_tx_2nd_tpat, alct_sync_mode;
    Signal alct_tx_1st_tpat_r, alct_tx_2nd_tpat_r;
	Signal seq_cmd02_r, seq_cmd13_r;

	Signal L1A_SyncAdb_r, ext_inject_trig_r, ccb_brcstm_r, bstr1_subaddr_r, dout_bx0_r, rsv_in02_r, rsv_in13_r;
	Signal L1A1, ext_inject1, ext_trig1, subaddr_str1, dout_str1, rsv_in1;
	Signal SyncAdb2, L1A2, ext_trig2, ext_inject2, bx0_2, dout_str2, subaddr_str2, brcst_str2, ccb_brcst2;
	Signal dec_out, ecc_error;
	Signal send_bxn, bxn_mux, ecc_err_5, parity_out;
	Signal ttc_bx0_e, actv_feb_fg_e, bxn_mux_e, ln_e, l_e, lfa_e, validl_e, hn_e, h_e, hfa_e, validh_e, dummy3;
	Signal zero_suppress;
	Signal clock_lac;
	Signal din_dly_int;
	Signal shower_int, shower_bits;


	dll  dll2x; // DLL for doubling the clock
	dll2 dllsh;   // DLL for the double clock phase adjustment
	jtag TAP;
#ifdef DAQ01
	fifo ReadOutFifo;
#endif

#ifdef DAQ06
	daq daq_06;
#endif

	outfifo of;
	Flip flip;
#ifdef RED_LAT
#ifdef TRIGWP
	trigger_wp core;
#else
	trigger_rl core;
#endif
#else
	trigger core;
#endif
	synchro sync;
	hit_counters hitcnt;
	loopback lpback;
	ecc16_decoder ecc16_dec;
	ecc32_encoder ecc32_en;

#ifdef LX100
	gbtx_c gbtx;
#endif

#ifdef LX150T
    gtp_tux_c gtp_tux;
#endif

	Signal seu_error;

};



