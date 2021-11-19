#include "daq.h"

// how many parts of hot channel mask to send (12 bits each)
#define HCMASK_PARTS   (HCMASKBITS/12)   
// how many parts of collision mask to send (14 bits each)
#define COLLMASK_PARTS (COLLMASKBITS/14)
// how many parts of layer to send (12 bits or less in each)
#define LAYER_PARTS (LYWG/12 + ((LYWG%12 == 0) ? 0 : 1))

// how many 12-bit words per entire chamber
#define CHAMBER_WORDS (LAYER_PARTS * 6) 

// wide pattern version flag for DAQ
#ifdef TRIGWP
	#define WP "1'b1"
#else
	#define WP "1'b0"
#endif


void daq::operator()
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
	Signal clk
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
    Output_(daqp, 18, 0);		// output to daq (including all service bits) 
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
	Input (config_report_i);
	Input_(bxn_before_reset, MXBXN-1, 0);
	Input_(virtex_id, IDSIZE-1, 0);
	Input_(trig_reg, 2, 0);
	Input_(config_reg, CRSIZE-1, 0);
	Input_(hot_channel_mask, HCMASKBITS-1, 0);
	Input_(collision_mask, COLLMASKBITS-1, 0);
	Input (zero_suppress);
	Input (trig_stop);
	Input (seu_error);
    Input (clk);

beginmodule
    Wire_(best1e, BESTBITS-1,  0);	// early best track parameters
	Wire_(best2e, BESTBITS-1,  0);
    Wire_(bxne, 11,  0);			// early bx number

    Reg_(best1d, BESTBITS-1,  0);	// best track parameters delayed
	Reg_(best2d, BESTBITS-1,  0);
    Reg_(bxnd, 11,  0);			// bx number delayed

    Wire_(best1m, BESTBITS-1,  0);	// best track parameters read from daq memory
	Wire_(best2m, BESTBITS-1,  0);
    Wire_(bxnm, 11,  0);			// bx number read from daq memory

	Wire__(lyd, LYWG-1, 0, 5, 0); // raw hits delayed
	Wire__(lym, LYWG-1, 0, 5, 0); // raw hits read from daq memory

	Reg (l1a_fifo_re); 
	Wire(l1a_fifo_empty);
	Wire(l1a_fifo_full);

	Wire (l1a_proc);
	Wire (best_we);
	Wire (raw_we);

	Reg_(best_adw,  7, 0);
	Wire_(best_adwf, 7, 0); 
	Reg_(best_adr,  7, 0);
	Reg_(best_adb,  7, 0);
	Reg_(raw_adw,  7, 0);
	Wire_(raw_adwf,  7, 0);
	Reg_(raw_adr,  7, 0);
	Reg_(raw_adb,  7, 0);

	Wire (best_full);
	Wire (raw_full);
	Wire (best_fullf);
	Wire (raw_fullf);
	Wire_(l1a_in_count, 11, 0); // l1a counter
	Wire_(l1a_in_countf, 11, 0); 
	Reg_(readout_count, 11, 0); // shows how many readouts have been actually shipped
	Reg_(state, 4, 0); // state machine state

	Wire_(l1a_int_delay, 7, 0);
    Wire_(l1a_int_offset,3, 0);

	Reg_(best_cnt, 3, 0);
	Reg (have_lcts);
	Reg_(bxn_track, 11, 0); // bxn corresponding to l1a time - l1a_delay

	Reg_(lct_bins, 3, 0); // how many lct and raw bins actually sent
	Reg_(lct_bins_report, 3, 0); // lct bins number to report in daq (includes padding)
	Reg_(raw_bins, 4, 0);

	Reg_(cmpart_cnt, 5, 0); // coll mask part counter

	Reg_(best1t, BESTBITS-1, 0);
	Reg_(best2t, BESTBITS-1, 0);
	Reg (best_first);
	Reg__(lyt, LYWG-1, 0, 5, 0); // one complete time bin for tx
	Reg_(raw_cnt, 5, 0); // raw time bins counter
	Reg_(ly_cnt, 2, 0); // layer counter for tx
	Reg_(wg_cnt, 3, 0); // wiregroup groups-of-12 counter for layer tx
	Reg_(crccalc, 2, 0);// tells when to calculate CRC
    Reg_(daqw, 18, 0);		// output to daq before crc (including all service bits) 
	Wire(davv); // data valid flag (required by DMB or DDU)
	Reg_(frame_count, 10, 0);
	Reg (l1a_procr);

	Reg__(config_word, 14, 0, 7, 0); // virtex id and config words to report
	Reg_ (conf_w_count, 3, 0); // config word counter

	Reg__(collmask_word, 13, 0, COLLMASK_PARTS-1, 0); // collision mask parts
	Reg__(hot_word, 12, 0, CHAMBER_WORDS-1, 0); // hot channel parts
	Reg_ (hot_w_count, 5, 0); // hot channel part counter
	Wire_ (bxn_l1a, 11, 0); // bxn at the l1a arrival time
	Wire (l1a_bxn_fifo_empty);// unused outputs
	Wire (l1a_bxn_fifo_full);
	Reg_(bxnr, 11, 0); // delayed bxn
	Reg (ly_zero);
    Reg (tb_zero);
	Wire (valor);
	Reg (valorr);
	Reg (config_report);
	Wire_(shower_e, 1, 0);
	Reg_ (shower_d, 1, 0);
	Wire_(shower_m, 1, 0);
	Reg_ (shower_t, 1, 0);

	int i;
	
	assign l1a_int_delay = l1a_delay - 1;
	assign l1a_int_offset = l1a_offset - 1;
	assign L1AWindow = best_we;
	assign l1aTP = l1a_proc;
	assign validhd = tvalid(best1d); // output to test point

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
	 (tvalid(best1) | (shower_int > 1)), // valid signal, either track or nominal or tight shower
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

	// L1A fifo stores best track and raw hit memory addresses for every L1A
	Module (l1a_fifo);
	l1a_fifo.bwd = 30; // to accomodate both 8-bit addresses and both full flags, and l1a counter
	l1a_fifo.bwad = 8; 
	l1a_fifo
	(
		(l1a_in_count,  best_full,  raw_full,  best_adw,  raw_adw ),
		(l1a_in_countf, best_fullf, raw_fullf, best_adwf, raw_adwf),
		l1a_procr,
		l1a_fifo_re,
		!hard_rst,
		l1a_fifo_empty,
		l1a_fifo_full,
		clk
	);

	// L1A bxn fifo stores bxn of L1A arrival 
	Module (l1a_bxn_fifo);
	l1a_bxn_fifo.bwd = 12;
	l1a_bxn_fifo.bwad = 8; 
