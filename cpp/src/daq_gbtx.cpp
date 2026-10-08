#include "daq_gbtx.h"

// Optical DAQ format (see Table 11 of the ALCT firmware document)
// Each word is 28 bits: Link 1 = bits [27:14], Link 0 = bits [13:0]
//
//  header      0x5E750FF
//  word 1      {2'b01, BXN at L1A[11:0],        2'b00, L1A_counter[11:0]}
//  word 2      {2'b01, Readout_counter[11:0],   2'b00, BXN[11:0]}
//  word 3      {2'b01, BXN_before_reset[11:0],  2'b00, 3'b0, LCT_bins[3:0], Raw_bins[4:0]}
//  word 4      {1'b0, TrReg[4:2], wp, 2'b0, ke, mr, n/p, b/f, wgn[2:0],
//               2'b00, FW_version[5:0], sp6, cp, seu, lof, rof, zse}
//  LCT bins    {2'b00, ALCT1[11:0], shower[1:0], ALCT0[11:0]}       one word per time bin
//  raw hits    {zsf[1:0], layer[n*24+23:n*24+12], 2'b00, layer[n*24+11:n*24]}
//  evener      0xDECEA5E
//  CRC         {3'b010, 3'b000, CRC[21:0]}
//  word count  {3'b010, 14'b0, word_count[10:0]}
//
// Between data blocks, idle words 0x051E57A are transmitted, at least 8 of them.
// Each block is padded to a multiple of 4 words (4 x 32 bits = 128 bits on the receiver side),
// and blocks always start at the same position within the 4-word group.

// words per layer, 24 bits in each
#define LAYER_PARTS (LYWG/24 + ((LYWG%24 == 0) ? 0 : 1))
// layer register width, padded to multiple of 24 bits
#define LYTW (LAYER_PARTS*24)

#if ((LYWG % 24) != 0)
	#define PADLY(a) (Signal(LYTW - LYWG, 0), (a))
#else
	#define PADLY(a) (a)
#endif

// wide pattern version flag for DAQ
#ifdef TRIGWP
	#define WP "1'b1"
#else
	#define WP "1'b0"
#endif

// special words
#define W_HEADER "28'h5E750FF"
#define W_EVENER "28'hDECEA5E"
#define W_IDLE   "28'h051E57A"
#define W_ZSF1   "28'h4000000" // zsf = 1, replaces entire layer without hits
#define W_PAD    "28'hC000000" // zsf = 3, padding to multiple of 4 words

// delay of the L1A fifo write. Must be longer than the longest raw hit 
// or LCT write window (31 clocks), so the readout logic running at 160M never 
// catches up with the data still being written at 40M
#define L1A_FIFO_DELAY "6'd34"

// ALCT word for DAQ: {key[6:0], patb, amu, quality[1:0], valid}
#define ALCT12(a) (a)(10,4), "1'b0", (a)(2,0), tvalid(a)

// prepare raw hit section transmission
#define START_RAW \
	raw_cnt = 0; \
	ly_cnt = 0; \
	wg_cnt = 0; 

// load one time bin of raw hits from raw memory, advance raw memory address
#define LOAD_RAW \
	for (i = 0; i < 6; i++) lyt[i] = PADLY(lym[i]); \
	lzt = lyzm; \
	raw_adr++; 

// next state after the last word of LCT or raw hits, or after header if there are none
// frame_count(1,0) == 1 means that with 3 trailer words the block length will be multiple of 4
#define AFTER_DATA ifelse(frame_count(1,0) == 1, SEND_EVENER, SEND_RAWPAD)

