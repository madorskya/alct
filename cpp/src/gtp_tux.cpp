#include "gtp_tux.h"

void gtp_tux_c::operator()
  (
	Signal    daqo,
	Signal    clk,

	Signal    tx_p,
	Signal    tx_n,
	Signal    refclk_p,
	Signal    refclk_n,

	Signal    reset
   
  )
{
initio
    Input_ (daqo, 18, 0);
    Input  (clk);

    Output_ (tx_p, 1, 0);
    Output_ (tx_n, 1, 0);
    Input (refclk_p);
    Input (refclk_n);

    Input    (reset);

beginmodule

    printv("\toptical_lx150t gtp\n"
            "\t(\n"
            "\t\t.clock    (clk),\n"
            "\t\t.daq_word (daqo),\n"
            "\t\t.tx_p     (tx_p),\n"
            "\t\t.tx_n     (tx_n),\n"
            "\t\t.refclk_p (refclk_p),\n"
            "\t\t.refclk_n (refclk_n),\n"
            "\t\t.reset_i  (reset)\n"
            "\t);\n");

endmodule
}
