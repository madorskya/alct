#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class gbtx_c: public module
{
 public:
  void operator()
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
  );

  Signal gbt_clk40;
  Signal gbt_clk160;
  Signal elink;
  Signal el0, el1;
  Signal el0_r;
  Signal el1_r;
  Signal dv;
  Signal dv_r;

};
