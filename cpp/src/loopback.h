#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class loopback: public module
{
 public:
	void operator()
	(
		Signal seq_cmd,
		Signal alct_rx_1st,
		Signal alct_rx_2nd,
		
		Signal alct_tx_1st_tpat,
		Signal alct_tx_2nd_tpat,
		Signal alct_sync_mode,
		
		Signal clock
	);

	Signal
		alct_sync_teventodd,
		alct_sync_random_loop,
		alct_sync_adr,
		alct_sync_random,
		alct_tx_1st_tpat,
		alct_tx_2nd_tpat,
		alct_rng_1st,
		alct_rng_2nd,
		alct_rx_1st_tpat,
		alct_rx_2nd_tpat;

	memory alct_rx_1st_tpat_r, alct_rx_2nd_tpat_r;

};
