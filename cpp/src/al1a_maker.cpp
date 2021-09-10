#include "al1a_maker.h"

void al1a_maker::operator()
(
	Signal l1a_in,
	Signal track_bxn,
	Signal l1a_outp,
	Signal best_wnd,
	Signal raw_wnd,
	Signal best_full,
	Signal raw_full,
	Signal raw_we_en,
	Signal best_we,
	Signal raw_we,
	Signal l1a_int_en,
	Signal reset,
	Signal clk
)
{
initio
	Input_(l1a_in, BWTMARK, 0); // real l1a input 
 	Input_(track_bxn, MXBXN, 0);  // valid track's bxn to match with l1a (MSB is valid flag)
 	Output (l1a_outp); // processed l1a output
	Input_(best_wnd, 3, 0); // length of best we signal
	Input_(raw_wnd, 4, 0); // length of raw we signal
	Input (best_full); // best memory full
	Input (raw_full); // raw memory full
	Input (raw_we_en); // enable raw we (fifo_mode != 0)
	OutReg (best_we); // best we 
	OutReg (raw_we);  // raw we
	Input (l1a_int_en); // internal l1a enable from configuration
	Input (reset);
	Input (clk);
beginmodule

	Reg (l1ar);
	Reg_(best_cnt, 3, 0);
	Reg_(raw_cnt, 4, 0);
 	Wire (l1a_out); // processed l1a output
	Wire (doutb);
	Wire (l1a_match_empty);
	Reg (l1a_match_emptyr);
	

	Module (l1a_match);
	l1a_match.bwd = 1;
	l1a_match.bwad = BWTMARK;
	l1a_match
	(
		track_bxn(MXBXN-1, 0), // ada - bxn with track
		l1a_in (BWTMARK-1, 0), // adb - l1a time marker       
		"1'b0",                // dina - writing always 0 after we read out l1a time marker
		"1'b1",                // dinb - writing always 1 at l1a time marker location 
		l1a_out,               // dout - output showing match
		doutb,
		"1'b1",		           // wea - always enabled to zero the l1a time marker after one use
		l1a_in (BWTMARK),      // web - MSB is valid flag     
		clk,
		clk      
	);

	// output l1a if there was a transition 0->1 on the input
	// or internal l1a generated.
	assign l1a_outp = l1a_out;
	
	always (posedge (clk))
	begin
		l1ar = l1a_in;
		l1a_match_emptyr = l1a_match_empty;

		If (best_we)
		begin
			If (best_cnt < best_wnd) best_cnt++;
			Else best_we = 0;
		end

		If (raw_we)
		begin
			If (raw_cnt < raw_wnd) raw_cnt++;
			Else raw_we = 0;
		end

		If (l1a_outp && !best_full) 
		begin
			best_we = 1;
			best_cnt = 1;
		end

		If (l1a_outp && !raw_full && raw_we_en)
		begin
			raw_we = 1;
			raw_cnt = 1;
		end

	end

endmodule
}

