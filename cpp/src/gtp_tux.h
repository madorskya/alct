#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class gtp_tux_c: public module
{
 public:
  void operator()
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
   
  );
};