//#if defined ALCT672 // there are not enough RAM blocks for this fifo in alct672
	// using distributed always trying to fix the bnx at the time of L1a bug
	l1a_bxn_fifo.distributed = 1;
//#endif
	l1a_bxn_fifo
	(
		bxnr,
		bxn_l1a,
		l1a_procr,
		l1a_fifo_re,
		!hard_rst,
		l1a_bxn_fifo_empty,
		l1a_bxn_fifo_full,
		clk
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
		clk
	);

	// raw hits to be reported in DAQ sequence are stored in this memory
	Module (raw_memory);
 	raw_memory.bwd = HCMASKBITS; 
	raw_memory.bwad = 8;
	raw_memory
	(
		raw_adw,
		raw_adr,
		raw_adb,
		(lyd[0], lyd[1], lyd[2], lyd[3], lyd[4], lyd[5]),
		(lym[0], lym[1], lym[2], lym[3], lym[4], lym[5]),
		raw_we,
		((Signal)"3'b0", fifo_tbins),
		raw_full,
		clk
	);

	// CRC generator
	Module (crcgen);
	crcgen
	(
		daqw, 
		daqp, 
		crccalc, 
		davv,
		clk
	);

	// dav bit generator
	Module (davgen);
	davgen
	(	
		l1a_proc,
		// set dav if quality of the best track is not 0 or send_empty is set or one of the memories is full
		davv,   
		clk
	);

	enum  // machine states
	{
		IDLE 			,
		WAIT_L1A 		,
		READ_L1A 		,
		WAIT_BEST_RAW	,
		CHECK_LCTS 		,

		SEND_HEADER 	,
		SEND_BXN_L1A 	,
		SEND_L1A_COUNT	,
		SEND_RO_COUNT	,
		SEND_BXN		,
		SEND_BXN_BEFORE ,
		SEND_HFEATURES 	,
		SEND_BIN_NUM	, // fw version is here

		SEND_CONFIG,

		SEND_COLLISION	,
		SEND_HOT_CHANNEL,
		SEND_LCTBINS	,
		SEND_LCTPAD1    ,
		SEND_LCTPAD2    ,
		SEND_RAWBINS	,
		SEND_RAWPAD     ,
		SEND_EVENER		,
		SEND_CRC0		,
		SEND_CRC1		,
		SEND_FRAME_CNT
	};

	always (posedge (clk))
	begin
