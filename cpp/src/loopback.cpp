#include "loopback.h"

void loopback::operator()
(
	Signal seq_cmd,
	Signal alct_rx_1st,
	Signal alct_rx_2nd,

	Signal alct_tx_1st_tpat,
	Signal alct_tx_2nd_tpat,
	Signal alct_sync_mode,

	Signal clock
)
{
initio
	Input_(seq_cmd, 3, 0);
 	Input_(alct_rx_1st, 16,5);
 	Input_(alct_rx_2nd, 16,5);

	Output_(alct_tx_1st_tpat, 28,1);
	Output_(alct_tx_2nd_tpat, 28,1);
	Output (alct_sync_mode);

	Input (clock);
beginmodule

	Wire (alct_sync_teventodd);
	Wire (alct_sync_random_loop);
	Wire (alct_sync_random);

	Reg_(alct_tx_1st_tpat, 28, 1);
	Reg_(alct_tx_2nd_tpat, 28, 1);
	Wire_(alct_rng_1st, 28, 1);
	Wire_(alct_rng_2nd, 28, 1);
	Wire_(alct_sync_adr, 1, 0);
	Wire_(alct_rx_1st_tpat, 9, 0);
	Wire_(alct_rx_2nd_tpat, 9, 0);

//	Reg__(alct_rx_1st_tpat_r, 9, 0, 1, 0); // delay regs to match TMB expectations
//	Reg__(alct_rx_2nd_tpat_r, 9, 0, 1, 0);


	assign alct_sync_mode 		 = (seq_cmd(2) | seq_cmd(0));	// alct_rx(13) in both 1st and 2nd phase beco its DC level
	assign alct_sync_teventodd	 = (seq_cmd(2) & seq_cmd(0));	// alct_rx(13) in both 1st and 2nd phase asserts teven/todd mode
	assign alct_sync_random_loop = (seq_cmd(2) &!seq_cmd(0));	// alct_rx(13) in just 2nd phase asserts pseudo random loopback
	assign alct_sync_adr		 = (seq_cmd(3) , seq_cmd(1));	// alct_rx(14) in both 1st and 2nd phase multiplexed
	assign alct_sync_random		 = (seq_cmd(3) & seq_cmd(1)) && alct_sync_mode;	// Engage lfsr random generator

	assign alct_rx_1st_tpat(9, 0) = (alct_rx_1st(16, 15), alct_rx_1st(12, 5));	// Skip over pairs 13,14 wot carry seq_cmd
	assign alct_rx_2nd_tpat(9, 0) = (alct_rx_2nd(16, 15), alct_rx_2nd(12, 5));	// Skip over pairs 13,14 wot carry seq_cmd

	always (posedge (clock))  
	begin
		If (alct_sync_mode) 
		begin						// Only if in loopback mode
			If (alct_sync_teventodd) 
			begin						// Teven/Tod Mode:
				alct_tx_1st_tpat(28, 01) = Signal(28, 0xAAAAAAA);				//   Load 1010 Teven in all banks
				alct_tx_2nd_tpat(28, 01) = Signal(28, 0x5555555);				//   Load 0101 Todd  in all banks
			end
			Else 
			If (alct_sync_random_loop) 
			begin				// Random loopbback
				alct_tx_1st_tpat(10, 01) =  alct_rx_1st_tpat/*_r[1]*/(9, 0);	//   1st 10 bits are Randoms from TMB
				alct_tx_1st_tpat(20, 11) = ~alct_rx_1st_tpat/*_r[1]*/(9, 0);	//   2nd 10 bits are complemented Randoms from TMB
				alct_tx_1st_tpat(28, 21) =  alct_rx_1st_tpat/*_r[1]*/(7, 0);	//   Last 8 bits are duplicated Randoms from TMB
				alct_tx_2nd_tpat(10, 01) =  alct_rx_2nd_tpat/*_r[1]*/(9, 0);	//   1st 10 bits are Randoms from TMB
				alct_tx_2nd_tpat(20, 11) = ~alct_rx_2nd_tpat/*_r[1]*/(9, 0);	//   2nd 10 bits are complemented Randoms from TMB
				alct_tx_2nd_tpat(28, 21) =  alct_rx_2nd_tpat/*_r[1]*/(7, 0);	//   Last 8 bits are duplicated Randoms from TMB
			end
		 	Else 
			begin
				begincase (alct_sync_adr(1, 0))							// Load 1 of 3 banks at a time
					case1(Signal(2,0)) 
					begin
						alct_tx_1st_tpat(10, 01) = alct_rx_1st_tpat/*_r[1]*/(9, 0);	// Load 1st bank of 10, retains other banks 
						alct_tx_2nd_tpat(10, 01) = alct_rx_2nd_tpat/*_r[1]*/(9, 0);
					end
					case1(Signal(2,1)) 
					begin
						alct_tx_1st_tpat(20, 11) = alct_rx_1st_tpat/*_r[1]*/(9, 0);	// Load 2nd bank of 10, retains other banks 
						alct_tx_2nd_tpat(20, 11) = alct_rx_2nd_tpat/*_r[1]*/(9, 0);
					end
					case1(Signal(2,2)) 
					begin
						alct_tx_1st_tpat(28, 21) = alct_rx_1st_tpat/*_r[1]*/(7, 0);	// Load 3rd bank of  8, retains other banks 
						alct_tx_2nd_tpat(28, 21) = alct_rx_2nd_tpat/*_r[1]*/(7, 0);
					end
					case1(Signal(2,3)) 
					begin
						alct_tx_1st_tpat(28, 01) = alct_rng_1st(28, 1);		// Load Random+ECC in all banks
						alct_tx_2nd_tpat(28, 01) = alct_rng_2nd(28, 1);
					end
				endcase
			end
		end

		// introduce delay into test patterns to match TMB expectations
//		alct_rx_1st_tpat_r[1] = alct_rx_1st_tpat_r[0];
//		alct_rx_1st_tpat_r[0] = alct_rx_1st_tpat;

//		alct_rx_2nd_tpat_r[1] = alct_rx_2nd_tpat_r[0];
//		alct_rx_2nd_tpat_r[0] = alct_rx_2nd_tpat;

	end
	

endmodule
}