void daq_gbtx::operator()
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
	Signal config_report_i,
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
)
{
initio

	Input_(ly0, LYWG-1,  0);
	Input_(ly1, LYWG-1,  0);
	Input_(ly2, LYWG-1,  0);
	Input_(ly3, LYWG-1,  0);
	Input_(ly4, LYWG-1,  0);
	Input_(ly5, LYWG-1,  0);
    Input_(best1, BESTBITS-1,  0);	// best track parameters 
	Input_(best2, BESTBITS-1,  0);	// second best track parameters {key, patb, amu, quality}
	Input_(shower_int, 1, 0); // in-time shower bits
    Input_(bxn, 11,  0);		// bx number
    Input_(fifo_tbins, 4,  0);	// length of the dump for one L1A
    Output_(daqp, 27, 0);		// 28-bit output word to optical link, clk160 domain
    OutReg (daq_gbtx_valid);	// 1 while daqp carries a DAQ block: from header through word count, aligned with daqp
    Input_(l1a_delay, 7,  0); 	// time back to get the raw hits corresponding to this to L1A
    Input_(fifo_pretrig, 4,  0); // delay of the input raw hit data to compensate for the processing and pretriggering
    Input_(fifo_mode, 1,  0);	// bits from configuration register defining the raw hits dump mode
    Input (L1A); 
    Input (hard_rst);
	Input (l1a_internal);	// if ==1 then L1A will be generated internally for each valid track found
    Input_(l1a_window,3, 0);  // window in which to look for valid tracks in case of L1A
    Input_(l1a_offset,3, 0);  // initial value for l1a counter

	Output(L1AWindow); // test points
	Output(l1aTP);
	Output(validhd);

	Input (send_empty); // set this to 1 to report DAQ for empty events
	Input (config_report_i); // not used in optical format
	Input_(bxn_before_reset, MXBXN-1, 0);
	Input_(virtex_id, IDSIZE-1, 0); // not used in optical format
	Input_(trig_reg, 2, 0);
	Input_(config_reg, CRSIZE-1, 0); // not used in optical format
	Input_(hot_channel_mask, HCMASKBITS-1, 0); // not used in optical format
	Input_(collision_mask, COLLMASKBITS-1, 0); // not used in optical format
	Input (zero_suppress);
	Input (trig_stop);
	Input (seu_error);
    Input (clk); // 40M TTC clock
	Input (clk160); // 160M link clock

beginmodule

	// ---------------- 40M (clk) domain signals

    Wire_(best1e, BESTBITS-1,  0);	// early best track parameters
	Wire_(best2e, BESTBITS-1,  0);
    Wire_(bxne, 11,  0);			// early bx number
	Wire_(shower_e, 1, 0);

    Reg_(best1d, BESTBITS-1,  0);	// best track parameters delayed
	Reg_(best2d, BESTBITS-1,  0);
    Reg_(bxnd, 11,  0);			// bx number delayed
	Reg_ (shower_d, 1, 0);

	Wire__(lyd, LYWG-1, 0, 5, 0); // raw hits delayed
	Wire_(lyzd, 5, 0); // flags showing that delayed layer has no hits

	Wire_(l1a_int_delay, 7, 0);
	Wire (l1a_proc);
	Wire (best_we);
	Wire (raw_we);
	Wire (valor);
	Reg  (l1a_procr);
	Reg_ (bxnr, 11, 0); // delayed bxn

	Reg_(best_adw,  7, 0); // memory write addresses
	Reg_(raw_adw,  7, 0);
	Wire (best_full);
	Wire (raw_full);
	Wire_(l1a_in_count, 11, 0); // l1a counter
	Reg_ (rst_cnt, 6, 0); // counts clocks after reset, blocks L1A fifo writes until delay line is flushed

	// L1A info delayed before writing into L1A fifos
	Wire (l1a_procd);
	Wire_(l1a_in_countd, 11, 0);
	Wire (best_fulld);
	Wire (raw_fulld);
	Wire_(best_adwd, 7, 0);
	Wire_(raw_adwd, 7, 0);
	Wire_(bxnrd, 11, 0);
	Wire (l1a_fifo_we);

	Wire (l1a_fifo_full);
	Wire (l1a_bxn_fifo_full); // unused

	// ---------------- 160M (clk160) domain signals

	Reg (l1a_fifo_re); 
	Wire(l1a_fifo_empty);
	Wire(l1a_bxn_fifo_empty); // unused
	Wire_(l1a_in_countf, 11, 0); 
	Wire (best_fullf);
	Wire (raw_fullf);
	Wire_(best_adwf, 7, 0); 
	Wire_(raw_adwf,  7, 0);
	Wire_(bxn_l1a, 11, 0); // bxn at the l1a arrival time

    Wire_(best1m, BESTBITS-1,  0);	// best track parameters read from daq memory
	Wire_(best2m, BESTBITS-1,  0);
    Wire_(bxnm, 11,  0);			// bx number read from daq memory
	Wire_(shower_m, 1, 0);
	Wire__(lym, LYWG-1, 0, 5, 0); // raw hits read from daq memory
	Wire_(lyzm, 5, 0); // layer zero flags read from daq memory

	Reg_(best_adr,  7, 0);
	Reg_(best_adb,  7, 0);
	Reg_(raw_adr,  7, 0);
	Reg_(raw_adb,  7, 0);

	Reg_(readout_count, 11, 0); // shows how many readouts have been actually shipped
	Reg_(state, 4, 0); // state machine state
	Reg_(idle_cnt, 2, 0); // idle words counter between blocks
	Reg_(wphase, 1, 0); // word position within 4-word group

	Reg_(best_cnt, 3, 0); // LCT bin counter
	Reg_(lct_bins, 3, 0); // how many lct bins actually sent
	Reg_(lct_bins_report, 3, 0); // lct bins number to report in daq (includes padding)
	Reg_(raw_bins, 4, 0); // how many raw bins sent

	Reg__(lyt, LYTW-1, 0, 5, 0); // one complete time bin for tx; current layer is always in lyt[0]
	Reg_(lzt, 5, 0); // layer zero flags for current time bin; current layer is always in lzt[0]
	Reg_(raw_cnt, 5, 0); // raw time bins counter
	Reg_(ly_cnt, 2, 0); // layer counter for tx
	Reg_(wg_cnt, 3, 0); // 24-bit layer part counter for tx
	Reg (ly_zero);

	Reg_(crccalc, 2, 0);// tells when to calculate CRC
    Reg_(daqw, 27, 0);	// output to daq before crc
	Reg  (daqv_w); // block valid flag aligned with daqw, before crc stage
	Reg_(frame_count, 10, 0);

	Wire_(fwver, 5, 0);
	assign fwver = FWVER;

	int i;
	
	assign l1a_int_delay = l1a_delay - 1;
	assign L1AWindow = best_we;
	assign l1aTP = l1a_proc;
	assign validhd = tvalid(best1d); // output to test point

	// ======================== 40M domain

	// best_delay module delays best track data + bxn for l1a_delay clocks
	Module (best_delay);  
 	best_delay.bwd = BESTBITS*2 + 12 + 2; 
	best_delay.bwad = 8;
	best_delay.ram_style = 1; // block
	best_delay.max_l1a_window = 16;
	best_delay
	(
	 (shower_int, best1,  best2,  bxn),
	 (shower_e,   best1e, best2e, bxne),
		l1a_int_delay, // delay best tracks one clock less because l1a_maker wants to know early to generate internal l1a
		hard_rst,
	    l1a_procr,
	    l1a_window,
	    (tvalid(best1) || (shower_int > 1)), // valid signal, either track or nominal or tight shower
	    valor,
		trig_stop,
		clk
	);

	// raw_delay module delays raw hit data for l1a_delay+fifo_pretrig clocks
	Module (raw_delay);
 	raw_delay.bwd = HCMASKBITS; 
	raw_delay.bwad = 8;
	raw_delay.ram_style = 1; // block
	raw_delay
	(
		(ly0,  ly1,  ly2,  ly3,  ly4,  ly5),
		(lyd[0], lyd[1], lyd[2], lyd[3], lyd[4], lyd[5]),
		(l1a_delay + fifo_pretrig),
		hard_rst,
		trig_stop,
		clk
	);

	// layer zero flags are calculated here at 40M and stored with raw hits, 
	// so the 160M readout logic does not need wide comparators
	assign lyzd = 
	(
		lyd[5] == 0, 
		lyd[4] == 0, 
		lyd[3] == 0, 
		lyd[2] == 0, 
		lyd[1] == 0, 
		lyd[0] == 0
	);

	// detects rising edge on L1A, makes l1a_proc, best_we and raw_we signals
	Module (l1a_maker_);
	l1a_maker_
	(
		L1A,
		valor,
		(tvalid(best1e) || (shower_e > 1)), // valid track or nominal|tight shower
		l1a_proc,
		l1a_window,
		fifo_tbins,
		l1a_fifo_full,
		best_full,
		raw_full,
		fifo_mode != 0,
		best_we,
		raw_we,
		l1a_internal,
        l1a_in_count,
        l1a_offset,
		send_empty,
		hard_rst,
		clk
	);

	// L1A info is delayed before being written into L1A fifos.
	// This guarantees that by the time the 160M readout logic sees the L1A, 
	// all LCTs and raw hits for it are already written into memories.
	Module (l1a_dly);
	l1a_dly.bwd = 1 + 12 + 1 + 1 + 8 + 8 + 12;
	l1a_dly.bwad = 6;
	l1a_dly.ram_style = 0; // distributed
	l1a_dly
	(
		(l1a_procr, l1a_in_count,  best_full,  raw_full,  best_adw,  raw_adw,  bxnr),
		(l1a_procd, l1a_in_countd, best_fulld, raw_fulld, best_adwd, raw_adwd, bxnrd),
		(Signal)L1A_FIFO_DELAY,
		(Signal)"1'b1",
		(Signal)"1'b0",
		clk
	);

	// ignore delay line output for a while after reset, until it's flushed
	assign l1a_fifo_we = l1a_procd && rst_cnt(6);

	always (posedge (clk))
	begin
	  	If (!hard_rst)
	  	begin
			best_adw = 1;
			raw_adw = 1;
			l1a_procr = 0;
			rst_cnt = 0;
	  	end
		Else
		begin
			If (best_we) best_adw++;
			If (raw_we)  raw_adw++;
			If (!rst_cnt(6)) rst_cnt++;
			l1a_procr = l1a_proc;
			// add one clock's delay to best tracks and shower to align them with raw hits
			(shower_d, best1d, best2d, bxnd) = (shower_e, best1e, best2e, bxne); 
			bxnr = bxn; // delay bxn to complensate for L1A transition detection time
		end
	end

	// ======================== 40M -> 160M crossing

	// L1A fifo stores best track and raw hit memory addresses for every L1A
	Module (l1a_fifo);
	l1a_fifo.bwd = 30; // to accomodate both 8-bit addresses and both full flags, and l1a counter
	l1a_fifo.bwad = 8; 
	l1a_fifo
	(
		(l1a_in_countd, best_fulld, raw_fulld, best_adwd, raw_adwd),
		(l1a_in_countf, best_fullf, raw_fullf, best_adwf, raw_adwf),
		l1a_fifo_we,
		l1a_fifo_re,
		!hard_rst,
		l1a_fifo_empty,
		l1a_fifo_full,
		clk,
		clk160
	);

	// L1A bxn fifo stores bxn of L1A arrival 
	Module (l1a_bxn_fifo);
	l1a_bxn_fifo.bwd = 12;
	l1a_bxn_fifo.bwad = 8; 
	// using distributed always trying to fix the bnx at the time of L1a bug
	l1a_bxn_fifo.distributed = 1;
	l1a_bxn_fifo
	(
		bxnrd,
		bxn_l1a,
		l1a_fifo_we,
		l1a_fifo_re,
		!hard_rst,
		l1a_bxn_fifo_empty,
		l1a_bxn_fifo_full,
		clk,
		clk160
	);

	// best tracks and showers to be reported in DAQ sequence are stored in this memory
	Module (best_memory);
 	best_memory.bwd = BESTBITS*2 + 12 + 2; 
	best_memory.bwad = 8;
	best_memory
	(
		best_adw,
		best_adr,
		best_adb,
		(shower_d, best1d, best2d, bxnd),
		(shower_m, best1m, best2m, bxnm),
		best_we,
		((Signal)"4'b0", l1a_window),
		best_full,
		clk,
		clk160
	);

	// raw hits to be reported in DAQ sequence are stored in this memory, along with layer zero flags
	Module (raw_memory);
 	raw_memory.bwd = HCMASKBITS + 6; 
	raw_memory.bwad = 8;
	raw_memory
	(
		raw_adw,
		raw_adr,
		raw_adb,
		(lyzd, lyd[0], lyd[1], lyd[2], lyd[3], lyd[4], lyd[5]),
		(lyzm, lym[0], lym[1], lym[2], lym[3], lym[4], lym[5]),
		raw_we,
		((Signal)"3'b0", fifo_tbins),
		raw_full,
		clk,
		clk160
	);

	// ======================== 160M domain

	// CRC generator, also inserts CRC word into data stream
	Module (crcgen);
	crcgen
	(
		daqw, 
		daqp, 
		crccalc, 
		clk160
	);

	enum  // machine states
	{
		IDLE 			,
		WAIT_L1A 		,
		READ_L1A 		,
		WAIT_BEST_RAW	,
		CHECK_LCTS 		,

		SEND_HEADER 	,
		SEND_W1			, // BXN at L1A, L1A counter
		SEND_W2			, // readout counter, BXN
		SEND_W3			, // BXN before reset, bin numbers
		SEND_W4			, // flags, fw version

		SEND_LCTBINS	,
		SEND_RAWBINS	,
		SEND_RAWPAD     ,
		SEND_EVENER		,
		SEND_CRC		,
		SEND_FRAME_CNT
	};

	// Notes on memory read timing:
	// daq_memory_2clk has 2 clk160 cycles of read latency, 
	// so data for address set in cycle N are available in cycle N+2.
	//
	// best memory: address set to the start of the block in CHECK_LCTS, 
	// data used in SEND_W2 (BXN) and then one LCT bin per word, starting 
	// at SEND_W4+1. Address is advanced starting from SEND_W3, 
	// so it's always 2 words ahead.
	//
	// raw memory: address is set in READ_L1A, then advanced each time 
	// a time bin is loaded into lyt. Each time bin takes at least 6 words 
	// (all layers suppressed), so the next bin is always ready.

	always (posedge (clk160))
	begin
		wphase++;

		// crcgen adds one clock of delay to daqw, so the valid flag is delayed by one clock too.
		// This aligns daq_gbtx_valid with daqp, including the CRC word inserted by crcgen.
		daq_gbtx_valid = daqv_w;

	  	If (!hard_rst)
	  	begin
		  	readout_count = l1a_offset;
          	state = IDLE;
			best_adb = 0;
			raw_adb = 0;
			crccalc = 0;
			idle_cnt = 0;
			daqw = W_IDLE;
			daqv_w = 0;
	  	end
		Else
		begin

			// block valid: set in states that output header through word count into daqw
			daqv_w = (state >= SEND_HEADER) && (state <= SEND_FRAME_CNT);

			// daq state machine
			daqw = W_IDLE;
			l1a_fifo_re = 0;
			begincase (state)
				case1(IDLE)
				begin
					crccalc = 0; // don't calculate or output any crc
					// start new readout only if: 
					// there is L1A to process,
					// enough idle words sent since last block,
					// header will be at position 0 within 4-word group (it is 5 clocks away)
					If (!l1a_fifo_empty && idle_cnt == 4 && wphase == 3)
					begin
						state = WAIT_L1A;
						l1a_fifo_re = 1;
					end
					If (idle_cnt != 4) idle_cnt++;
				end

				case1(WAIT_L1A) // let l1a fifo spit out info
				begin
					state = READ_L1A;
				end

				case1(READ_L1A) // pick up info from l1a fifo
				begin
					raw_adr  = raw_adwf;
					raw_adb  = raw_adwf;
					state = WAIT_BEST_RAW;
				end

				case1(WAIT_BEST_RAW) 
				begin
					state = CHECK_LCTS;
				end

				case1(CHECK_LCTS) 
				begin
					best_adr = best_adwf; // set best memory address to start of this l1a
                    best_adb = best_adwf;
					best_cnt = 0;
					frame_count = 0;
					// L1A was checked for LCTs before being stored, so always report LCT bins unless overflow
					lct_bins = ifelse(best_fullf, 0, l1a_window);
					raw_bins = ifelse((fifo_mode != 0) && !raw_fullf, fifo_tbins, 0);
					// lct_bins+1 reported to account for padding
					lct_bins_report = ifelse(lct_bins != 0, lct_bins + "4'b1", "4'b0");
					state = SEND_HEADER;
				end

				case1(SEND_HEADER)
				begin
					daqw = W_HEADER;	
					state = SEND_W1;
					crccalc = 1; // start calculating crc
				end

				case1(SEND_W1)
				begin
					daqw = ((Signal)"2'b01", bxn_l1a, "2'b00", l1a_in_countf);
					state = SEND_W2;
				end

				case1(SEND_W2)
				begin
					daqw = ((Signal)"2'b01", readout_count, "2'b00", bxnm); // bxnm is BXN at the start of LCT window
					state = SEND_W3;
				end

				case1(SEND_W3)
				begin
					daqw = ((Signal)"2'b01", bxn_before_reset, "2'b00", "3'b000", lct_bins_report, raw_bins);	
					best_adr++; // start advancing best memory address, so it's 2 clocks ahead of the LCT words
					state = SEND_W4;
				end

				case1(SEND_W4)
				begin
					daqw = 
					(
						(Signal)"1'b0", trig_reg, WP, "2'b00", KE, MR, NP, BF, WGN,
						"2'b00", fwver, SP6, 
						"1'b0", // cp: configuration is not reported in optical format
						seu_error, 
						best_fullf, // lof
						raw_fullf, // rof
						zero_suppress
					);
					best_adr++;
					START_RAW

					If (lct_bins != 0) state = SEND_LCTBINS;
					Else 
					begin
						LOAD_RAW
						state = ifelse(raw_bins != 0, SEND_RAWBINS, AFTER_DATA);
					end
				end

				case1(SEND_LCTBINS)
				begin
					If (best_cnt == lct_bins) 
						daqw = "28'h0"; // padding bin
					Else
						daqw = ((Signal)"2'b00", ALCT12(best2m), shower_m, ALCT12(best1m));

					best_adr++;

					If (best_cnt == lct_bins) // last LCT bin
					begin
						LOAD_RAW
						state = ifelse(raw_bins != 0, SEND_RAWBINS, AFTER_DATA);
					end
					best_cnt++;
				end

				case1(SEND_RAWBINS)
				begin
					ly_zero = zero_suppress && lzt(0); // entire layer is 0

					If (ly_zero) 
						daqw = W_ZSF1; // for empty layer send layer compression marker
					Else
						daqw = ((Signal)"2'b00", lyt[0](23,12), "2'b00", lyt[0](11,0)); // send layer portion

					If (ly_zero || wg_cnt == LAYER_PARTS-1) // layer finished or was all zeros
					begin
						wg_cnt = 0;
						If (ly_cnt == 5) // time bin finished
						begin
							ly_cnt = 0;
							raw_cnt++;
							LOAD_RAW
							If (raw_cnt == raw_bins) state = AFTER_DATA; // dump finished
						end
						Else
						begin
							// move to next layer
							ly_cnt++;
							for (i = 0; i < 5; i++) lyt[i] = lyt[i+1];
							lzt = ((Signal)"1'b0", lzt(5,1));
						end
					end
					Else
					begin
						// move to next part of the same layer
						wg_cnt++;
						lyt[0] = (Signal(24, 0), lyt[0](LYTW-1, 24));
					end
				end

				case1(SEND_RAWPAD)
				begin
  				    daqw = W_PAD; // send padding
				    // if block length will be multiple of 4 go to evener
				    If (frame_count(1,0) == 1) state = SEND_EVENER;
				end

				case1(SEND_EVENER) 
				begin
        	        state = SEND_CRC;
        		    daqw = W_EVENER;
				    crccalc = 4; // evener is not included into CRC
				end
        	
				case1(SEND_CRC) 
				begin
         		    state = SEND_FRAME_CNT;
        		    daqw = "28'h0";
				    crccalc = 2; // send CRC word
				end

				case1(SEND_FRAME_CNT)
				begin
					state = IDLE;
					daqw = ((Signal)"3'b010", "14'h0", frame_count); // frame_count includes this word
					readout_count++; // count only actually completed readouts
					crccalc = 0; // stop calculating or sending crc
					idle_cnt = 0;
	        	end
        	
        	    Default state = IDLE;

			endcase

			frame_count++;
		end
	end

endmodule
}