/*
		config_word[0] = virtex_id(14, 0);
		config_word[1] = virtex_id(29, 15);
		config_word[2] = ((Signal)"2'h0", trig_reg, virtex_id(39, 30));
		config_word[3] = config_reg(14,0);
		config_word[4] = config_reg(29,15);
		config_word[5] = config_reg(44,30);
		config_word[6] = config_reg(59,45);
		config_word[7] = ((Signal)"6'h0", config_reg(68,60));

		for (i = 0; i < COLLMASK_PARTS; i++)
		{
			collmask_word[i] = collision_mask((i+1)*14-1, i*14);
		}
		
		for (i = 0; i < 6; i++)
		{
			for (j = 0; j < LAYER_PARTS-1; j++)
			{
				hot_word[i * LAYER_PARTS + j] = hot_channel_mask(i*LYWG + (j+1)*12 - 1, i*LYWG + j*12);
			}
			hot_word[(i+1) * LAYER_PARTS - 1] = hot_channel_mask((i+1)*LYWG - 1, i*LYWG + j*12);
		}
*/
	  	If (!hard_rst)
	  	begin
		  	readout_count = l1a_offset;
          	state = IDLE;
			best_adw = 1;
			raw_adw = 1;
			best_adb = 0;
			raw_adb = 0;
			crccalc = 0;
			config_report = 0;
			ly_zero = 0;
	  	end
		Else
		begin
			If (best_we) best_adw++;
			If (raw_we)  raw_adw++;

			// daq state machine
			daqw = "19'b100_0000_0000_0000_0000";
			l1a_fifo_re = 0;
			begincase (state)
				case1(IDLE)
				begin
					state = ifelse (l1a_fifo_empty, IDLE, WAIT_L1A);
					l1a_fifo_re = !l1a_fifo_empty;
					crccalc = 0; // don't calculate or output any crc
				end

				case1(WAIT_L1A) // let l1a fifo spit out info
				begin
					state = READ_L1A;
				end

				case1(READ_L1A) // pick up info from l1a fifo
				begin
					raw_adr  = raw_adwf;
					raw_adb  = raw_adwf;
					best_cnt = 0; // reset lct counter so we can count in check_lcts state
					state = WAIT_BEST_RAW;
				end

				case1(WAIT_BEST_RAW) // wait for data from best and raw memory 
				begin
					state = CHECK_LCTS;
				end

				case1(CHECK_LCTS) // lct check removed here since L1A was checked for LCTs before being stored
				begin

					have_lcts = 1;
					best_cnt++;
					state = SEND_HEADER;
					best_adr = best_adwf; // reset best memory address back to start of this l1a
                    best_adb = best_adwf;
					frame_count = 0;
				end

				case1(SEND_HEADER)
				begin
					daqw = "19'hdb0a";	
					state = SEND_BXN_L1A;
					crccalc = 1; // start calculating crc
				end

				case1(SEND_BXN_L1A)
				begin
					daqw = ((Signal)"7'hd", bxn_l1a); // BXN at the time of L1A
					state = SEND_L1A_COUNT;
				end

				case1(SEND_L1A_COUNT)
				begin
					daqw = ((Signal)"7'hd", l1a_in_countf);	
					bxn_track = bxnm;
					state = SEND_RO_COUNT;
				end

				case1(SEND_RO_COUNT)
				begin
					daqw = ((Signal)"7'hd", readout_count);	
					state = SEND_BXN;
				end

				case1(SEND_BXN)
				begin
					daqw = ((Signal)"4'b0", config_report, (Signal)"1'b0", (Signal)"1'b0", bxn_track);	
					state = SEND_BXN_BEFORE;
				end

				case1(SEND_BXN_BEFORE)
				begin
				    daqw = ((Signal)"6'h0", zero_suppress, bxn_before_reset);	
					state = SEND_HFEATURES;
				end

				case1(SEND_HFEATURES)
				begin
					daqw = ((Signal)"7'h0", seu_error, SP6, WP, PB, RL, KE, MR, NP, BF, WGN);	
					state = SEND_BIN_NUM;
				end

				case1(SEND_BIN_NUM)
				begin
					lct_bins = ifelse(send_empty || have_lcts, l1a_window, 0);
					raw_bins = ifelse(fifo_mode != 0,             fifo_tbins, 0);
					// lct_bins+1 reported to account for padding
					lct_bins_report = ifelse(lct_bins != 0, lct_bins + "4'b1", "4'b0");

					daqw = ((Signal)"4'b0", FWVER, lct_bins_report, raw_bins);

					state = ifelse (config_report, SEND_CONFIG, 
							ifelse (lct_bins != 0, SEND_LCTBINS,
							ifelse (raw_bins != 0, SEND_RAWBINS, SEND_EVENER)));

					conf_w_count = 0;

					If (state == SEND_LCTBINS)
					begin
						// in case we're jumping to LCTBINS, prepare all
						best1t = best1m;
						best2t = best2m;
						shower_t = shower_m;
						best_adr++; // increase best mem address ahead of time (takes two clocks to read mem)
						best_cnt = 0;
						best_first = 1;
					end

					If (state == SEND_RAWBINS)
					begin
					// in case we're jumping to RAWHITS, prepare all
						for (i = 0; i < 6; i++) lyt[i] = lym[i];
						raw_adr++;
						raw_cnt = 0;
						ly_cnt = 0;
						wg_cnt = 0;
					end

				end

				case1(SEND_CONFIG)
				begin
					daqw = config_word[conf_w_count];	
					conf_w_count++;
					If(conf_w_count == "4'h8")
					begin
						cmpart_cnt = 0;
						state = SEND_COLLISION;
					end
				end
				
				case1(SEND_COLLISION)
				begin
					daqw = ((Signal)"4'h0", collmask_word[cmpart_cnt]);	
					cmpart_cnt++;
					If (cmpart_cnt == COLLMASK_PARTS) 
					begin
						state = SEND_HOT_CHANNEL;
						hot_w_count = 0;
					end
				end
				
				case1(SEND_HOT_CHANNEL)
				begin
					daqw = ((Signal)"7'b0", hot_word[hot_w_count]);
					hot_w_count++;

					If (hot_w_count == CHAMBER_WORDS) // transmission finished
					begin
						state = ifelse (lct_bins != 0, SEND_LCTBINS,
								ifelse (raw_bins != 0, SEND_RAWBINS, SEND_EVENER));

						best1t = best1m;
						best2t = best2m;
						shower_t = shower_m;
						best_adr++; // increase best mem address ahead of time (takes two clock to read mem)
						best_cnt = 0;
						best_first = 1;
						If (state == SEND_RAWBINS)
						begin
						// in case we're jumping to RAWHITS, prepare all
							for (i = 0; i < 6; i++) lyt[i] = lym[i];
							raw_adr++;
							raw_cnt = 0;
							ly_cnt = 0;
							wg_cnt = 0;
						end
					end
				end
				
				case1(SEND_LCTBINS)
				begin
					If (best_first)
							daqw = ((Signal)"5'b0", shower_t, best1t(10,4), "1'b0", best1t(2,0), tvalid(best1t));
					Else
						  daqw = ((Signal)"5'b0", shower_t, best2t(10,4), "1'b0", best2t(2,0), tvalid(best2t));

					If (!best_first) 
					begin
						best_adr++;
						best1t = best1m;
						best2t = best2m;
						shower_t = shower_m;
						best_cnt++;
					end
					best_first = !best_first;
	
					If (best_cnt == lct_bins) 
					begin
						state = SEND_LCTPAD1;
						
						for (i = 0; i < 6; i++) lyt[i] = lym[i];
						raw_adr++;
						raw_cnt = 0;
						ly_cnt = 0;
						wg_cnt = 0;
					end
				end
				
				case1(SEND_LCTPAD1)
				begin
					daqw = "19'd0";
					state = SEND_LCTPAD2;
				end

				case1(SEND_LCTPAD2)
				begin
					daqw = "19'd0";
					state = ifelse (raw_bins != 0, SEND_RAWBINS, SEND_EVENER);
				end

				case1(SEND_RAWBINS)
				begin

					ly_zero = zero_suppress && (lyt[ly_cnt] == 0); // entire layer is 0

				// entire time bin suppression is not working so far because of raw_memory latency 
