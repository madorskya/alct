#include "jtag.h"

void jtag::operator()
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
	Signal hcounters,
	Signal clk
)
{
initio
	tck.input("tck");
	tms.input("tms");
	tdi.input("tdi");
	tdo.output("tdo", makereg);

	HCmask.output(HCMASKBITS-1, 0, "HCmask", makereg);
	collmask.output(COLLMASKBITS-1, 0, "collmask", makereg);
	ParamReg.output(PRSIZE-1, 0, "ParamReg", makereg);
	ConfgReg.output(CRSIZE-1, 0, "ConfgReg", makereg);
	OutReg_(hmt_thresholds, HMT_THRESHOLD_BITS-1, 0); // all HMT thresholds, combined into one reg
	tst_pls.output("tst_pls", makereg);

	din_dly.output("din_dly", makereg);
	dout_dly.input(DLYLNGRP-1, 0, "dout_dly");
	clk_dly.output("clk_dly");
	input_dis.output("input_dis", makereg);

	YR.output(YRSIZE-1, 0, "YR", makereg);
	OS.input(OSSIZE, 0, "OS");
	OSre.output("OSre", makereg); // output storage read enable

#ifdef S6
	Output			(adc_sck);		// Serial clock
	Output			(adc_sdi);		// Serial data to ADC
	Output			(adc_ncs);		// Chip select active low
	Input			(adc_sdo);		// Serial data from ADC
	Input			(adc_eoc);		// End of conversion
#endif	

	hard_rst.input("hard_rst");
	jstate.output(3, 0, "jstate");
	ID.input(IDSIZE-1, 0, "ID");
	TrigReg.output(4, 0, "TrigReg", makereg);
	SNout.output("SNout", makereg);
	SNin.input("SNin");
	Input_(hcounters, BWHC*HCNUM-1, 0);
	clk.input("clk");

beginmodule


	parameter IRsize ("IRsize", 4);				 // Instruction register size - 1
	parameter SRsize ("SRsize", 4);				 // shift register size - 1
	parameter HCsize ("HCsize", HCMASKBITS-1);	 // HotChannel Mask size - 1
	parameter cmsize ("cmsize", COLLMASKBITS-1); // collision pattern mask size - 1
	parameter PRsize ("PRsize", PRSIZE-1);		 // Parameter register size - 1) 
	parameter CRsize ("CRsize", CRSIZE-1);		 // Configuration register size - 1)
  parameter HMTsize("HMTsize", HMT_THRESHOLD_BITS-1); // HMT threshold size - 1
	parameter YRsize ("YRsize", YRSIZE-1);		 // output register size - 1
	parameter OSsize ("OSsize", OSSIZE);		 // output storage data size
	parameter TRsize ("TRsize", 4);				 // trigger register size - 1
	parameter IDsize ("IDsize", IDSIZE-1);		 // ID register size - 1
	parameter CNsize ("CNsize", BWHC*HCNUM-1);   // hit counters size - 1

// definitions of the TAP states (see standard JTAG TAP state diagram for details)
	parameter RunTestIdle    ("RunTestIdle"    ,1);
	parameter TestLogicReset ("TestLogicReset" ,0);
			                 
	parameter SelDRScan      ("SelDRScan"    ,2); 
	parameter CaptureDR   	 ("CaptureDR"  	 ,3); 
	parameter ShiftDR     	 ("ShiftDR"    	 ,4); 
	parameter Exit1DR     	 ("Exit1DR"    	 ,5); 
	parameter PauseDR     	 ("PauseDR"    	 ,6); 
	parameter Exit2DR     	 ("Exit2DR"    	 ,7); 
	parameter UpdateDR    	 ("UpdateDR"   	 ,8); 
							 				      
	parameter SelIRScan   	 ("SelIRScan"  	 ,9); 
	parameter CaptureIR   	 ("CaptureIR"  	 ,10);
	parameter ShiftIR     	 ("ShiftIR"    	 ,11);
	parameter Exit1IR     	 ("Exit1IR"    	 ,12);
	parameter PauseIR     	 ("PauseIR"    	 ,13);
	parameter Exit2IR     	 ("Exit2IR"    	 ,14);
	parameter UpdateIR    	 ("UpdateIR"   	 ,15);
	
// IR commands definitions

	parameter IDRead        ("IDRead",        0);   // read ID register	
	parameter HCMaskRead    ("HCMaskRead"    ,0x1);
	parameter HCMaskWrite   ("HCMaskWrite"   ,0x2);
	parameter RdTrig        ("RdTrig"        ,0x3); // read trigger register
	parameter WrTrig        ("WrTrig"        ,0x4); // write trigger register

	parameter RdCfg         ("RdCfg"         ,0x6); // read control register
	parameter WrCfg         ("WrCfg"         ,0x7); // write control register

	parameter hmt_read      ("hmt_read",      0x12);
	parameter hmt_write     ("hmt_write",     0xf);

	parameter Wdly          ("Wdly"          ,0xd); // write delay lines. cs_dly bits in ParamReg determine which line to use. Length is 96 bits.
	parameter Rdly          ("Rdly"          ,0xe); // read  delay lines. cs_dly bits in ParamReg determine which line to use. Length is 96 bits.
	parameter YRwrite       ("YRwrite"       ,0x19); // YR is actually an extended config register now
	parameter YRread        ("YRread"        ,0x10);
	parameter CNread        ("CNread"        ,0x11); // hit counters read
#ifdef S6
	parameter ADCread        ("ADCread"        ,0x8); // S6 ADC read
	parameter ADCwrite       ("ADCwrite"       ,0x9); // S6 ADC write
#endif
			                
	parameter CollMaskRead  ("CollMaskRead"  ,0x13);
	parameter CollMaskWrite ("CollMaskWrite" ,0x14);
	parameter ParamRegRead  ("ParamRegRead"  ,0x15);
	parameter ParamRegWrite ("ParamRegWrite" ,0x16);
	// this command is used to enable the output clocking for ClkCount clocks. If ClkCount==0xff then clock is enabled forever
	parameter InputEnable   ("InputEnable"   ,0x17); 
	parameter InputDisable  ("InputDisable"  ,0x18); 
	parameter OSread        ("OSread"        ,0x1a);

	// commands to access the serial number
	parameter SNread        ("SNread"        ,0x1b);
	parameter SNwrite0      ("SNwrite0"      ,0x1c);
	parameter SNwrite1      ("SNwrite1"      ,0x1d);
	parameter SNreset       ("SNreset"       ,0x1e);
			                
	parameter Bypass        ("Bypass"        ,0x1f);

	// states for DS2401 readout state machine
	parameter SNidleST      ("SNidleST",   0);
	parameter SNwriteST     ("SNwriteST",  1);
	parameter SNreadST		("SNreadST",   2);
	parameter SNsampleST	("SNsampleST", 3);


	ParamRegs.reg(PRsize, 0, "ParamRegs"); 
    // parameters register: see components at the top of this file
	ConfgRegs.reg(CRsize, 0, "ConfgRegs"); 
	Reg_(hmt_thresholds_s, HMTsize, 0);
    // configuration register: see components at the top of this file
	dly_clk_en.reg("dly_clk_en"); // enable clk_dly bit

	TAPstate.reg (3, 0, "TAPstate");
	IR.reg (IRsize, 0, "IR"); // instruction register (parallel)
	bpass.reg("bpass");
	sr.reg (SRsize, 0, "sr"); //shift register
#ifdef S6
	tdomux.reg (15, 0, "tdomux");
#else
	tdomux.reg (12, 0, "tdomux");
#endif
	dly_tdo.reg("dly_tdo");
	Reg_(dout_dly_r, DLYLNGRP-1, 0);
	YRs.reg (YRsize, 0, "YRs"); // extended configuration register
	OSs.reg (OSsize, 0, "OSs");  // output storage shift register
	TrigRegs.reg(TRsize, 0, "TrigRegs");
	IDs.reg(IDsize, 0, "IDs"); // ID register shifter
	SNcnt.reg(11, 0, "SNcnt");
	SNrd.reg("SNrd");

	SNresetRQ.reg("SNresetRQ");
	SNwrite0RQ.reg("SNwrite0RQ");
	SNwrite1RQ.reg("SNwrite1RQ");
	SNreadRQ.reg("SNreadRQ");
	SNresetRQ1.reg("SNresetRQ1");
	SNwrite0RQ1.reg("SNwrite0RQ1");
	SNwrite1RQ1.reg("SNwrite1RQ1");
	SNreadRQ1.reg("SNreadRQ1");
	SNresetRQ2.reg("SNresetRQ2");
	SNwrite0RQ2.reg("SNwrite0RQ2");
	SNwrite1RQ2.reg("SNwrite1RQ2");
	SNreadRQ2.reg("SNreadRQ2");

	SNstate.reg(1, 0, "SNstate");

	Reg_(hcounterss, BWHC*HCNUM-1, 0);

#ifdef VGEN
	printv("initial input_dis = 0;\n");
	printv("initial TAPstate = RunTestIdle;\n");
#endif

#ifdef S6
	Reg_(adc_wr_reg, 4, 0);
	Wire_(adc_rd_reg, 4, 0);
	Reg_(adc_wr_sr, 4, 0);
	Reg_(adc_rd_sr, 4, 0);


	assign adc_sck = adc_wr_reg(0);			// Serial clock
	assign adc_sdi = adc_wr_reg(1);			// Serial data to ADC
	assign adc_ncs = adc_wr_reg(2);			// Chip select active low

	assign adc_rd_reg(0) = adc_wr_reg(0);	// Serial clock
	assign adc_rd_reg(1) = adc_wr_reg(1);	// Serial data to ADC
	assign adc_rd_reg(2) = adc_wr_reg(2);	// Chip select active low
	assign adc_rd_reg(3) = adc_sdo;			// Serial data from ADC
	assign adc_rd_reg(4) = adc_eoc;			// End of conversion
#endif

	// standard TAP	state machine implementation
	always (posedge (tck) or negedge (hard_rst))
	begin
	
		If (hard_rst == 0)
		begin
			// initializing the registers on reset
			collmask    = 0; 
			collmask = ~collmask; //put all ones 
			HCmask      = 0; 
			HCmask = ~HCmask; //put all ones 
			ParamReg    = "9'b1111111_01";
			ConfgReg    = "69'b01_0_00_00_1_0_0_000_101_0_0001_0011_01111000_000_01_00001_00111_11_100_010_00000001_0_0_0_00";
			// default values for ME2/1
			hmt_thresholds = (Signal(10, 1), Signal(10, 1), Signal(10, 1));
			input_dis = 0;
			TAPstate = RunTestIdle;
#ifdef S6
			adc_wr_reg(0) = 0;						// Serial clock
			adc_wr_reg(1) = 0;						// Serial data to ADC
			adc_wr_reg(2) = 1;						// Chip select active low
			adc_wr_reg(3) = 0;						// Unused
			adc_wr_reg(4) = 0;						// Unused
#endif

		end
		Else
		begin
			dout_dly_r = dout_dly; // lock dout_dly into IOB
			dly_clk_en = 0;	// assignment of delay line clock enable for all else cases
			OSre = 0;
			begincase (TAPstate)
			
	 			case1(RunTestIdle) TAPstate = ifelse (tms == 0, RunTestIdle , SelDRScan);
				case1(TestLogicReset) TAPstate = ifelse (tms == 0, RunTestIdle , TestLogicReset);
	 			
//	Data register states-------------------------------------------------
		 		case1(CaptureDR)
	 				begin
	 					TAPstate = ifelse (tms == 0, ShiftDR , Exit1DR);
	 					
						begincase (IR)
							case2(HCMaskWrite, HCMaskRead)       tdomux = 1; 
							case2(CollMaskWrite, CollMaskRead)   tdomux = 2;
							case1(ParamRegWrite)                 tdomux = 4; 
							case1(ParamRegRead)            begin tdomux = 4; ParamRegs = ParamReg; end
							case1(WrCfg)                         tdomux = 8; 
							case1(RdCfg)                   begin tdomux = 8; ConfgRegs = ConfgReg; end
							case2(Wdly, Rdly)                    tdomux = 16;
							case1(Bypass)                  begin tdomux = 32; bpass = 0; end
							case1(OSread)                  begin tdomux = 128; OSs = OS; OSre = 1;end
							case1(WrTrig)                        tdomux = 256; 
							case1(RdTrig)                  begin tdomux = 256; TrigRegs = TrigReg; end
							case1(IDRead)				   begin tdomux = 512; IDs = ID; end
							case1(SNread)				         tdomux = 1024;    
							case1(YRwrite)				   		 tdomux = 2048;
							case1(YRread)				   begin tdomux = 2048; YRs = YR; end   
							case1(CNread)				   begin tdomux = 4096; hcounterss = hcounters; end   
#ifdef S6
							case1(ADCread)				   begin tdomux = 8192;  adc_rd_sr = adc_rd_reg; end
							case1(ADCwrite)				   begin tdomux = 16384; adc_wr_sr = adc_wr_reg;end
							case1(hmt_write)         tdomux = (1 << 15); 
							case1(hmt_read)    begin tdomux = (1 << 15); hmt_thresholds_s = hmt_thresholds; end
#endif
							Default                              tdomux = 0;
						endcase
	 					
	 				end
		 		case1(ShiftDR)
		 			begin
	 					TAPstate = ifelse (tms == 0, ShiftDR , Exit1DR);
						// get the proper bit to tdo according to the original register length
	 					begincase (IR) 
	 						case2(HCMaskWrite  , HCMaskRead  ) HCmask    = (tdi, HCmask(HCsize,1));
	 						case2(CollMaskWrite, CollMaskRead) collmask  = (tdi, collmask(cmsize,1));
	 						case2(ParamRegWrite, ParamRegRead) ParamRegs = (tdi, ParamRegs(PRsize,1));
	 						case2(RdCfg        , WrCfg       ) ConfgRegs = (tdi, ConfgRegs(CRsize,1));
		 					case1(Bypass)                       bpass     = tdi; 
							// in case of Delay line get tdi to the output, and enable the clk_dly (see assignment above the case statement)
	 						case2(Wdly, Rdly)                   begin din_dly = tdi; dly_clk_en = 1; end
	 						case2(YRwrite, YRread)              YRs       = (tdi, YRs(YRsize,1));
	 						case1(CNread)              			hcounterss = (tdi, hcounterss(CNsize,1));
	 						case1(OSread)                       OSs       = (tdi, OSs(OSsize,1));
	 						case2(RdTrig, WrTrig)               TrigRegs = (tdi, TrigRegs(TRsize,1));
							case1(IDRead)						IDs = (tdi, IDs(IDsize, 1));
#ifdef S6
	 						case2(hmt_write    , hmt_read    ) hmt_thresholds_s = (tdi, hmt_thresholds_s(HMTsize,1));
							case1(ADCread)	adc_rd_sr	 = (tdi,adc_rd_sr(4,1));
							case1(ADCwrite)	adc_wr_sr	 = (tdi,adc_wr_sr(4,1));
#endif
	 					endcase
		 			end
		 		case1(UpdateDR)
		 			begin
		 				TAPstate = ifelse (tms == 0, RunTestIdle, SelDRScan);
		 				begincase (IR)
		 					case1(ParamRegWrite) ParamReg = ParamRegs;
		 					case1(WrTrig       ) begin TrigReg  = TrigRegs; tst_pls = ifelse (TrigRegs(3,0) == 3, 1, 0); end
		 					case1(YRwrite      ) YR       = YRs;
							case1(WrCfg        ) ConfgReg = ConfgRegs;
#ifdef S6
							case1(hmt_write    ) hmt_thresholds = hmt_thresholds_s;
							case1(ADCwrite     ) adc_wr_reg	= adc_wr_sr;
#endif							
	 					endcase
		 			end
	 			case1(SelDRScan) TAPstate = ifelse (tms == 0, CaptureDR , SelIRScan);
		 		case1(Exit1DR  ) TAPstate = ifelse (tms == 0, PauseDR , UpdateDR);
		 		case1(PauseDR  ) TAPstate = ifelse (tms == 0, PauseDR , Exit2DR);
	 			case1(Exit2DR  ) TAPstate = ifelse (tms == 0, ShiftDR , UpdateDR);
	 			
// Instruction register states-------------------------------------------	
	 			case1(CaptureIR)
		 			begin                                                  
		 				TAPstate = ifelse (tms == 0, ShiftIR , Exit1IR);         
	 					sr = IR; // read IR into the sr
	 					tdomux = 64;
	 				end                                                    
		 		case1(ShiftIR)
		 			begin                                                  
	 					TAPstate = ifelse (tms == 0, ShiftIR , Exit1IR);         
	 					sr = (tdi, sr(SRsize,1)); // shift
		 			end                                                    
		 		case1(UpdateIR)
        	        begin
            	        TAPstate = ifelse (tms == 0, RunTestIdle , SelDRScan);   
                	    IR = sr;
                    	// decode command
	                    begincase (IR)
    	                	case1(InputEnable)  input_dis = 0;
        	            	case1(InputDisable) input_dis = 1;
							case1(SNreset)      SNresetRQ  = ~SNresetRQ;
							case1(SNwrite0)     SNwrite0RQ = ~SNwrite0RQ;
							case1(SNwrite1)     SNwrite1RQ = ~SNwrite1RQ;
							case1(SNread)       SNreadRQ   = ~SNreadRQ;
            	        endcase
                	end
		 		case1(SelIRScan) TAPstate = ifelse (tms == 0, CaptureIR , TestLogicReset);     
		 		case1(Exit1IR  ) TAPstate = ifelse (tms == 0, PauseIR , UpdateIR);        
	 			case1(PauseIR  ) TAPstate = ifelse (tms == 0, PauseIR , Exit2IR);         
	 			case1(Exit2IR  ) TAPstate = ifelse (tms == 0, ShiftIR , UpdateIR);        
			                                                           
			endcase	                            
		end
	end

	assign clk_dly = ifelse (dly_clk_en, !tck , 0); // special case of the clock for the delay line. Have to use combinational logic here.

	always (negedge (tck))
	begin

		  // combinatorial logic for dly_tdo moved here to it's not affected by input timing 
			dly_tdo = (ror (dout_dly_r & (~ParamReg(DLYLNGRP+1,2))));
			tdo = 
				(tdomux(0) & HCmask(0))    |
				(tdomux(1) & collmask(0))  |
				(tdomux(2) & ParamRegs(0)) | 
				(tdomux(3) & ConfgRegs(0)) | 
				(tdomux(4) & dly_tdo)      |
				(tdomux(5) & bpass)        | 
				(tdomux(6) & sr(0))        |
				(tdomux(7) & OSs(0))       |
				(tdomux(8) & TrigRegs(0))  |
				(tdomux(9) & IDs(0))       |
				(tdomux(10) & SNrd)		   |
				(tdomux(11) & YRs(0)) 	   |
				(tdomux(12) & hcounterss(0));
#ifdef S6
			tdo = tdo |
				(tdomux(13) & adc_rd_sr(0)) |
				(tdomux(14) & adc_wr_sr(0)) |
				(tdomux(15) & hmt_thresholds_s(0));
#endif
	end
	
	assign jstate = ~TAPstate;


	// state machine for DS2401 readout
	always (posedge (clk))
	begin

		begincase (SNstate)

			case1(SNidleST)
			begin
				// analyze requests
				If (SNresetRQ2  ^ SNresetRQ1)  begin SNout = 0; end
				If (SNwrite0RQ2 ^ SNwrite0RQ1) begin SNstate = SNwriteST; SNout = 0; SNcnt = 2400; end // 60 us
				If (SNwrite1RQ2 ^ SNwrite1RQ1) begin SNstate = SNwriteST; SNout = 0; SNcnt = 240; end  // 6 us
				If (SNreadRQ2   ^ SNreadRQ1)   begin SNstate = SNreadST;  SNout = 0; SNcnt = 240; end  // 6 us
			end

			case1(SNwriteST)
			begin
				If (SNcnt == 0) 
				begin
					SNstate = SNidleST;
					SNout = 1;
				end
				Else SNcnt--;
			end

			case1(SNreadST)
			begin
				If (SNcnt == 0) 
				begin
					SNstate = SNsampleST;
					SNcnt = 360; // 9 us
					SNout = 1;
				end
				Else SNcnt--;
			end

			case1(SNsampleST)
			begin
				If (SNcnt == 0) 
				begin
					SNstate = SNidleST;
					SNrd = SNin;
				end
				Else SNcnt--;
			end
		endcase

		SNresetRQ2  = SNresetRQ1; 	
		SNwrite0RQ2 = SNwrite0RQ1;
		SNwrite1RQ2 = SNwrite1RQ1;
		SNreadRQ2   = SNreadRQ1;  

		SNresetRQ1  = SNresetRQ; 	
		SNwrite0RQ1 = SNwrite0RQ;
		SNwrite1RQ1 = SNwrite1RQ;
		SNreadRQ1   = SNreadRQ;  
	end

endmodule
}


