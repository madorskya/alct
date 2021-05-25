#include "ecc32_encoder.h"
//-------------------------------------------------------------------------------------------------------------------
// 	ECC 32-data/7-parity Encoder
//	02/19/09	Initial copied from Xilinx xapp645 top_32b_EDC.v
//	02/23/09	Removed FFs + reorganized into separate ecoder/decoder sections
//-------------------------------------------------------------------------------------------------------------------
void ecc32_encoder::operator() 
(
	Signal enc_in,
	Signal enc_out,
	Signal parity_out,
	Signal clk
)
{
initio
//-------------------------------------------------------------------------------------------------------------------
// Ports
//-------------------------------------------------------------------------------------------------------------------
	Input_(enc_in, 31, 0);
	OutReg_(enc_out, 31, 0);
	Output_(parity_out, 6, 0);
	Input (clk);
beginmodule
//-------------------------------------------------------------------------------------------------------------------
// Encoder checkbit generator equations
//-------------------------------------------------------------------------------------------------------------------
	Wire_(enc_chk, 6, 0);

	always (posedge(clk))
	begin
		enc_out = enc_in;
	end

	assign enc_chk(0) = enc_out(0)  ^ enc_out(1)  ^ enc_out(3)  ^ enc_out(4)  ^ enc_out(6)  ^ enc_out(8)  ^ enc_out(10) ^ enc_out(11) ^ enc_out(13) ^ enc_out(15) ^ enc_out(17) ^ enc_out(19) ^ enc_out(21) ^ enc_out(23) ^ enc_out(25) ^ enc_out(26) ^ enc_out(28) ^ enc_out(30);
	assign enc_chk(1) = enc_out(0)  ^ enc_out(2)  ^ enc_out(3)  ^ enc_out(5)  ^ enc_out(6)  ^ enc_out(9)  ^ enc_out(10) ^ enc_out(12) ^ enc_out(13) ^ enc_out(16) ^ enc_out(17) ^ enc_out(20) ^ enc_out(21) ^ enc_out(24) ^ enc_out(25) ^ enc_out(27) ^ enc_out(28) ^ enc_out(31);
	assign enc_chk(2) = enc_out(1)  ^ enc_out(2)  ^ enc_out(3)  ^ enc_out(7)  ^ enc_out(8)  ^ enc_out(9)  ^ enc_out(10) ^ enc_out(14) ^ enc_out(15) ^ enc_out(16) ^ enc_out(17) ^ enc_out(22) ^ enc_out(23) ^ enc_out(24) ^ enc_out(25) ^ enc_out(29) ^ enc_out(30) ^ enc_out(31);
	assign enc_chk(3) = enc_out(4)  ^ enc_out(5)  ^ enc_out(6)  ^ enc_out(7)  ^ enc_out(8)  ^ enc_out(9)  ^ enc_out(10) ^ enc_out(18) ^ enc_out(19) ^ enc_out(20) ^ enc_out(21) ^ enc_out(22) ^ enc_out(23) ^ enc_out(24) ^ enc_out(25);
	assign enc_chk(4) = enc_out(11) ^ enc_out(12) ^ enc_out(13) ^ enc_out(14) ^ enc_out(15) ^ enc_out(16) ^ enc_out(17) ^ enc_out(18) ^ enc_out(19) ^ enc_out(20) ^ enc_out(21) ^ enc_out(22) ^ enc_out(23) ^ enc_out(24) ^ enc_out(25);
	assign enc_chk(5) = enc_out(26) ^ enc_out(27) ^ enc_out(28) ^ enc_out(29) ^ enc_out(30) ^ enc_out(31);
	assign enc_chk(6) =
	enc_out(0)  ^ enc_out(1)  ^ enc_out(2)  ^ enc_out(3)  ^ enc_out(4)  ^ enc_out(5)  ^ enc_out(6)  ^ enc_out(7)  ^
	enc_out(8)  ^ enc_out(9)  ^ enc_out(10) ^ enc_out(11) ^ enc_out(12) ^ enc_out(13) ^ enc_out(14) ^ enc_out(15) ^
	enc_out(16) ^ enc_out(17) ^ enc_out(18) ^ enc_out(19) ^ enc_out(20) ^ enc_out(21) ^ enc_out(22) ^ enc_out(23) ^
	enc_out(24) ^ enc_out(25) ^ enc_out(26) ^ enc_out(27) ^ enc_out(28) ^ enc_out(29) ^ enc_out(30) ^ enc_out(31) ^
	enc_chk(5) ^ enc_chk(4) ^ enc_chk(3) ^ enc_chk(2) ^ enc_chk(1) ^ enc_chk(0);

	assign	parity_out = enc_chk;

//-------------------------------------------------------------------------------------------------------------------
endmodule
//-------------------------------------------------------------------------------------------------------------------
}