//				    tb_zero = zero_suppress && lyt[0] == 0 && lyt[1] == 0 && lyt[2] == 0 && lyt[3] == 0 && lyt[4] == 0 && lyt[5] == 0; // entire time bin is 0
				    If (!ly_zero) // if layer is not 0, transmit parts
					begin
					    for (i = 0; i < LAYER_PARTS-1; i++)
						{
							If (wg_cnt == i) 
							    daqw = ((Signal)"7'b0", lyt[ly_cnt]((i+1)*12-1, i*12)); // send layer portion
						}
					end

					If (wg_cnt == LAYER_PARTS-1 || ly_zero) // layer finished or was all zeros
					begin
					    If (!ly_zero)
						    daqw = ((Signal)"7'b0", lyt[ly_cnt](LYWG-1, (LAYER_PARTS-1)*12)); // send the last layer portion
					    Else
						    daqw = "19'h1000"; // for empty layer send layer compression marker
						wg_cnt = 0;
						ly_cnt++;
						If (ly_cnt == 6 || tb_zero) // time bin finished
						begin
						    If (tb_zero) daqw = "19'h2000"; // for empty time bin send bin compression marker
							ly_cnt = 0;
							raw_adr++;
							raw_cnt++;
							for (i = 0; i < 6; i++) lyt[i] = lym[i];
							If (raw_cnt == raw_bins) // dump finished
							begin
							    // if number of frames multiple of 4 already go to evener, else send padding
							    If (frame_count(1,0) == 0) state = SEND_EVENER;
							    Else state = SEND_RAWPAD; 
							end
						end
					end
					Else
						wg_cnt++;
	
				end

				case1(SEND_RAWPAD)
				begin
  				    daqw = "19'h3000"; // send padding
				    // if reached multiple of 4 go to evener
				    If (frame_count(1,0) == 0) state = SEND_EVENER;
				end

				case1(SEND_EVENER) 
				begin
        	        state = SEND_CRC0;
        		    daqw = "19'hde0d";
				    crccalc = 4; // evener is not included into CRC
				end
        	
				case1(SEND_CRC0) 
				begin
         		    state = SEND_CRC1;
        		    daqw = "19'h0";
				    crccalc = 2; // send lsb
				end

				case1(SEND_CRC1) 
				begin
					state = SEND_FRAME_CNT;
					daqw = "19'h0";
					crccalc = 3; // send msb
				end
	
				case1(SEND_FRAME_CNT)
				begin
					state = IDLE;
					daqw = ((Signal)"8'b00111010", frame_count);
					readout_count++; // count only actually completed readouts
					crccalc = 0; // stop calculating or sending crc
	        	end
        	
        	    Default state = IDLE;

			endcase

			frame_count++;
			l1a_procr = l1a_proc;
			valorr = valor;
			// add one clock's delay to best tracks and shower to align them with raw hits
			(shower_d, best1d, best2d, bxnd) = (shower_e, best1e, best2e, bxne); 
			bxnr = bxn; // delay bxn to complensate for L1A transition detection time
		end
	end

endmodule
}
