#include "gbtx.h"

void gbtx_c::operator()
(
    Signal daq_word,

    Signal elink_p,
    Signal elink_n,
    Signal gbt_tx_datavalid,
    Signal gbt_clk40_p,
    Signal gbt_clk40_n,
    Signal gbt_txrdy,

	Signal clk160
)
{
initio
  
    Input_  (daq_word, 27, 0); // DAQ word from optical DAQ module, clk160 domain

    Output_ (elink_p, 13, 0); // output to GBTX
    Output_ (elink_n, 13, 0);
    Output  (gbt_tx_datavalid); // data valid flag to GBTX
    Input  (gbt_clk40_p); // GBTX TX clk
    Input  (gbt_clk40_n);
    Input  (gbt_txrdy); // GBTX ready flag

	Output (clk160); // 160M clock derived from GBTX clock, DAQ logic runs on it
beginmodule

	assign gbt_tx_datavalid = Signal(1,1);

    Wire (gbt_clk160);
    Wire_(elink, 13, 0);
    Reg_ (el0_r, 13, 0); // link 0, sent on rising edge
    Reg_ (el1_r, 13, 0); // link 1, sent on falling edge

    #ifdef VGEN
	printv("\tdll_gbtx dllg (.CLK_IN1_P(gbt_clk40_p), .CLK_IN1_N(gbt_clk40_n), .CLK_OUT1(gbt_clk160), .RESET(0), .LOCKED());\n");
	printv("\tOBUFDS elink_buf[13:0] (.I(elink), .O(elink_p), .OB(elink_n));\n");
	printv("\tODDR2 elink_oddr[13:0] (.D0(el0_r), .D1(el1_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(elink));\n");
    #else
	// in simulation, test fixture drives 160M clock directly into gbt_clk40_p
	assign gbt_clk160 = gbt_clk40_p;
	assign elink_p = el0_r;
	assign elink_n = ~el0_r;
    #endif

    always (posedge (gbt_clk160))
    begin
		// 28-bit DAQ word is split between two halves of DDR output
		el0_r = daq_word(13, 0);
		el1_r = daq_word(27,14);
    end

    assign clk160 = gbt_clk160;
endmodule
}
