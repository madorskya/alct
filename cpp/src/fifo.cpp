#include "fifo.h"

void fifo::operator()
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
    Signal BoardID, 
	Signal CSCID, 
    Signal L1A,
	Signal hard_rst,
    Signal l1a_internal, Signal l1a_window, Signal l1a_offset,
	Signal WaitForSpaceInOutFIFO,
	Signal L1AWindow, Signal l1aTP, Signal validhd,
	Signal send_empty,
	Signal bxn_before_reset,
	Signal trig, Signal acc_trig,
	Signal clk
)
{
initio

	comment("// layer data")
	ly0.input(LYWG-1, 0, "ly0");
	ly1.input(LYWG-1, 0, "ly1");
	ly2.input(LYWG-1, 0, "ly2");
	ly3.input(LYWG-1, 0, "ly3");
	ly4.input(LYWG-1, 0, "ly4");
	ly5.input(LYWG-1, 0, "ly5");

	
    best1.input (BESTBITS-1, 0, "best1");	comment("// best track parameters ")
	best2.input (BESTBITS-1, 0, "best2");	comment("// second best track parameters {key, patb, amu, quality}")
    bxn.input (11, 0, "bxn");				comment("// bx number")
    fifo_tbins.input (4, 0, "fifo_tbins");	comment("// length of the dump for one L1A")
    Output_(daqp, 18, 0);					comment("// output to daq (including all service bits) ")
    l1a_delay.input (7, 0, "l1a_delay"); 	comment("// time back to get the raw hits corresponding to this to L1A")
    fifo_pretrig.input (4, 0, "fifo_pretrig"); comment("// delay of the input raw hit data to compensate for the processing and pretriggering")
    fifo_mode.input (1, 0, "fifo_mode");	comment("// bits from configuration register defining the raw hits dump mode")
    BoardID.input (2, 0, "BoardID");
    CSCID.input (3, 0, "CSCID");
    L1A.input ("L1A"); 
    hard_rst.input ("hard_rst");
	l1a_internal.input ("l1a_internal");	comment("// if ==1 then L1A will be generated internally for each valid track found")
    l1a_window.input (3, 0, "l1a_window");  comment("// window in which to look for valid tracks in case of L1A")
    l1a_offset.input (3, 0, "l1a_offset");  comment("// initial value for l1a counter")
	WaitForSpaceInOutFIFO.input ("WaitForSpaceInOutFIFO"); comment("// this is used in debugging mode, when there is not enough space in the Output FIFO")
	L1AWindow.output ("L1AWindow", makereg);
	l1aTP.output ("l1aTP", makereg);
	Output (validhd);
	Input (send_empty); comment("// set this to`1 to report DAQ for empty events")
	Input_(bxn_before_reset, MXBXN-1, 0);
	Input_(trig, 2, 0);
	Input_(acc_trig, 2, 0);
    clk.input ("clk");

beginmodule

	comment("// L1A processing state machine state definitions")
	parameter L1Aidle           ("L1Aidle"          , 0);
	parameter L1AreadBest0      ("L1AreadBest0"     , 1);
	parameter L1AreadBest1      ("L1AreadBest1"     , 2);
	parameter L1AreadBest2      ("L1AreadBest2"     , 3);
	parameter L1AsendHeader0    ("L1AsendHeader0"   , 4);
	parameter L1AsendHeader1    ("L1AsendHeader1"   , 5);
	parameter L1AsendHeader2    ("L1AsendHeader2"   , 6);
	parameter L1AsendHeader3    ("L1AsendHeader3"   , 7);
	parameter L1AsendLCT0_0     ("L1AsendLCT0_0"    , 8);
	parameter L1AsendLCT0_1     ("L1AsendLCT0_1"    , 9);
	parameter L1AsendLCT1_0     ("L1AsendLCT1_0"    , 10);
	parameter L1AsendLCT1_1     ("L1AsendLCT1_1"    , 11);
	parameter L1AsendRawHits    ("L1AsendRawHits"   , 12);
	parameter L1AsendEvener     ("L1AsendEvener"    , 13);
	parameter L1AsendFrameCount ("L1AsendFrameCount", 14);
#ifdef DAQCRC
	parameter L1AsendCRC0       ("L1AsendCRC0"      , 15);
	parameter L1AsendCRC1       ("L1AsendCRC1"      , 16);
#endif

	lyr0.wire(LYWG-1, 0, "lyr0"); comment("// delayed inputs")
	lyr1.wire(LYWG-1, 0, "lyr1");
	lyr2.wire(LYWG-1, 0, "lyr2");
	lyr3.wire(LYWG-1, 0, "lyr3");
	lyr4.wire(LYWG-1, 0, "lyr4");
	lyr5.wire(LYWG-1, 0, "lyr5");

	lyt0.wire(LYWG-1, 0, "lyt0"); comment("// output of the Raw hits memory. These registers are also uesd for multiplexing the raw hits during the transmission to DAQ")
	lyt1.wire(LYWG-1, 0, "lyt1");
	lyt2.wire(LYWG-1, 0, "lyt2");
	lyt3.wire(LYWG-1, 0, "lyt3");
	lyt4.wire(LYWG-1, 0, "lyt4");
	lyt5.wire(LYWG-1, 0, "lyt5");

    lytr.reg(LYWG-1, 0, "lytr"); comment("// layer for the transmission")
    
    q1tr.reg(QBITS-1, 0, 2, 0, "q1tr");
    q2tr.reg(QBITS-1, 0, 2, 0, "q2tr"); comment("// temp registers for the quality of the transmitted tracks")
    zz5.reg(4, 0, "zz5"); comment("// dummy")
    z4.reg(3, 0, "z4"); comment("// dummy")
    z5.reg(4, 0, "z5"); comment("// dummy")
    L1Aempty.wire("L1Aempty"); comment("// flag showing that L1A fifo is empty")
    L1Astate.reg(4, 0, "L1Astate"); comment("// state machine state (see definitions at the top of the file)")
    L1Astaten.reg(4, 0, "L1Astaten"); comment("// new state machine state (see definitions at the top of the file)")
    L1Anumber.reg(7, 0, "L1Anumber"); comment("// L1A number counter. Reset by hard_rst")
	bxnr.reg(4, 0, "bxnr"); comment("// 5 lsb of the bxn for the best track")
	bxntr.reg(4, 0, "bxntr");
    bxnlast.wire(4, 0, "bxnlast"); comment("// this wire is always one clock behind the bxn (see comments to BestTrkBuffer)")
    LCTchip.reg(2, 0, "LCTchip"); comment("// chip counter (for compatibility with the old readout format)")
    Tbin.reg(4, 0, "Tbin");    comment("// time bin counter (for compatibility with the old readout format)")
    LyCnt.reg(3, 0, "LyCnt");   comment("// layer counter for transmission")
    bytetr.reg(7, 0, "bytetr");  comment("// byte from the layer to transmit")
    FrameCount.reg(10, 0, "FrameCount"); comment("// frame counter for transmission")
	BxnRdAdd.wire(8, 0, "BxnRdAdd"); comment("// read addr for the memory keeping track of the bx numbers for the raw hits buffer")
	RawWritten.reg("RawWritten"); comment("// shows if raw hits were written for this best track")
    RawWrittenf.reg("RawWrittenf");
    // new
   	best1d.wire(BESTBITS-1, 0, "best1d"); comment("// best tracks delayed for l1a_delay clocks")
	best2d.wire(BESTBITS-1, 0, "best2d");
    bxnd.wire(11, 0, "bxnd"); comment("// bxn delayed for l1a_delay clocks")
    TimeBin.reg(4, 0, "TimeBin"); comment("// Time bin number when writing into Raw hits fifo")
    L1AoneShot.reg("L1AoneShot"); comment("// expanded L1A for raw hit recording")
    zz3.reg(2, 0, "zz3"); comment("// dummy")
	zzz3.reg(2, 0, "zzz3");
	zzzz3.reg(2, 0, "zzzz3");
	zzzzz3.reg(2, 0, "zzzzz3");
    z8.reg(7, 0, "z8");
    z31.wire(30, 0, "z31");
    L1Anumberf.wire(7, 0, "L1Anumberf");     comment("// read from best track fifo")
    L1Anumberp.reg("L1Anumberp");     comment("// previous L1A number's lsb")
    L1Anumberw.reg(7, 0, "L1Anumberw");     comment("// number for readout")
    L1AnumberRaw0.reg(7, 0, "L1AnumberRaw0");  comment("// raw hit L1A number")
	L1AnumberRaw1.reg(7, 0, "L1AnumberRaw1");
	L1AnumberRaw2.reg(7, 0, "L1AnumberRaw2");
	L1AnumberRaw3.reg(7, 0, "L1AnumberRaw3");

    best1f.wire(BESTBITS-1, 0, "best1f"); comment("// read from best track fifo")
	best2f.wire(BESTBITS-1, 0, "best2f");
    bxnf.wire(11, 0, "bxnf");          comment("// read from best track fifo")
    best1w.reg(BESTBITS-1, 0, "best1w"); comment("// best tracks found in l1a_window")
    best2w.reg(BESTBITS-1, 0, "best2w"); comment("// best tracks found in l1a_window")
    bxnw.reg(11, 0, "bxnw");          comment("// bxn for the best tracks foind in l1a_window")
    LctRdEn.reg(3, 0, "LctRdEn");        comment("// LCT fifo read enables")
    RawEmpty.reg(3, 0, "RawEmpty");       comment("// Raw fifo empty flags")
    WindowCnt.reg(3, 0, "WindowCnt");      comment("// l1a window counter")
    OutValid.reg("OutValid");             comment("// best track output register valid")
    RawOutValid.reg(3, 0, "RawOutValid");    comment("// raw fifo output registers valid")
    L1Aint.wire("L1Aint");              comment("// internally generated L1A")
    L1Ar.reg("L1Ar");                 comment("// L1A history (for front detection)")

	// new new

	RawWrAdd.reg(7, 0, "RawWrAdd");   comment("// Raw hits buffer write address")
	RawWrAddf.wire(7, 0, "RawWrAddf"); comment("// Raw hits buffer write address read from FIFO")
	RawRdAdd.reg(7, 0, "RawRdAdd"); comment("// Raw hits buffer read address")
	RawStartAdd.reg(7, 0, "RawStartAdd"); comment("// Raw hits buffer address which must not be overwritten. The data for L1A start here")
	RawCanWrite.wire("RawCanWrite"); comment("// shows if there is space in raw hit memory for one more dump")
	BestFifoCount.wire(7, 0, "BestFifoCount"); comment("// how many words are in best track fifo")
	bxnraw.reg(11, 0, "bxnraw"); comment("// bxn when the raw hit dump started")
	BestCanWrite.reg("BestCanWrite"); comment("// shows if there is space in the best track biffer for one more window")

	rbr_delay.wire(7, 0, "rbr_delay");
	l1a_delaymin1.wire(7, 0, "l1a_delaymin1");
	lybyte.reg(3, 0, "lybyte");

	Reg_(daq, 18, 0);
#ifdef DAQCRC
	Reg_(crcc, 10, 0); comment ("// CRC itself")
	Reg_(crccalc, 1, 0);	  comment ("// tells when to calculate CRC")
#endif
	Wire (davv);
	Reg_(readout_count, 11, 0); comment("// counter of DAQ readouts")


	int i;

	rhm.init("RawHitMemory", "rhm");
	rhm
	(
		RawWrAdd,
		RawRdAdd,
		RawStartAdd,
		fifo_tbins,
		RawCanWrite,
		clk,
		clk,
		(lyr0, lyr1, lyr2, lyr3, lyr4, lyr5),
		(lyt0, lyt1, lyt2, lyt3, lyt4, lyt5),
		L1AoneShot
	);

	bestf.init("bestfifo", "bestf");
	bestf
	(
		clk,
		!hard_rst,
		(/*RawHitsExist,*/ RawWrAdd, L1Anumber, best1d, best2d, bxnd),
		L1AWindow,
		((L1Astate == L1AreadBest0 && !OutValid) || (L1Astate == L1AreadBest2 && L1Anumberf(0) == L1Anumberp)),
		(/*RawHitsExistf,*/ RawWrAddf, L1Anumberf, best1f, best2f, bxnf),
		L1Aempty,
		BestFifoCount
	);

	assign rbr_delay = l1a_delay + fifo_pretrig;
	rbr.init("RollBackRaw", "rbr");
	rbr.dwidth = HCMASKBITS;
	rbr
	(
		(ly0,  ly1,  ly2,  ly3,  ly4,  ly5),
		(lyr0, lyr1, lyr2, lyr3, lyr4, lyr5),
		rbr_delay,
		hard_rst,
		clk
	);	
	
	comment("// best track delay line to compensate l1a_delay time (L1A arrival delay)")
	rbt.init("RollBackTrack", "rbt");
	rbt.dwidth = BESTBITS + BESTBITS + 12;
	rbt
	(
		(best1,  best2 , bxn),
		(best1d, best2d, bxnd),
		l1a_delay,
		hard_rst,
		clk
	);	

	comment("// in case of the internal L1A this delay line will generate the L1Aint at the proper moment, if there was valid track")
	assign l1a_delaymin1 = l1a_delay - 1;
	IntL1Adelay.init("L1ADelayLine", "IntL1Adelay");
	IntL1Adelay.dwidth = 1;
	IntL1Adelay
	(
		ifelse (l1a_internal && best1(3), "1'h1", "1'h0"),
		L1Aint,
		l1a_delaymin1,
		hard_rst,
		clk
	);	

	
#ifdef DAQCRC
	comment("// CRC generator")
	daqcrc.init("crc", "daqcrc");
	daqcrc
	(
		daq, 
		daqp, 
		crccalc, 
		davv,
		clk
	);
#else
	assign daqp = daq;
#endif

	assign validhd = best1d (3);

	comment("// dav bit generator")
	davgen.init("dav", "davgen");
	davgen
	(	
		(L1A && !L1Ar) || L1Aint,       
		L1Anumber,
		best1d (3) || send_empty,  // valid tracks or send_empty is set
		L1AWindow,		
		davv,   
		clk
	);

	always (posedge (clk))
    begin

	  hard_rst_temp = hard_rst.getr().r[0];
	  If (!hard_rst)
	  begin
       	  L1Anumber = l1a_offset - 1;
		  readout_count = l1a_offset;
          L1Astate = L1Aidle;
          L1Astaten = L1Aidle;
		  RawWrAdd = 1; comment("// so raw hit buffer considered empty")
	  end
	  Else
	  begin

		If (L1AWindow)
		begin
			If (WindowCnt < l1a_window) WindowCnt++;
			Else L1AWindow = 0; comment("// track dump is over")
		end

		If (L1AoneShot)
		begin
			If (TimeBin < fifo_tbins) TimeBin++;
			Else 
			begin
				L1AoneShot = 0; comment("// raw dump is over")
//				RawHitsExist = 0;
			end
			RawWrAdd++;
		end

        comment("// expand L1A for fifo_tbins clocks to allow writing the data into raw hit memory")
		comment("// expand L1A for Window clocks to allow writing the data into best track FIFO")
		BestCanWrite = ifelse (BestFifoCount < (255 - 16), 1, 0);
		l1aTP = 0;
		If ((L1A && !L1Ar) || L1Aint) 
		begin
			l1aTP = 1;
			comment("// if there is enough space in both raw hit and best track fifos, start raw hit dump")
			If (RawCanWrite && BestCanWrite && (fifo_mode != 0)) 
			begin
				L1AoneShot = 1; comment("// start raw dump if there is space in the memory")
				TimeBin = 1; 
//				RawHitsExist = 1;
			end
//			else RawHitsExist = 0;

			If ((BestCanWrite && (fifo_mode == 0)) || (RawCanWrite && BestCanWrite && (fifo_mode != 0)))
			begin
				WindowCnt = 1; 
				L1AWindow = 1;  comment("// start track dump")
			end
    		L1Anumber++;
		end
		L1Ar = L1A;        
       
        comment("// L1A processing state machine. Fetches the L1A parameters from L1A fifo and sends the information to DAQ, if necessary.")
        comment("// Compatible with the ALCT 2000 design document. Please see this document (page 46) for details")
   		daq = "19'b100_0000_0000_0000_0000";
   		LctRdEn = 0; 
        begincase (L1Astate)
        	case1(L1Aidle) 
        	begin 
				comment("// idle state. Just waiting for information to appear in the BestTrack fifo")
        		L1Astaten = ifelse (!L1Aempty && !WaitForSpaceInOutFIFO, L1AreadBest0, L1Aidle); comment("// determine next state ")
        		best1w = 0;
        		best2w = 0;
#ifdef DAQCRC
				crccalc = 0;
#endif
        	end
        	
        	case1(L1AreadBest0) 
        	begin
				comment("// reading the information from fifo")
        		L1Astaten = L1AreadBest1;
        		OutValid = 1;

        	end

        	case1(L1AreadBest1) 
        	begin
				comment("// reading the information from fifo")
        		L1Astaten = L1AreadBest2;
        		L1Anumberp = L1Anumberf(0);
//				RawHitsExistw = RawHitsExistf;
				RawStartAdd = RawWrAddf;
				bxnraw = bxnf;
        	end

        	case1(L1AreadBest2) 
        	begin
				comment("// reading the information from fifo")
        		L1Astaten = ifelse ((L1Anumberp == L1Anumberf(0)) && !L1Aempty, L1AreadBest2, L1AsendHeader0);
        		If (L1Anumberp == L1Anumberf(0))
        		begin
	        		comment("// search for the first valid tracks in l1a_window")
    	    		If (best1w == 0 && best1f != 0) 
        			begin
		        		best1w = best1f;
		        		best2w = best2f;
						bxnw   = bxnf;	 
	        		end
					L1Anumberw = L1Anumberf;       		
        		end
        		FrameCount = 0;
        		OutValid = !L1Aempty;

				// if the best track for this L1A was not found, then don't send any data, just go back to idle state
				// later has to be fixed so that if there is no track found, the data are not even written into fifo (dav bit)
				// if send_empty is 1, then send data anyway
				If (L1Astaten == L1AsendHeader0 && best1w == 0 && (!send_empty)) L1Astaten = L1Aidle;

        	end
        	
        	case1(L1AsendHeader0) 
        	begin
				comment("// start sending data")
        		L1Astaten = L1AsendHeader1; comment("// determine next state ")
				daq = ((Signal)"8'b0100_1100", BoardID, CSCID, L1Anumberw(3,0));
				If (best1w == 0) bxnw = bxnf;
#ifdef DAQCRC
				crccalc = 1;
#endif
        	end
        	
        	case1(L1AsendHeader1)
        	begin
        		L1Astaten = L1AsendHeader2; comment("// determine next state ")
				daq = ((Signal)"4'b0000", trig, "5'b0000_1", fifo_tbins, fifo_mode);
        	end
        	
        	case1(L1AsendHeader2)
        	begin
        		L1Astaten = L1AsendHeader3; comment("// determine next state ")
				daq = ((Signal)"4'b0000", acc_trig, bxnraw); comment("// send here the bxn when the raw hit dump started")
        	end
        	
        	case1(L1AsendHeader3)
        	begin
        		L1Astaten = L1AsendLCT0_0; comment("// determine next state ")
				daq = LCTCHIPS; comment("// LCT chips")
        	end

        	case1(L1AsendLCT0_0)
        	begin
        		L1Astaten = L1AsendLCT0_1; comment("// determine next state ")
				daq = (bxnw(2,0), best1w(10,4), "1'b0", best1w(2,0), best1w(3));
        	end

        	case1(L1AsendLCT0_1)
        	begin
        		L1Astaten = L1AsendLCT1_0; comment("// determine next state ")
				daq = (bxn_before_reset, bxnw(4,3)); // send bxn before reset (request from TY/Wesley 10/24/2005)
        	end

        	case1(L1AsendLCT1_0)
        	begin
        		L1Astaten = L1AsendLCT1_1; comment("// determine next state ")
				daq = (bxnw(2,0), best2w(10,4), "1'b0", best2w(2,0), best2w(3));
				RawRdAdd = RawStartAdd;
        	end

        	case1(L1AsendLCT1_1)
        	begin
        		comment("// if fifo_mode disables the Raw hits dump or there was no space in the raw hits dump buffer for this track data - go to evener")
//        		L1Astaten = (RawHitsExistw) ? L1AsendRawHits : L1AsendEvener; // determine next state 
#ifdef DAQCRC
        		L1Astaten = ifelse ((fifo_mode != 0), L1AsendRawHits, L1AsendCRC0); comment("// determine next state ")
#else
        		L1Astaten = ifelse ((fifo_mode != 0), L1AsendRawHits, L1AsendEvener); comment("// determine next state ")
#endif
				daq = (readout_count, bxnw(4,3)); 
				LCTchip = 0; comment("//  set up the counters for the raw hit transmission")
				LyCnt = 0;
				Tbin = 0;
        	end

        	case1(L1AsendRawHits)
        	begin
       			
       			comment("// get the proper layer for transmission (see L1AsendRawHitsN states of the state machine below) ")
       			begincase (LyCnt(3,1))                                                                                
       			    case1(0) lytr = lyt0;                                                                              
       			    case1(1) lytr = lyt1;                                                                              
       			    case1(2) lytr = lyt2;                                                                              
       			    case1(3) lytr = lyt3;                                                                              
       			    case1(4) lytr = lyt4;                                                                              
       			    case1(5) lytr = lyt5;                                                                              
	    		endcase                                                                                          
				                                                                                                 
				lybyte = (LCTchip, LyCnt(0)); comment("// layer byte for transmission")
				for (i = 0; i < LYWG/8; i++)
				{
					If (lybyte == i) bytetr = lytr(i*8+7, i*8);
				}

				daq = ((Signal)"4'b0", LCTchip(1,0), Tbin, bytetr); comment("// only two bits of LCTchip are used here (Kubic has to fix it in the data format)")
				
				comment("// RawRdAddtr and RawEn are processed one clock ahead of everything else, because reading the memory takes one clock")
				If (LyCnt == 10) 
				begin
					comment("// set the address to the beginning of the raw hits for this L1A (....ing compatibility)")
					If (Tbin == fifo_tbins - 1) RawRdAdd = RawStartAdd; 
					Else RawRdAdd++; comment("// increment the address to get to the next time bin")
				end
				
				comment("// finished reading time bin for the current chip")
				If (LyCnt == 11) 
				begin
					comment("// finished reading")
					If (Tbin == fifo_tbins - 1) 
					begin
						LCTchip++; comment("// next chip")
						Tbin = 0;
					end 
					Else Tbin++; comment("// next time bin")
    				
					LyCnt = 0; comment("// reset layer counter")
	    		end
	    		Else LyCnt++;
#ifdef DAQCRC
        		L1Astaten = ifelse ((LCTchip == LCTS), L1AsendCRC0, L1AsendRawHits); comment("// determine next state ")
#else
        		L1Astaten = ifelse ((LCTchip == LCTS), L1AsendEvener, L1AsendRawHits); comment("// determine next state ")
#endif
        	end
        	
#ifdef DAQCRC
        	case1(L1AsendCRC0) 
        	begin
        		L1Astaten = L1AsendCRC1;
        		daq = 0;
				crccalc = 2;
        	end

        	case1(L1AsendCRC1) 
        	begin
        		L1Astaten = L1AsendEvener;
        		daq = 0;
				crccalc = 3;
        	end
#endif

        	case1(L1AsendEvener) 
        	begin
        		L1Astaten = L1AsendFrameCount;
#ifdef DAQCRC
        		daq = "19'hde0d";
				crccalc = 0;
#else        		
				daq = "19'he0d";
#endif
        	end
        	
        	case1(L1AsendFrameCount)
        	begin
        		L1Astaten = L1Aidle;
        		daq = ((Signal)"8'b00111010", FrameCount);
				readout_count++; // count only actually completed readouts
        	end
        	
        	Default L1Astaten = L1Aidle;
        	
        endcase
        FrameCount++; comment("// always counting except in L1AreadBest state")
		L1Astate = L1Astaten;

      end
    end

	// raw hits delay line to compensate the processing time (fifo_pretrig) and l1a_delay time (L1A arrival delay)
	
//	assign Counters = (RawWrAdd, RawRdAdd, RawStartAdd);

endmodule
}
