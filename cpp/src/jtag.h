#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class jtag: public module
{
public:
	void operator()
	(
		Signal tck, Signal tms, Signal tdi, Signal tdo,
		Signal HCmask, Signal collmask, Signal ParamReg, Signal ConfgReg, 
		Signal hmt_thresholds,
		Signal tst_pls,
		Signal din_dly, Signal dout_dly, Signal clk_dly, Signal input_dis,
		Signal YR, Signal OS, Signal OSre,
#ifdef S6
		Signal adc_sck,		// Serial clock
		Signal adc_sdi,		// Serial data to ADC
		Signal adc_ncs,		// Chip select active low
		Signal adc_sdo,		// Serial data from ADC
		Signal adc_eoc,		// End of conversion
#endif
		Signal hard_rst,
		Signal jstate,
		Signal ID,
		Signal TrigReg,
		Signal SNout,
		Signal SNin,
		Signal hitcounters,
		Signal clk
	);

	Signal 
		ParamRegs, 
		ConfgRegs, 
		dly_clk_en, 
		TAPstate, 
		IR, 
		bpass, 
		sr, 
		tdomux, 
		dly_tdo, 
		YRs, 
		OSs, 
		OSre, 
		TrigRegs, 
		TrigReg, 
		IDs, 
		SNcnt, 
		SNrd,
		SNresetRQ,
		SNwrite0RQ,
		SNwrite1RQ,
		SNreadRQ,
		SNresetRQ1,
		SNwrite0RQ1,
		SNwrite1RQ1,
		SNreadRQ1,
		SNresetRQ2,
		SNwrite0RQ2,
		SNwrite1RQ2,
		SNreadRQ2,
		SNstate,
		hcounterss;
#ifdef S6
	Signal
		adc_wr_reg,
		adc_rd_reg,
		adc_wr_sr,
		adc_rd_sr;
#endif

};

