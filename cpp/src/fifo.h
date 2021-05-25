
#include "vlib.h"
#include "vmac.h"
#include "RawHitMemory.h"
#include "bestfifo.h"
#include "rollbackraw.h"
#include "crc.h"
#include "dav.h"

class fifo: public module
{
 public:
    void operator()
	(
	    Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,   // layer data
	    Signal best1,              			// best track parameters 
	    Signal best2,              			// second best track parameters
	    Signal bxn,                            // bx number
	    Signal fifo_tbins,                     // length of the dump for one L1A
	    Signal daqp,                            // output to daq (including all service bits) 
	    Signal l1a_delay,                       // time back to get the raw hits corresponding to this to L1A
	    Signal fifo_pretrig,                     // delay of the input raw hit data to compensate for the processing and pretriggering
	    Signal fifo_mode,                      // bits from configuration register defining the raw hits dump mode
	    Signal BoardID, 
	    Signal CSCID, 
	    Signal L1A,
	    Signal hard_rst,
	    Signal l1a_internal, Signal l1a_window, Signal l1a_offset,
	    Signal WaitForSpaceInOutFIFO,
	    Signal L1AWindow, Signal l1aTP, Signal validhd,
		Signal send_empty, // set this to 1 to report DAQ for empty events
		Signal bxn_before_reset,
		Signal trig, Signal acc_trig, 
	    Signal clk
	    );

    Signal lyr0, lyr1, lyr2, lyr3, lyr4, lyr5; // delayed inputs
    Signal lyt0, lyt1, lyt2, lyt3, lyt4, lyt5; // output of the Raw hits memory. These registers are also uesd for multiplexing the raw hits during the transmission to DAQ
    Signal lytr; // layer for the transmission
     
    Signal best1r; // parameters of the 3 best muons 3.2 mks back from L1A (in 3 clock window)
    Signal best2r; // parameters of the second 3 best muons 3.2 mks back from L1A (in 3 clock window)
    Signal best1tr; // parameters of the 3 best muons 3.2 mks back from L1A (in 3 clock window) prepared for transmission
    Signal best2tr; // parameters of the second 3 best muons 3.2 mks back from L1A (in 3 clock window) prepared for transmission
    memory q1tr;
    memory q2tr; // temp registers for the quality of the transmitted tracks
    Signal zz5; // dummy
    Signal z4; // dummy
    Signal z5; // dummy
    Signal L1Aempty; // flag showing that L1A fifo is empty
    Signal L1Astate; // state machine state (see definitions at the top of the file)
    Signal L1Astaten; // new state machine state (see definitions at the top of the file)
    Signal L1Anumber; // L1A number counter. Reset by hard_rst
    Signal quality; // used to check if there are any valid tracks corresponding to this L1A
    Signal t; // temp variable
    Signal bxnr, bxntr; // 5 lsb of the bxn for the best track
    Signal bxnlast; // this wire is always one clock behind the bxn (see comments to BestTrkBuffer)
    Signal LCTchip; // chip counter (for compatibility with the old readout format)
    Signal Tbin;    // time bin counter (for compatibility with the old readout format)
    Signal LyCnt;   // layer counter for transmission
    Signal bytetr;  // byte from the layer to transmit
    Signal FrameCount; // frame counter for transmission
    Signal BxnRdAdd; // read addr for the memory keeping track of the bx numbers for the raw hits buffer
    Signal RawWritten, RawWrittenf; // shows if raw hits were written for this best track
     
    // new
    Signal best1d, best2d; // best tracks delayed for l1a_delay clocks
    Signal bxnd; // bxn delayed for l1a_delay clocks
    Signal TimeBin; // Time bin number when writing into Raw hits fifo
    Signal L1AoneShot; // expanded L1A for raw hit recording
    Signal Windowcnt; // L1A valid track window counter
    Signal zz3, zzz3, zzzz3, zzzzz3; // dummy
    Signal z8;
    Signal z31;
    Signal L1Anumberf;     // read from best track fifo
    Signal L1Anumberp;     // previous L1A number's lsb
    Signal L1Anumberw;     // number for readout
    Signal L1AnumberRaw0, L1AnumberRaw1, L1AnumberRaw2, L1AnumberRaw3;   // raw hit L1A number
    Signal best1f, best2f; // read from best track fifo
    Signal bxnf;          // read from best track fifo
    Signal best1w, best2w; // best tracks found in l1a_window
    Signal bxnw;          // bxn for the best tracks foind in l1a_window
    Signal LctRdEn;        // LCT fifo read enables
    Signal RawEmpty;       // Raw fifo empty flags
    Signal WindowCnt;      // l1a window counter
    Signal OutValid;             // best track output register valid
    Signal RawOutValid;    // raw fifo output registers valid
    Signal L1Aint;              // internally generated L1A
    Signal L1Ar;                 // L1A history (for front detection)

    // new new

    Signal RawWrAdd;   // Raw hits buffer write address
    Signal RawWrAddf; // Raw hits buffer write address read from FIFO
    Signal RawRdAdd; // Raw hits buffer read address
    Signal RawStartAdd; // Raw hits buffer address which must not be overwritten. The data for L1A start here
    Signal RawCanWrite; // shows if there is space in raw hit memory for one more dump
    Signal BestFifoCount; // how many words are in best track fifo
    Signal bxnraw; // bxn when the raw hit dump started
    Signal BestCanWrite; // shows if there is space in the best track biffer for one more window
    Signal rbr_delay;
    Signal l1a_delaymin1;
    Signal lybyte;
	unsigned hard_rst_temp;

    RawHitMemory rhm;
    bestfifo bestf;
    RollBackRaw rbr;
    RollBackRaw rbt;
    RollBackRaw IntL1Adelay;

    Signal daq;
#ifdef DAQCRC
    crc daqcrc;
    Signal crcc, crcreset, crccalc;
#endif

    dav davgen; // "data available" bit generator
    Signal davv, readout_count;
	

};

