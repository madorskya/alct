#include "gtp_tux.h"

void gtp_tux_c::operator()
  (
	Signal    daqo,
	Signal    daq_gbtx_valid,
	Signal    clk,

	Signal    tx_p,
	Signal    tx_n,
	Signal    refclk_p,
	Signal    refclk_n,

	Signal    reset,
	Signal    clk160
   
  )
{
initio
    Input_ (daqo, 27, 0); // 28-bit optical DAQ word, clk160 domain
    Input  (daq_gbtx_valid); // 1 during DAQ block transmission (header through word count), aligned with daqo
    Input  (clk); // 40M TTC clock

    Output_ (tx_p, 1, 0);
    Output_ (tx_n, 1, 0);
    Input (refclk_p);
    Input (refclk_n);

    Input    (reset);
	Output   (clk160);

beginmodule

    printv("\toptical_lx150t gtp\n"
            "\t(\n"
            "\t\t.clock     (clk),\n"
            "\t\t.daq_word  (daqo),\n"
            "\t\t.daq_valid (daq_gbtx_valid),\n"
            "\t\t.tx_p      (tx_p),\n"
            "\t\t.tx_n      (tx_n),\n"
            "\t\t.refclk_p  (refclk_p),\n"
            "\t\t.refclk_n  (refclk_n),\n"
            "\t\t.reset_i   (reset),\n"
            "\t\t.clk160    (clk160)\n"
            "\t);\n");

endmodule
}
