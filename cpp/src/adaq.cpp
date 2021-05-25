#include "adaq.h"

// how many parts of hot channel mask to send (12 bits each)
#define HCMASK_PARTS   (HCMASKBITS/12)   
// how many parts of collision mask to send (14 bits each)
#define COLLMASK_PARTS (COLLMASKBITS/14)
// how many parts of layer to send (12 bits or less in each)
#define LAYER_PARTS (LYWG/12 + (LYWG%12)/4)

#define atvalid(a) (a(1,0) != 0)

void adaq::operator()
(
	Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,   
    Signal best1,              			
	Signal best2,              			
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
    Input_(bxn, 11,  0);		// bx number
    Input_(fifo_tbins, 4,  0);	// length of the dump for one L1A
    Output_(daqp, 18, 0);		// output to daq (including all service bits) 
    Input_(l1a_delay, 7,  0); 	// time back to get the raw hits corresponding to this to L1A
    Input_(fifo_pretrig, 4,  0); // delay of the input raw hit data to compensate for the processing and pretriggering
    Input_(fifo_mode, 1,  0);	// bits from configuration register defining the raw hits dump mode
    Input_(L1A, BWTMARK, 0); // one extra bit is valid flag (MSB) 
    Input (hard_rst);
	Input (l1a_internal);	// if ==1 then L1A will be generated internally for each valid track found
    Input_(l1a_window,3, 0);  // window in which to look for valid tracks in case of L1A
    Input_(l1a_offset,3, 0);  // initial value for l1a counter

	Output(L1AWindow); // test points
	Output(l1aTP);
	Output(validhd);

	Input (send_empty); // set this to 1 to report DAQ for empty events
	Input (config_report);
	Input_(bxn_before_reset, MXBXN-1, 0);
	Input_(virtex_id, IDSIZE-1, 0);
	Input_(trig_reg, 2, 0);
	Input_(config_reg, CRSIZE-1, 0);
	Input_(hot_channel_mask, HCMASKBITS-1, 0);
	Input_(collision_mask, COLLMASKBITS-1, 0);
    Input (clk);

beginmodule
    Reg_(best1e, BESTBITS-1,  0);	// early best track parameters
	Reg_(best2e, BESTBITS-1,  0);
    Reg_(bxne, 11,  0);			// early bx number

    Wire_(best1ee, BESTBITS-1,  0);	// early best track parameters
	Wire_(best2ee, BESTBITS-1,  0);
    Wire_(bxnee, 11,  0);			// early bx number

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
	Reg_(l1a_count, 11, 0); // event counter
	Reg_(readout_count, 11, 0); // shows how many readouts have been actually shipped
	Reg_(state, 4, 0); // state machine state

	Wire_(l1a_int_delay, 7, 0);
    Wire_(l1a_int_offset,3, 0);

	Reg_(best_cnt, 3, 0);
	Reg (have_lcts);
	Reg_(bxn_l1a, 11, 0); // bxn corresponding to l1a time - l1a_delay

	Reg_(lct_bins, 3, 0); // how many lct and raw bins actually sent
	Reg_(raw_bins, 4, 0);

	Reg_(cmpart_cnt, 5, 0); // coll mask part counter
	Reg_(collision_reg, COLLMASKBITS-1, 0);

	Reg_(bestm, BESTBITS*2-1, 0);
	Reg (best_first);
	Reg__(lyt, LYWG-1, 0, 5, 0); // one complete time bin for tx
	Reg_(raw_cnt, 5, 0); // raw time bins counter
	Reg_(ly_cnt, 2, 0); // layer counter for tx
	Reg_(wg_cnt, 3, 0); // wiregroup groups-of-12 counter for layer tx
	Reg_(crccalc, 2, 0);// tells when to calculate CRC
    Reg_(daqw, 18, 0);		// output to daq before crc (including all service bits) 
	Wire(davv); // data valid flag (required by DMB or DDU)
	Reg_(frame_count, 9, 0);
	Reg (l1a_procr);
	Wire_(self_l1a, BWTMARK, 0);
	Wire_(tbxn, BWTMARK, 0);
	Wire (best1v);
	Wire (best1eev);
	Reg (trig_stop);
	int i;
	
	assign l1a_int_delay = l1a_delay - 2;
	assign l1a_int_offset = l1a_offset - 1;
	assign L1AWindow = best_we;
	assign l1aTP = l1a_proc;
	assign validhd = atvalid(best1d);



	// best_delay module delays best track data + bxn for l1a_delay clocks
	Module (best_delay);  
 	best_delay.bwd = BESTBITS*2 + 12; 
	best_delay.bwad = 8;
	best_delay.ram_style = 1; // block
	best_delay
	(
		(best1,  best2,  bxn),
		(best1ee, best2ee, bxnee),
		l1a_int_delay, // delay best tracks two clocks less because l1a_maker wants to know early to generate internal l1a
		hard_rst,
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
	l1a_fifo.bwd = 18; // to accomodate both 8-bit addresses and both full flags
	l1a_fifo.bwad = 8; 
	l1a_fifo
	(
		(best_full,  raw_full,  best_adw,  raw_adw ),
		(best_fullf, raw_fullf, best_adwf, raw_adwf),
		l1a_procr,
		l1a_fifo_re,
		!hard_rst,
		l1a_fifo_empty,
		l1a_fifo_full,
		clk
	);

	assign best1v   = atvalid(best1);
	assign best1eev = atvalid(best1ee);
    assign self_l1a = (best1v, bxn);
    assign tbxn     = (best1eev, bxnee);

	// detects rising edge on L1A, makes l1a_proc, best_we and raw_we signals
	Module (al1a_maker_);
	al1a_maker_
	(
		ifelse(l1a_internal, self_l1a, L1A), // if internal l1a is enabled, generate it as soon as the track is found
		tbxn,
		l1a_proc,
		l1a_window,
		fifo_tbins,
		best_full,
		raw_full,
		fifo_mode != 0,
		best_we,
		raw_we,
//		l1a_internal,
		"1'b0",
		hard_rst,
		clk
	);

	// best tracks to be reported in DAQ sequence are stored in this memory
	Module (best_memory);
 	best_memory.bwd = BESTBITS*2 + 12; 
	best_memory.bwad = 8;
	best_memory
	(
		best_adw,
		best_adr,
		best_adb,
		(best1d, best2d, bxnd),
		(best1m, best2m, bxnm),
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

	// dav bit not needed
	assign davv = "1'b0";

	enum  // machine states
	{
		IDLE 			,
		WAIT_L1A 		,
		READ_L1A 		,
		WAIT_BEST_RAW	,
		CHECK_LCTS 		,
		SEND_HFEATURES 	,
		SEND_FWVER 		,
		SEND_HEADER 	,
		SEND_BXN_BEFORE ,
		SEND_BXN_L1A 	,
		SEND_L1A_COUNT	,
		SEND_RO_COUNT	,
		SEND_BIN_NUM	,
		SEND_VIRTEXID0	,	
		SEND_VIRTEXID1	,
		SEND_VIRTEXID2	,
		SEND_CONFIG0	,
		SEND_CONFIG1	,
		SEND_CONFIG2	,
		SEND_CONFIG3	,
		SEND_CONFIG4	,
		SEND_COLLISION	,
		SEND_HOT_CHANNEL,
		SEND_LCTBINS	,
		SEND_RAWBINS	,
		SEND_CRC0		,
		SEND_CRC1		,
		SEND_EVENER		,
		SEND_FRAME_CNT
	};

	always (posedge (clk))
	begin
	  	If (!hard_rst)
	  	begin
			l1a_count = l1a_offset;
			l1a_count--;
		  	readout_count = l1a_offset;
          	state = IDLE;
			best_adw = 1;
			raw_adw = 1;
			best_adb = 0;
			raw_adb = 0;
			crccalc = 0;
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
					l1a_count++;
					state = READ_L1A;
				end

				case1(READ_L1A) // pick up info from l1a fifo
				begin
					best_adr = best_adwf; // addresses from l1a fifo go into read addresses
					best_adb = best_adwf; // blocking address - cannot write past that
					raw_adr  = raw_adwf;
					raw_adb  = raw_adwf;
					best_cnt = 0; // reset lct counter so we can count in check_lcts state
					have_lcts = 0;
					state = WAIT_BEST_RAW;
				end

				case1(WAIT_BEST_RAW) // wait for data from best and raw memory 
				begin
					best_adr++;
					state = CHECK_LCTS;
				end

				case1(CHECK_LCTS) // before sending anything, check if we have any lcts
				begin

					have_lcts = have_lcts | atvalid(best1m);
					best_adr++;
					best_cnt++;

					If (best_cnt == 1) // take l1a bxn from first best memory location
					begin
						bxn_l1a = bxnm;
					end

					If (best_cnt == l1a_window || best_fullf) // all lcts scanned, or if best fifo was full then no need to scan
					begin
						state = ifelse(send_empty || best_fullf || raw_fullf || have_lcts, SEND_HFEATURES, IDLE);
						best_adr = best_adwf; // reset best memory address back to start of this l1a
					end
					frame_count = 0;
				end

				case1(SEND_HFEATURES)
				begin
					daqw = ((Signal)"7'hd0", PB, RL, KE, MR, NP, BF, WGN);	
					state = SEND_FWVER;
					crccalc = 1; // start calculating crc
				end

				case1(SEND_FWVER)
				begin
					daqw = ((Signal)"4'hd", "6'h0", FWVER);	
					state = SEND_HEADER;
				end

				case1(SEND_HEADER)
				begin
					daqw = "16'hdb0a";	
					state = SEND_BXN_BEFORE;
				end

				case1(SEND_BXN_BEFORE)
				begin
					daqw = ((Signal)"4'hd", bxn_before_reset);	
					state = SEND_BXN_L1A;
				end

				case1(SEND_BXN_L1A)
				begin
					daqw = ((Signal)"1'b0", config_report, best_fullf, raw_fullf, bxn_l1a);	
					state = SEND_L1A_COUNT;
				end

				case1(SEND_L1A_COUNT)
				begin
					daqw = ((Signal)"4'b0", l1a_count);	
					state = SEND_RO_COUNT;
				end

				case1(SEND_RO_COUNT)
				begin
					daqw = ((Signal)"4'b0", readout_count);	
					state = SEND_BIN_NUM;
				end

				case1(SEND_BIN_NUM)
				begin
					lct_bins = ifelse(!best_fullf && (send_empty || have_lcts), l1a_window, 0);
					lct_bins(0) = 0; // only even numbers of lct bins allowed
					raw_bins = ifelse(!raw_fullf && fifo_mode != 0,             fifo_tbins, 0);

					daqw = ((Signal)"7'b0", lct_bins, raw_bins);	

					state = ifelse (config_report, SEND_VIRTEXID0, 
							ifelse (lct_bins != 0, SEND_LCTBINS,
							ifelse (raw_bins != 0, SEND_RAWBINS, SEND_CRC0)));

					If (state == SEND_LCTBINS)
					begin
						// in case we're jumping to LCTBINS, prepare all
						bestm = (best2m, best1m);
						best_adr++; // increase best mem address ahead of time (takes two clock to read mem)
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

				case1(SEND_VIRTEXID0)
				begin
					daqw = (virtex_id(14, 0));	
					state = SEND_VIRTEXID1;
				end

				case1(SEND_VIRTEXID1)
				begin
					daqw = (virtex_id(29, 15));	
					state = SEND_VIRTEXID2;
				end

				case1(SEND_VIRTEXID2)
				begin
					daqw = ((Signal)"2'h0", trig_reg, virtex_id(39, 30));	
					state = SEND_CONFIG0;
				end

				case1(SEND_CONFIG0)
				begin
					daqw = (config_reg(14,0));	
					state = SEND_CONFIG1;
				end

				case1(SEND_CONFIG1)
				begin
					daqw = (config_reg(29,15));	
					state = SEND_CONFIG2;
				end

				case1(SEND_CONFIG2)
				begin
					daqw = (config_reg(44,30));	
					state = SEND_CONFIG3;
				end

				case1(SEND_CONFIG3)
				begin
					daqw = (config_reg(59,45));	
					state = SEND_CONFIG4;
				end

				case1(SEND_CONFIG4)
				begin
					daqw = ((Signal)"6'h0", config_reg(68,60));	
					cmpart_cnt = 0;
					collision_reg = collision_mask;
					state = SEND_COLLISION;
				end
				
				case1(SEND_COLLISION)
				begin
					daqw = ((Signal)"1'h0", collision_reg(13,0));	
					collision_reg = ((Signal)"14'h0", collision_reg(COLLMASKBITS-1, 14));
					cmpart_cnt++;
					If (cmpart_cnt == COLLMASK_PARTS) 
					begin
						state = SEND_HOT_CHANNEL;
						for (i = 0; i < 6; i++) lyt[i] = hot_channel_mask((i+1)*LYWG-1, i*LYWG);
						ly_cnt = 0;
						wg_cnt = 0;
					end
				end
				
				case1(SEND_HOT_CHANNEL)
				begin
					daqw = ((Signal)"4'b0", lyt[ly_cnt](11, 0));
					lyt[ly_cnt] = lyt[ly_cnt](LYWG-1, 12); // shift to next portion
					wg_cnt++;

					If (wg_cnt == LAYER_PARTS) // layer finished
					begin
						wg_cnt = 0;
						ly_cnt++;
						If (ly_cnt == 6) // time bin finished
						begin
							state = ifelse (lct_bins != 0, SEND_LCTBINS,
									ifelse (raw_bins != 0, SEND_RAWBINS, SEND_CRC0));

							bestm = (best2m, best1m);
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
				end
				
				case1(SEND_LCTBINS)
				begin
					daqw = ((Signal)"4'b0", bestm(BESTBITS-1, 0), atvalid(bestm));
					bestm = bestm(BESTBITS*2-1, BESTBITS); // shift to second best

					If (!best_first) 
					begin
						best_adr++;
						bestm = (best2m, best1m);
						best_cnt++;
					end
					best_first = !best_first;
	
					If (best_cnt == lct_bins) 
					begin
						state = ifelse (raw_bins != 0, SEND_RAWBINS, SEND_CRC0);
						
						for (i = 0; i < 6; i++) lyt[i] = lym[i];
						raw_adr++;
						raw_cnt = 0;
						ly_cnt = 0;
						wg_cnt = 0;
					end
				end
				
				case1(SEND_RAWBINS)
				begin
					daqw = ((Signal)"4'b0", lyt[ly_cnt](11, 0));
					lyt[ly_cnt] = lyt[ly_cnt](LYWG-1, 12); // shift to next portion
					wg_cnt++;

					If (wg_cnt == LAYER_PARTS) // layer finished
					begin
						wg_cnt = 0;
						ly_cnt++;
						If (ly_cnt == 6) // time bin finished
						begin
							ly_cnt = 0;
							raw_adr++;
							raw_cnt++;
							for (i = 0; i < 6; i++) lyt[i] = lym[i];
							If (raw_cnt == raw_bins) // dump finished
							begin
								state = SEND_CRC0;
							end
						end
					end
	
				end

        	case1(SEND_CRC0) 
        	begin
        		state = SEND_CRC1;
        		daqw = 0;
				crccalc = 2; // send lsb
        	end

        	case1(SEND_CRC1) 
        	begin
        		state = SEND_EVENER;
        		daqw = 0;
				crccalc = 3; // send msb
        	end

        	case1(SEND_EVENER) 
        	begin
        	    state = SEND_FRAME_CNT;
        		daqw = "19'hde0d";
				crccalc = 0; // stop calculating or sending crc
        	end
        	
        	case1(SEND_FRAME_CNT)
        	begin
        		state = IDLE;
        		daqw = ((Signal)"9'b001110100", frame_count);
				readout_count++; // count only actually completed readouts
        	end
        	
        	Default state = IDLE;

			endcase
			frame_count++;
			l1a_procr = l1a_proc;
			(best1d, best2d, bxnd) = (best1e,  best2e,  bxne); // add two clock's delay to best tracks to align them with raw hits
			(best1e, best2e, bxne) = (best1ee, best2ee, bxnee); 

		end
	end

endmodule
}
