#include "l1a_maker.h"

void l1a_maker::operator()
(
	Signal l1a_in,
	Signal valor,
	Signal track,
	Signal l1a_outp,
	Signal best_wnd,
	Signal raw_wnd,
	Signal l1a_fifo_full,
	Signal best_full,
	Signal raw_full,
	Signal raw_we_en,
	Signal best_we,
	Signal raw_we,
	Signal l1a_int_en,
    Signal l1a_in_count,
    Signal l1a_offset,
	Signal send_empty,
	Signal reset,
	Signal clk
)
{
initio
	Input (l1a_in); // real l1a input 
    Input (valor);
 	Input (track);  // valid track detected
 	Output (l1a_outp); // processed l1a output
	Input_(best_wnd, 3, 0); // length of best we signal
	Input_(raw_wnd, 4, 0); // length of raw we signal
	Input (l1a_fifo_full);
	Input (best_full); // best memory full
	Input (raw_full); // raw memory full
	Input (raw_we_en); // enable raw we (fifo_mode != 0)
	OutReg (best_we); // best we 
	OutReg (raw_we);  // raw we
	Input (l1a_int_en); // internal l1a enable from configuration
    OutReg_(l1a_in_count, 11, 0);
    Input_(l1a_offset, 3, 0);
	Input (send_empty);
	Input (reset);
	Input (clk);
beginmodule

	Reg (l1ar);
	Reg_(best_cnt, 3, 0);
	Reg_(raw_cnt, 4, 0);
 	Wire (l1a_out); // processed l1a output

	// output l1a if there was a transition 0->1 on the input
	// or internal l1a generated.
	assign l1a_out  = l1a_in && !l1ar; 
	assign l1a_outp = (l1a_out || (track && l1a_int_en)) && !best_full && !raw_full && !l1a_fifo_full && (valor || send_empty);
	
	always (posedge (clk))
	begin
	    If (!reset)
		begin
            l1a_in_count = l1a_offset;
	        l1a_in_count = l1a_in_count - Signal(12,1);
		end
		Else
		begin

		    l1ar = l1a_in;

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


            If (l1a_in)
			    l1a_in_count = l1a_in_count + Signal(12,1);
        end
	end

endmodule
}

