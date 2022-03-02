#include "gbtx.h"

void gbtx_c::operator()
(
    Signal daq_word,
    Signal clk,

    Signal elink_p,
    Signal elink_n,
    Signal gbt_tx_datavalid,
    Signal gbt_clk40_p,
    Signal gbt_clk40_n,
    Signal gbt_txrdy,

    Signal rst
)
{
initio
  
    Input_  (daq_word, 18, 0); // DAQ input, same as shipped to DMB
    Input   (clk); // DAQ input clk, 40 MHz sync with LHC

    Output_ (elink_p, 13, 0); // output to GBTX
    Output_ (elink_n, 13, 0);
    Output  (gbt_tx_datavalid); // data valid flag to GBTX
    Input  (gbt_clk40_p); // GBTX TX clk, async
    Input  (gbt_clk40_n);
    Input  (gbt_txrdy); // GBTX ready flag

    Input  (rst);
beginmodule

    Wire (gbt_clk40);
    Wire (gbt_clk160);
    Wire_ (elink, 13, 0);
    Wire_ (el0, 13, 0);
    Wire_ (el1, 13, 0);
    Reg_ (el0_r, 13, 0);
    Reg_ (el1_r, 13, 0);
    Wire  (dv);
    Reg  (dv_r);
		Reg_ (idle_cnt, 2, 0);
    

    //    assign gbt_tx_datavalid = (Signal)"1'b1";
    // MSB in el0 = 1 means frame 0

    #ifdef VGEN
	printv("\tdll_gbtx dllg (.CLK_IN1_P(gbt_clk40_p), .CLK_IN1_N(gbt_clk40_n), .CLK_OUT1(gbt_clk160), .RESET(0), .LOCKED());\n");
	printv("\tOBUFDS elink_buf[13:0] (.I(elink), .O(elink_p), .OB(elink_n));\n");
	printv("\tODDR2 elink_oddr[13:0] (.D0(el0_r), .D1(el1_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(elink));\n");
	printv("\tODDR2 dval_oddr (.D0(dv_r), .D1(dv_r), .C0(gbt_clk160), .C1(!gbt_clk160), .CE(1'b1), .R(1'b0), .S(1'b0), .Q(gbt_tx_datavalid));\n");
        printv("\tgbtx_fifo gbtxf (\n"
	       "\t\t.rst      (rst),\n"
	       "\t\t.wr_clk   (clk),\n"
	       "\t\t.rd_clk   (gbt_clk160),\n"
	       "\t\t.din      ({5'b00000, daq_word[18:10], 4'b0000, daq_word[9:0]}),\n"
	       "\t\t.wr_en    (~daq_word[18]),\n"
	       "\t\t.rd_en    (1'b1),\n"
	       "\t\t.dout     ({el1, el0}),\n"
	       "\t\t.full     (),\n"
	       "\t\t.valid    (dv),\n"
	       "\t\t.empty    ()\n"
	       "\t);\n");

    #else
	assign gbt_clk40 = gbt_clk40_p;
	assign elink_p = elink;
	assign elink_n = ~elink;
    #endif

    always (posedge (gbt_clk160))
    begin
			// test pattern and header
      el0_r = (Signal(1,1), idle_cnt, Signal(10,0));
		  el1_r = (Signal(1,0), idle_cnt, Signal(10,0));
			If (dv) // plug data if valid
			begin
				el0_r = el0_r | el0;
				el1_r = el1_r | el1;
			end
      dv_r  = dv;
			idle_cnt++;
    end

endmodule
}
