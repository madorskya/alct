#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class gtp_tux_c: public module
{
 public:
  void operator()
  (
	Signal    daqo,
	Signal    clk,

	Signal    tx_p,
	Signal    tx_n,
	Signal    refclk_p,
	Signal    refclk_n,

	Signal    reset
   
  );
};
