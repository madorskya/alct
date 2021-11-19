
#include "alct.h"
#include "stdio.h"
#include "time.h" 

void alct::operator()
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
)
{
initio

	int wgi, wgf, i, j;
	Signal* lct[] = {lctptr};
	char lctname[] = "lct _";

	comment("// input buses")
	for (i = 0; i < LCTS; i++)
	{
		lctname[3] = '0' + i;
		lct[i]->input(47, 0, lctname);
	}

	valid.output("valid", makereg);
	amu.output("amu", makereg);
	lctSpec_FirstFr.output("lctSpec_FirstFr", makereg);
	dduSpec_LastFr.output("dduSpec_LastFr", makereg);

	quality.output(1, 0, "quality", makereg);
	keyp.output(6, 0, "keyp", makereg);
	bxn_wrFifo.output(2, 0, "bxn_wrFifo", makereg);
	daqData.output(6, 0, "daqData", makereg);
	seq_seu.output(1, 0, "seq_seu", makereg);
	cs_dly.output(DLYLNGRP-1, 0, "cs_dly");
	comment("// settst_dly == 0 means take inputs from AFEBS, ==1 takes inputs from internal shift registers of delay lines")
	settst_dlyp.output("settst_dlyp"); 
	rs_dly.output("rs_dly");
	din_dly.output("din_dly"); 
	dout_dly.input(DLYLNGRP-1, 0, "dout_dly");
	clk_dly.output("clk_dly"); 

	tck2.input("tck2"); comment("// for TCK2 set the attribute DONT USE in the Global Buffers section of the synthesis constraints")
	tms2.input("tms2");
	tdi2.input("tdi2"); 
	tdo2.output("tdo2");

	Output_(clk40sh, 1, 0);
	clk80.output(1, 0, "clk80");

	mx_oe.output("mx_oe");
	L1A_SyncAdb.input("L1A_SyncAdb");
	ext_inject_trig.input("ext_inject_trig");

	rsrvd_out.output(1, 0, "rsrvd_out", makereg);
	activeFeb_cfgDone.output("activeFeb_cfgDone", makereg);
	jstate.output(3, 0, "jstate");
	tst_pls.output("tst_pls");                     comment("// test pulse output")
    Output_(TP0, 15, 0); comment("// test point header 0")
    Output_(TP1, 31, 0); comment("// test point header 1")
	AsyncAdb.input("AsyncAdb");
	tp_strt_ext.input("tp_strt_ext");
	alct_sn.inout("alct_sn");
	mc_sn.inout("mc_sn");

	Input_(ccb_brcstm, 3, 0);
	Input (bstr1_subaddr);
	Input (dout_bx0);
	Input (seq_cmd02);
	Input (seq_cmd13);
	Input (rsv_in02);
	Input (rsv_in13);

#ifdef S6
	Output (adc_sck);		// Serial clock
	Output (adc_sdi);		// Serial data to ADC
	Output (adc_ncs);		// Chip select active low
	Input (adc_sdo);		// Serial data from ADC
	Input (adc_eoc);		// End of conversion
#endif
#ifdef LX100
	Output_ (elink_p, 13, 0);
	Output_ (elink_n, 13, 0);
	Output (gbt_tx_datavalid);
	Input  (gbt_clk40_p);
	Input  (gbt_clk40_n);
	Input  (gbt_txrdy);
#endif
#ifdef LX150T
    Output_ (tx_p, 1, 0);
    Output_ (tx_n, 1, 0);
	Input (refclk_p);
	Input (refclk_n);
#endif
	Input (sl_cn_done);

	clkp.input("clkp");

beginmodule


    flip.init(7, 0, "Flip");

	collmask.wire(COLLMASKBITS-1, 0, "collmask");
	HCmask.wire(HCMASKBITS-1, 0, "HCmask");
	Wire_(hmt_thresholds, HMT_THRESHOLD_BITS-1, 0);
	PromoteColl.wire("PromoteColl");
	drifttime.wire(2, 0, "drifttime");
	pretrig.wire(2, 0, "pretrig"); // threshold for coll tracks
	trig.wire(2, 0, "trig"); // pattern for coll tracks
	Wire_(acc_pretrig, 2, 0); // threshold for acc tracks
	Wire_(acc_trig, 2, 0); // pattern for acc tracks
    trig_mode.wire(1, 0, "trig_mode");
    fifo_tbins.wire(4, 0, "fifo_tbins");
	
    h.wire(QBITS-1, 0, "h");
    hn.wire(KEYBITS-1, 0, "hn");
    hfa.wire("hfa");
	hpatb.wire("hpatb");

    l.wire(QBITS-1, 0, "l");
    ln.wire(KEYBITS-1, 0, "ln");
	lfa.wire("lfa");
	lpatb.wire("lpatb");

    ConfgReg.wire(CRSIZE-1, 0, "ConfgReg"); // original config register
	Wire_(ConfgRegx, YRSIZE-1, 0); // extended config register (former YR - output register)

    daqo.wire(DAQBITS-1, 0, "daqo"); comment("// the L1A fifo input, including control bits")
    fifo_mode.wire(1, 0, "fifo_mode");
    BoardID.wire(2, 0, "BoardID");
    l1a_delay.wire(7, 0, "l1a_delay"); comment("// time from trigger to L1A")
    L1A.reg("L1A"); 

	Reg (SyncAdb);  comment("// demuxed SyncAdb input at 80 MHz")
	Reg (SyncAdb1); comment ("// demuxed SyncAdb at 40 MHz")
	Reg (SyncAdb2); comment ("// demuxed SyncAdb at 40 MHz after ecc")
    
    clk.wire("clk");
	clk2x.wire("clk2x");
	input_dis.wire("input_dis");
	tck2b.wire("tck2b");

    OSdata.wire(OSSIZE, 0, "OSdata"); comment("// size of this wire is one bit more than needed. This bit is used for fifo_empty flag ")
    OSre.wire("OSre");
    z20.wire(19, 0, "z20"); comment("// dummy")
    locked.wire("locked"); comment("// first dll locked")
    clk2xsh.wire("clk2xsh");
    TstPlsEn.wire("TstPlsEn");   comment("// test pulse enable from JTAG interface")
    tst_pls_en.reg(1, 0, "tst_pls_en"); comment("// registers for TstPlsEn rising edge detection")
    l1a_internal.wire("l1a_internal"); comment("// if ==1 then the board will generate L1A for itself on each trigger.")
   	l1a_window.wire(3, 0, "l1a_window");   comment("// window in which to look for valid tracks in case of L1A")
    l1a_offset.wire(3, 0, "l1a_offset");   comment("// initial value for l1a counter")
    fifo_pretrig.wire(4, 0, "fifo_pretrig"); comment("// time bins before pretrigger (included in fifo_tbins)")
	NoSpaceForDAQ.wire("NoSpaceForDAQ"); comment("// tells DAQ state machine, that there is no space in buffer.output(buffer) for the next DAQ readout. Used only when FIFO.output(FIFO) is enabled.")
	Counters.wire(23, 0, "Counters"); comment("// used for raw hit fifo debugging")

	hard_rst.reg("hard_rst"); 
#ifdef VGEN
	printv("initial hard_rst = 0;\n");
#endif
	hrstcnt.reg(14, 0, "hrstcnt");
	ly[0].reg(LYWG-1, 0, "ly0");
	ly[1].reg(LYWG-1, 0, "ly1");
	ly[2].reg(LYWG-1, 0, "ly2");
	ly[3].reg(LYWG-1, 0, "ly3");
	ly[4].reg(LYWG-1, 0, "ly4");
	ly[5].reg(LYWG-1, 0, "ly5");
	input_disr.reg("input_disr");
	clkb.wire("clkb");
	settst_dly.wire("settst_dly");
	inject.wire("inject");	comment("// enables injecting patterns using ext_inject input")
	ext_inject.reg("ext_inject");
	ext_trig.reg("ext_trig");
    ext_trig_en.wire("ext_trig_en"); comment("// external trigger enable")
	trig_info_en.wire("trig_info_en");comment("// if it is == 0, the detected track info will not be written to the FIFO.output(FIFO), only DAQ readout will be written")
	tst_plss.reg("tst_plss");
	TrigReg.wire(4, 0, "TrigReg");
	SNout.wire("SNout");
	SNin.wire(1, 0, "SNin");
	sn_select.wire("sn_select");
	actv_feb_fg.wire("actv_feb_fg"); comment("// this is set to 1 as soon as some pattern is found by pattern detectors")

	comment ("// ccb control signals")
	Reg_(ccb_brcstd, 7, 0);
	Reg_(ccb_brcst, 7, 0);
	Reg_(ccb_brcst2, 7, 0);
	Reg (subaddr_str);
	Reg (brcst_str1d);
	Reg (brcst_str1);
	Reg (brcst_str2);
	Reg (bx0d);
	Reg (bx0);
	Reg (bx0_2);
	Reg (dout_str);
    Wire_(bxn, 11, 0); comment("// global bx number")
	Wire (l1a_cnt_reset);
	Wire (lhc_cycle_sel);

	Wire (ttc_l1reset);
	Wire (ttc_bx0);
	Wire (ttc_start_trigger);
	Wire (ttc_stop_trigger);

	Wire (l1aTP);
	Wire (l1awindowTP);
	Wire (validh);
	Wire (validl);
	Wire (validhd);
	Wire (fmm_trig_stop);

	Wire (send_empty);
	Wire_(bxc_offset, 11, 0);
	Wire_(bxn_before_reset, MXBXN-1, 0); // last value of bxn counter before reset, should be always the same normally
	Wire (clk2x_buff);
	Wire_(virtex_id, IDSIZE-1, 0);
	Wire (clksh); // shifted version of the main clock to control external muxes
	Wire (os_enable); // output storage enable (former YRIclkEnr)
	Wire_(hcounters, BWHC*HCNUM-1, 0); // combined hit counters

	Reg_(seq_cmd, 3, 0); // test command from TMB
	Reg_(seq_cmd_r, 3, 0); // test command from TMB
	Reg_(rsv_in,  3, 0); // reserved bits

	Wire_(alct_tx_1st_tpat, 28,1); // loopback test patterns 
	Wire_(alct_tx_2nd_tpat, 28,1);
	Wire (alct_sync_mode); // loopback mode flag

    Reg_ (alct_tx_1st_tpat_r, 28, 1);
    Reg_ (alct_tx_2nd_tpat_r, 28, 1);

#ifdef DAQ06
	Wire (config_report);
#endif

	Reg (seq_cmd02_r);
	Reg (seq_cmd13_r);

	Reg (L1A_SyncAdb_r);    
	Reg (ext_inject_trig_r);
	Reg_(ccb_brcstm_r, 3, 0);     
	Reg (bstr1_subaddr_r);  
	Reg (dout_bx0_r);       
	Reg (rsv_in02_r);       
	Reg (rsv_in13_r);       

	Reg (L1A1);
	Reg (ext_inject1);
	Reg (ext_trig1);
	Reg (subaddr_str1);
	Reg (dout_str1);
	Reg_(rsv_in1, 3, 0);

	Reg (L1A2);
	Reg (ext_inject2);
	Reg (ext_trig2);
	Reg (subaddr_str2);
	Reg (dout_str2);

	Wire_(dec_out, 15, 0);
	Wire_(ecc_error, 1, 0);

	Wire (send_bxn);
	Wire_(bxn_mux, 4, 0);
	Reg_(ecc_err_5, 4, 0);
	Wire_(parity_out, 6, 0);

	Wire (ttc_bx0_e); 
	Wire (actv_feb_fg_e); 
	Wire_(bxn_mux_e, 4, 0);
	Wire_(ln_e, 6, 0);
	Wire_(l_e, 1, 0);   
	Wire (lfa_e);
	Wire (validl_e); 
	Wire_(hn_e, 6, 0);
	Wire_(h_e, 1, 0);   
	Wire (hfa_e); 
	Wire (validh_e);
	Wire_(dummy3, 2, 0);
	Wire (zero_suppress);
	Wire (clock_lac);
  Wire_(shower_int, 1,0);
	Wire_(shower_bits, 4,0);

#ifdef VGEN
	printv("\n\tIBUFG ibufclk (.I(clkp), .O(clkb));");	// input clock buffer (instantiation required for DLL)
#ifndef S6
	printv("\n\tOBUF_F_24 ob800 (.O(clk80[0]), .I(clk2xsh));"); // output 80 MHz clock needs fast buffers
	printv("\n\tOBUF_F_24 ob801 (.O(clk80[1]), .I(clk2xsh));");
    printv("\n\tOBUF_S_8 ob400 (.O(clk40sh[0]), .I(clksh));"); // mux control
    printv("\n\tOBUF_S_8 ob401 (.O(clk40sh[1]), .I(clksh));");
#endif
	printv("\n\tIBUF buftck (.I(tck2), .O(tck2b)); // synthesis attribute buffer_type tck2 ibuf\n");
	comment("// tck global buffer")
	printv("\tIOBUF iobsn0 (.IO(alct_sn), .I(1'b0), .O(SNin[0]), .T(SNout));\n");	// serial number buffer (open drain)
	printv("\tIOBUF iobsn1 (.IO(mc_sn),   .I(1'b0), .O(SNin[1]), .T(SNout));\n");	// serial number buffer (open drain)

	Wire (seu_error);
#ifdef S6
	// clock forwarding
	Wire (clksh_inv);
#if ((defined(ALCT576) && defined(MIRROR)) || (defined(ALCT672) && !defined(MIRROR)))
	printv("\n\tODDR2 oddr_ckl40_fw0 (.D0(1'b1), .D1(1'b0), .C0(clksh), .C1(!clksh), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk40sh[0]));");
	printv("\n\tODDR2 oddr_ckl40_fw1 (.D0(1'b1), .D1(1'b0), .C0(clksh), .C1(!clksh), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk40sh[1]));");
#else
	printv("\n\tODDR2 oddr_ckl40_fw0 (.D0(1'b1), .D1(1'b0), .C0(clksh), .C1(clksh_inv), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk40sh[0]));");
	printv("\n\tODDR2 oddr_ckl40_fw1 (.D0(1'b1), .D1(1'b0), .C0(clksh), .C1(clksh_inv), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk40sh[1]));");
#endif
	printv("\n\tODDR2 oddr_ckl80_fw0 (.D0(1'b1), .D1(1'b0), .C0(clk2xsh), .C1(!clk2xsh), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk80[0]));");
	printv("\n\tODDR2 oddr_ckl80_fw1 (.D0(1'b1), .D1(1'b0), .C0(clk2xsh), .C1(!clk2xsh), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(clk80[1]));");
	printv("\n");

#if defined(ALCT576) && defined(MIRROR)
    Wire (din_dly_int);

    printv("\n// delay line for din_dly signal, to allow for better hold time");
    printv("\nIODELAY2 #(");
    printv("\n    .COUNTER_WRAPAROUND (\"WRAPAROUND\"), // \"STAY_AT_LIMIT\" or \"WRAPAROUND\"");
    printv("\n    .DATA_RATE          (\"SDR\"), // \"SDR\" or \"DDR\"");
    printv("\n    .DELAY_SRC          (\"ODATAIN\"), // \"IO\", \"ODATAIN\" or \"IDATAIN\"");
    printv("\n    .IDELAY2_VALUE      (0), // Delay value when IDELAY_MODE=\"PCI\" (0-255)");
    printv("\n    .IDELAY_MODE        (\"NORMAL\"), // \"NORMAL\" or \"PCI\"");
    printv("\n    .IDELAY_TYPE        (\"DEFAULT\"), // \"FIXED\", \"DEFAULT\", \"VARIABLE_FROM_ZERO\", \"VARIABLE_FROM_HALF_MAX\" or \"DIFF_PHASE_DETECTOR\"");
    printv("\n    .IDELAY_VALUE       (0), // Amount of taps for fixed input delay (0-255)");
    printv("\n    .ODELAY_VALUE       (128), // Amount of taps fixed output delay (0-255)");
    printv("\n    .SERDES_MODE        (\"NONE\"), // \"NONE\", \"MASTER\" or \"SLAVE\"");
    printv("\n    .SIM_TAPDELAY_VALUE (75) // Per tap delay used for simulation in ps");
    printv("\n)");
    printv("\ndin_dly_del");
    printv("\n( ");
    printv("\n    .BUSY     (), // 1-bit output: Busy output after CAL");
    printv("\n    .DATAOUT  (), // 1-bit output: Delayed data output to ISERDES/input register");
    printv("\n    .DATAOUT2 (), // 1-bit output: Delayed data output to general FPGA fabric");
    printv("\n    .DOUT     (din_dly), // 1-bit output: Delayed data output");
    printv("\n    .TOUT     (), // 1-bit output: Delayed 3-state output");
    printv("\n    .CAL      (1'b0), // 1-bit input: Initiate calibration input");
    printv("\n    .CE       (1'b0), // 1-bit input: Enable INC input");
    printv("\n    .CLK      (clk2x), // 1-bit input: Clock input");
    printv("\n    .IDATAIN  (1'b0), // 1-bit input: Data input (connect to top-level port or I/O buffer)");
    printv("\n    .INC      (1'b0), // 1-bit input: Increment / decrement input");
    printv("\n    .IOCLK0   (clk2x), // 1-bit input: Input from the I/O clock network");
    printv("\n    .IOCLK1   (1'b0), // 1-bit input: Input from the I/O clock network");
    printv("\n    .ODATAIN  (din_dly_int), // 1-bit input: Output data input from output register or OSERDES2.");
    printv("\n    .RST      (1'b0), // 1-bit input: Reset to zero or 1/2 of total delay period");
    printv("\n    .T        (1'b1) // 1-bit input: 3-state input signal");
    printv("\n);\n");
#endif


#endif

#else
	clkb = clkp;
	clk80 = (clk2xsh, clk2xsh);
	clk40sh = (clksh, clksh);
	tck2b = tck2;
	alct_sn = SNout;
	mc_sn = SNout;
#endif

#ifndef S6
	dll2x.init("dll", "dll2x");
	dll2x (clkb, clk, clk2x_buff, clksh, clock_lac, locked); comment("// DLL for doubling the clock")

	dllsh.init("dll2", "dllsh");
	dllsh (clk2x_buff, clk2x, clk2xsh, !locked);   comment("// DLL for the double clock phase adjustment")
#else
	dll2x.init("dll", "dll2x");
    dll2x
    (
		clkb,
		clk,
		clksh,
	    clksh_inv,
		clk2x,
		clk2xsh,
		clock_lac,
		locked
	);

#endif
	
	comment("// output clocks to Muxes")

	assign mx_oe = 0; comment("// Mux OE")
	
	comment("// JTAG port instantiation")
	struct tm *ltm;
	time_t lt;
	lt = time(NULL);
	ltm = localtime(&lt);

    char sday[10], smonth[10], syear[10];

#ifdef DAQ01
    sprintf(sday,   "8'h%02d", ltm->tm_mday);
    sprintf(smonth, "8'h%02d", ltm->tm_mon+1);
    sprintf(syear,  "16'h%04d", ltm->tm_year+1900);
	assign virtex_id = ((Signal)smonth, sday, syear, ALCTTYPE, (Signal)ALCTPAR | ALCTFB | ALCTPN), // date and version numbers. 
#endif

#ifdef DAQ06
    sprintf(sday,   "5'd%d",  ltm->tm_mday);
    sprintf(smonth, "4'd%d",  ltm->tm_mon+1);
    sprintf(syear,  "12'd%d", ltm->tm_year+1900); 
#ifndef S6
    assign virtex_id = ((Signal)smonth, sday, syear, "1'h0", sl_cn_done, "1'b0"   , SP6, PB, RL, KE, MR, NP, BF, WGN, FWVER);
#else
    assign virtex_id = ((Signal)smonth, sday, syear, "1'h0", sl_cn_done, seu_error, SP6, PB, RL, KE, MR, NP, BF, WGN, FWVER);
#endif
#endif


	TAP.init("jtag", "TAP");
	TAP 
	(
		tck2b, tms2, tdi2, tdo2,
		HCmask, collmask,
		(cs_dly, settst_dly, rs_dly),
		ConfgReg,
		hmt_thresholds,
		TstPlsEn,
#if defined(ALCT576) && defined(MIRROR)
		din_dly_int,
#else
		din_dly, 
#endif
		dout_dly, 
		clk_dly, 
		input_dis,
		ConfgRegx,
		OSdata, OSre,
#ifdef S6
		adc_sck,		// Serial clock
		adc_sdi,		// Serial data to ADC
		adc_ncs,		// Chip select active low
		adc_sdo,		// Serial data from ADC
		adc_eoc,		// End of conversion
#endif
		hard_rst,
		jstate,
		virtex_id,
		TrigReg,
		SNout,
		SNin(sn_select),
		hcounters,
		clk
	);								

	comment("// disassemble the ConfigReg into parameters")
	assign drifttime     = ((Signal)"1'b0", ConfgReg(20,19));
	assign pretrig       =  ConfgReg(15,13); // threshold for coll tracks
	assign trig          =  ConfgReg(18,16); // pattern for coll tracks
	assign PromoteColl   = !ConfgReg(65);
	assign trig_mode     =  ConfgReg(1,0);
	assign fifo_tbins    =  ConfgReg(25,21);
	assign l1a_delay     =  ConfgReg(43,36);
	assign fifo_mode     =  ConfgReg(32,31);
	assign BoardID       =  ConfgReg(55,53);
	assign l1a_internal  =  ConfgReg(52);
#ifdef DAQ01
    assign l1a_window    =  ConfgReg(47,44);
#endif

#ifdef DAQ06
    assign l1a_window    =  ConfgReg(47,44) | "4'b1"; // make it odd - need that so DAQ format word count divides by 4 (one padding LCT included)
#endif

    assign l1a_offset    =  ConfgReg(51,48);
    assign fifo_pretrig  =  ConfgReg(30,26);
    assign ext_trig_en   =  ConfgReg(2);
	assign trig_info_en  =  ConfgReg(67);
	assign inject        =  ConfgReg(4);
	assign sn_select     =  ConfgReg(68);
	assign lhc_cycle_sel =  ConfgReg(60);
	assign send_empty    =  ConfgReg(3); comment("// set this to 1 to enable sending DAQ for empty events")
    assign bxc_offset    =  ((Signal)"4'b0", ConfgReg(12,5)); comment("// bunch crossing counter offset (8 LSBs)")

#ifdef DAQ06
	assign config_report = ConfgReg(64);
#endif
	// pretrig and trig for acc tracks. Use those only if user set them to valid values (!= 0)
	assign acc_pretrig   = ifelse(ConfgReg(35, 33) == 0, pretrig, ConfgReg(35, 33)); // threshold for acc tracks
	assign acc_trig      = ifelse(ConfgReg(58, 56) == 0, trig ,   ConfgReg(58, 56)); // pattern for acc tracks

	assign zero_suppress = ConfgReg(66); // enable zero suppression for DAQ

	// disassemble extended config reg into parameters
	assign os_enable     = ConfgRegx(0); // output storage enable

	
#ifdef ME11BP
	int li[] = {2, 3, 0, 1, 4, 5};
	int lf[] = {0, 1, 4, 5, 2, 3};
#else
#ifdef ME11FP
	int li[] = {2, 3, 4, 5, 0, 1};
	int lf[] = {4, 5, 0, 1, 2, 3};
#else
#ifdef ME11BN
	int li[] = {3, 2, 5, 4, 1, 0};
	int lf[] = {5, 4, 1, 0, 3, 2};
#else
#ifdef ME11FN
	int li[] = {3, 2, 1, 0, 5, 4};
	int lf[] = {1, 0, 5, 4, 3, 2};
#else
	int li[] = {3, 2, 1, 0, 5, 4};
	int lf[] = {1, 0, 5, 4, 3, 2};
#endif
#endif
#endif
#endif

	comment("// demux the inputs")
	always (posedge (clk2x))	
	begin

		for (i = 0; i < LCTS; i++)
		{
			for (j = 0; j < LYN; j++)
			{
				// these complicated formulas are due to the complicated input routing and multiplexing scheme
#if (defined ME11BN || defined ME11FP)
				wgi = (j < 2) ? (LCTS-i-1) * 16 + 8 : (LCTS-i-1) * 16;
				wgf = (j < 4) ? (LCTS-i-1) * 16 + 8 : (LCTS-i-1) * 16;
#else
// routing for ME11FN and ME11BP matches the default routing below
				wgi = (j < 2) ? i * 16 : i * 16 + 8;
				wgf = (j < 4) ? i * 16 : i * 16 + 8;
#endif

				ly[lf[j]](wgf+7, wgf) = flip(ly[li[j]](wgi+7, wgi));
				ly[li[j]](wgi+7, wgi) = (*lct[i])(j*8+7, j*8);
			}
		}

		L1A = SyncAdb; 
		SyncAdb = L1A_SyncAdb_r; 
		
		ext_inject = ext_trig; 
		ext_trig = ext_inject_trig_r; 
		
		ccb_brcstd(3,0) = ccb_brcstd(7,4); 
		ccb_brcstd(7,4) = ccb_brcstm_r;
		
		brcst_str1d = subaddr_str; 
		subaddr_str = bstr1_subaddr_r;
		
		dout_str = bx0d;
		bx0d = dout_bx0_r;

		seq_cmd(0) = seq_cmd(2);
		seq_cmd(2) = seq_cmd02_r;

		seq_cmd(1) = seq_cmd(3);
		seq_cmd(3) = seq_cmd13_r;

		rsv_in(0) = rsv_in(2);
		rsv_in(2) = rsv_in02_r;

		rsv_in(1) = rsv_in(3);
		rsv_in(3) = rsv_in13_r;

		L1A_SyncAdb_r     = L1A_SyncAdb;
		ext_inject_trig_r = ext_inject_trig;
		ccb_brcstm_r      = ccb_brcstm;
		bstr1_subaddr_r   = bstr1_subaddr;
		dout_bx0_r        = dout_bx0;
		rsv_in02_r        = rsv_in02;
		rsv_in13_r        = rsv_in13;
		seq_cmd02_r       = seq_cmd02;
		seq_cmd13_r       = seq_cmd13;

	end

    always (posedge (clk))
   	begin

		(SyncAdb2, L1A2, ext_trig2, ext_inject2, bx0_2, dout_str2, subaddr_str2, brcst_str2, ccb_brcst2) = dec_out;
		ecc_err_5 = (ecc_error, Signal (3, 0));

		L1A1         = L1A;
		SyncAdb1     = SyncAdb;
		ext_inject1  = ext_inject;
		ext_trig1    = ext_trig;
		subaddr_str1 = subaddr_str;
		dout_str1    = dout_str;
		rsv_in1      = rsv_in;

		ccb_brcst   = ccb_brcstd;
		brcst_str1  = brcst_str1d;
		bx0         = bx0d;

		seq_cmd_r = seq_cmd;

    end

	// error correction on inputs
	ecc16_dec.init("ecc16_decoder", "ecc16_dec");
	ecc16_dec
	(
		(SyncAdb1, L1A1, ext_trig1, ext_inject1, bx0, dout_str1, subaddr_str1, brcst_str1, ccb_brcst), // dec_in
		(seq_cmd_r(3), seq_cmd_r(1), rsv_in1), // parity_in
		Signal (1,1), // ecc_en, always enabled
		dec_out,
		ecc_error
	);

	ecc32_en.init("ecc32_encode", "ecc32_en");
	ecc32_en
	(
		(Signal(3,1), ttc_bx0,   actv_feb_fg,   bxn_mux,   ln,   l(1,0), lfa,   validl,   hn,   h(1,0), hfa,   validh),
		(dummy3,      ttc_bx0_e, actv_feb_fg_e, bxn_mux_e, ln_e, l_e,    lfa_e, validl_e, hn_e, h_e,    hfa_e, validh_e),
		parity_out,
		clk	
	);


	assign settst_dlyp = settst_dly;
		
	comment("// process the data")
#ifdef RED_LAT
#ifdef TRIGWP
	core.init("trigger_wp", "core");
#else
	core.init("trigger_rl", "core");
#endif
#else
	core.init("trigger", "core");
#endif

	core
	(
		ly[0], ly[1], ly[2], ly[3], ly[4], ly[5],
		collmask,
		PromoteColl,
		h, hn, hfa, hpatb, validh,
		l, ln, lfa, lpatb, validl,
    shower_int, 
		drifttime, 
		pretrig, 
		trig, 
		trig_mode,
#ifdef RED_LAT
		acc_pretrig, 
		acc_trig, // acc track can have separate parameters only in reduced latency version
#endif
		actv_feb_fg,
		fmm_trig_stop,
	
		input_disr,
		ext_trig_en,
		ext_trig2,
		inject,
		ext_inject2,
		HCmask,
		hmt_thresholds,
		clk
	);	


	sync.init("synchro", "sync");
	sync
	(
		hard_rst,
		os_enable,
		ccb_brcst2(5,0), // ccb command
		brcst_str2,     // ccb command strobe
		bx0_2,
		bxc_offset,
		lhc_cycle_sel,
		bxn,
		l1a_cnt_reset,
		fmm_trig_stop,
		ttc_l1reset,
		ttc_bx0,
		ttc_start_trigger,
		ttc_stop_trigger,
		bxn_before_reset,
		clk
	);

	always (posedge (clk)) 
	begin
#ifdef VGEN		
		comment("// next two lines are responsible for the reset of the logic on power-up.")
		hard_rst = hrstcnt(0);
		hrstcnt = ((Signal)"1'b1", hrstcnt(14,1));
#else
		hard_rst = 1; // in simulation mode we don't need hard reset
#endif
		
		input_disr = input_dis; comment("// synchronize input_dis signal with the clock")
		
		tst_plss = tst_pls_en(0) && !tst_pls_en(1); comment("//  pulse tst_pls output only on rising edge of TstPlsEn")
		tst_pls_en(1) = tst_pls_en(0);
		tst_pls_en(0) = TstPlsEn;
	end

	assign tst_pls = 
		ifelse (TrigReg(4,2) == 0, tst_plss,
		ifelse (TrigReg(4,2) == 1, SyncAdb2,
		ifelse (TrigReg(4,2) == 2, AsyncAdb,
		ifelse (TrigReg(4,2) == 3, tp_strt_ext, 
		ifelse (TrigReg(4,2) == 4, !tst_plss,
		ifelse (TrigReg(4,2) == 5, !SyncAdb2,
		ifelse (TrigReg(4,2) == 6, !AsyncAdb,
		ifelse (TrigReg(4,2) == 7, !tp_strt_ext, 0))))))));


#ifdef DAQ01
	ReadOutFifo.init("fifo", "ReadOutFifo");
	ReadOutFifo
	(
    	ly[0], ly[1], ly[2], ly[3], ly[4], ly[5],
	    (hn, validh, hfa, h),
    	(ln, validl, lfa, l),
	    bxn,                        
    	fifo_tbins,                 
	    daqo,                        
    	l1a_delay,                   
	    fifo_pretrig,
    	fifo_mode,                  
	    BoardID, 
    	"4'd0",                  // CSCID
	    L1A2,
    	hard_rst && (!l1a_cnt_reset),
    	l1a_internal, l1a_window, l1a_offset,
		NoSpaceForDAQ,
		l1awindowTP, l1aTP, validhd,
		send_empty,
		bxn_before_reset,
		trig, acc_trig,
	    clk
	);
#endif

#ifdef DAQ06
	Module (daq_06);
	daq_06
	(
    	ly[0], ly[1], ly[2], ly[3], ly[4], ly[5],
	    (hn, validh, hfa, h),
    	(ln, validl, lfa, l),
			shower_int,
	    bxn,                        
    	fifo_tbins,                 
	    daqo,                        
    	l1a_delay,                   
	    fifo_pretrig,
    	fifo_mode,                  
	    L1A2,
    	hard_rst && (!l1a_cnt_reset),
    	l1a_internal, 
		l1a_window, 
		l1a_offset,
		l1awindowTP, 
		l1aTP, 
		validhd,
		send_empty,
		config_report,
		bxn_before_reset,
		virtex_id,
		TrigReg(4,2),
		ConfgReg,
		HCmask,
		collmask,
		zero_suppress,
		fmm_trig_stop,
		seu_error,
	    clk
	);
#endif

	// loopback module
	lpback.init ("loopback", "lpback");
	lpback
	(
		// command from TMB
		seq_cmd_r,
		// inputs
		(rsv_in1(1,0), seq_cmd_r(1,0), L1A1,     ext_inject1, dout_str1, brcst_str1, ccb_brcst(3,0)),
		(rsv_in1(3,2), seq_cmd_r(3,2), SyncAdb1, ext_trig1,   bx0,     subaddr_str1, ccb_brcst(7,4)),

		// loopback outputs
		alct_tx_1st_tpat,
		alct_tx_2nd_tpat,
		// flag that indicates loopback mode
		alct_sync_mode,

		clk		
	);

	assign send_bxn = (validh || validl || actv_feb_fg) && !alct_sync_mode;
	// sending shower bits instead of bxn[2:1], according to 
  // Sven's message from 2021-10-29
	assign shower_bits = (Signal(2,0), shower_int, bxn(0)); // BXN[4:3] set to zeros
	assign bxn_mux = ifelse(send_bxn, shower_bits, ecc_err_5);

	// mux the outputs
	always (posedge (clk2x))
	begin
		If (clock_lac == 0)
		begin
  	                valid             = alct_tx_1st_tpat_r(4);      // validh_e
			amu               = alct_tx_1st_tpat_r(5); 	// hfa_e 
			quality           = alct_tx_1st_tpat_r(7,6); 	// h_e  
			keyp              = alct_tx_1st_tpat_r(14,8);   // hn_e
			lctSpec_FirstFr   = alct_tx_1st_tpat_r(25); 	// daqo(14) 
			dduSpec_LastFr    = alct_tx_1st_tpat_r(28); 	// daqo(15) 
			bxn_wrFifo        = alct_tx_1st_tpat_r(17,15);  // bxn_mux_e(2,0)
			daqData           = alct_tx_1st_tpat_r(24,18);  // daqo(6,0)
			seq_seu           = alct_tx_1st_tpat_r(27,26);  // parity_out(1,0)
			activeFeb_cfgDone = alct_tx_1st_tpat_r(3); 	// actv_feb_fg_e 
			rsrvd_out(0)      = alct_tx_1st_tpat_r(1); 	// parity_out(4) 
			rsrvd_out(1)      = alct_tx_1st_tpat_r(2);      // parity_out(5)
		end
		Else
		begin
			valid             = alct_tx_2nd_tpat_r(4);	// validl_e 
			amu               = alct_tx_2nd_tpat_r(5); 	// lfa_e  
			quality           = alct_tx_2nd_tpat_r(7,6); 	// l_e  
			keyp              = alct_tx_2nd_tpat_r(14,8);   // ln_e 
			lctSpec_FirstFr   = alct_tx_2nd_tpat_r(25); 	// daqo(17)  
			dduSpec_LastFr    = alct_tx_2nd_tpat_r(28); 	// daqo(16)  
			bxn_wrFifo        = alct_tx_2nd_tpat_r(17,15);  // {daqo(18), bxn_mux_e(4,3)}
			daqData           = alct_tx_2nd_tpat_r(24,18);  // daqo(13,7)
			seq_seu           = alct_tx_2nd_tpat_r(27,26);  // parity_out(3,2)
			activeFeb_cfgDone = alct_tx_2nd_tpat_r(3); 	// 1
			rsrvd_out(0)      = alct_tx_2nd_tpat_r(1); 	// parity_out(6)  
			rsrvd_out(1)      = alct_tx_2nd_tpat_r(2);      // ttc_bx0_e
		end

		If (alct_sync_mode)
		begin
	        alct_tx_1st_tpat_r = alct_tx_1st_tpat; 
			alct_tx_2nd_tpat_r = alct_tx_2nd_tpat;
		end
		Else
		begin
	        alct_tx_1st_tpat_r = 
		(
		 daqo(15),       // 28	  
		 parity_out(1,0),// 27:26 
		 daqo(14),       // 25	  
		 daqo(6,0),      // 24:18       
		 bxn_mux_e(2,0), // 17:15 
		 hn_e,           // 14:8  
		 h_e,            // 7:6	  
		 hfa_e,          // 5	  
		 validh_e,       // 4 	  
		 actv_feb_fg_e,  // 3	  
		 parity_out(5),  // 2	  
		 parity_out(4)   // 1     
		 );

		//		                                                                             7     2
		//

		alct_tx_2nd_tpat_r = 
		(
		 daqo(16),        // 28	  
		 parity_out(3,2), // 27:26 
		 daqo(17), 	  // 25	  
		 daqo(13,7), 	  // 24:18 
		 daqo(18), 	  // 17 
		 bxn_mux_e(4,3),  // 16:15  
		 ln_e, 		  // 14:8	  
		 l_e, 		  // 7:6	  
		 lfa_e, 	  // 5 	  
		 validl_e, 	  // 4	  
		 Signal(1,1),     // 3	    
		 ttc_bx0_e, 	  // 2     
		 parity_out(6)    // 1
		 );

		end
		
	end

	comment("// output data storage")
	of.init("outfifo", "of");
	of 
	(
		OSINPUT, // all outputs of the ALCT go to this fifo before output mux
		(os_enable && !input_disr),  // when the input clock and output storage are enabled, enable writing into this storage, if there are valid tracks or daq readout
		clk,
		OSre,       // read enable (from JTAG interface)
		tck2b,     // read clock is a JTAG clock
		OSdata(OSSIZE-1, 0),  // data for readout via JTAG
		OSdata(OSSIZE),   // empty flag goes as MSB
		trig_info_en,
		NoSpaceForDAQ // if 1 - there is no space for one more DAQ readout
	);
/*
	Module(hitcnt);
	hitcnt 
	(
		(validh, hn),
		(validl, ln),
		hcounters,
		clk
	);
*/
	comment ("// Test point outputs")
	assign TP0 = 
	(
		seq_cmd_r,
		daqo(18), // write fifo signal
		validhd, // validh delayed
		validh,
		validl,
		ttc_l1reset,	
		fmm_trig_stop, 
		ttc_bx0, 
		ttc_start_trigger, 
		ttc_stop_trigger,
		l1awindowTP, 
		l1aTP, 
		~input_disr
	);

	assign TP1 = 
	(
		alct_tx_2nd_tpat_r(16,1),
		alct_tx_1st_tpat_r(16,1)
	);

#ifdef S6
	printv("\n\tPOST_CRC_INTERNAL p_c_i (.CRCERROR(seu_error));\n");
#else
	assign seu_error = "1'b0";
#endif

#ifdef LX100
	// module for transmission via GBTX
        Module (gbtx);
        gbtx
	(
	   daqo,
	   clk,
	   
	   elink_p,
	   elink_n,
	   gbt_tx_datavalid,
	   gbt_clk40_p,
	   gbt_clk40_n,
	   gbt_txrdy,
	   
       !hard_rst
        );
#endif

#ifdef LX150T
	// module for transmission via serial link
	Module (gtp_tux);
	gtp_tux
	(
	    daqo,
	    clk,

	    tx_p,
	    tx_n,
	    refclk_p,
	    refclk_n,

        !hard_rst
	);
#endif

endmodule
}



Signal Flip::operator()(Signal d)
{
initio
	d.input(7, 0, "d");
beginfunction
    begin
		result(0) = d(7);            
        result(1) = d(6);    
        result(2) = d(5);    
        result(3) = d(4);    
        result(4) = d(3);    
        result(5) = d(2);    
        result(6) = d(1);    
        result(7) = d(0);    
    end
endfunction
}

