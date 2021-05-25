//-------------------------------------------------------------------------------------------------------------------
// 	ECC 16/7 Decoder
//-------------------------------------------------------------------------------------------------------------------

#include "ecc16_decoder.h"

void ecc16_decoder::operator()
(
	Signal dec_in,
	Signal parity_in,
	Signal ecc_en,
	Signal dec_out,
	Signal error
)
{
initio
//-------------------------------------------------------------------------------------------------------------------
// Ports
//-------------------------------------------------------------------------------------------------------------------
	Input_		(dec_in   ,15,0);  // Data to decode
	Input_		(parity_in,5,0);   // ECC parity for input data
	Input		(ecc_en);		   // Enable error correction
	Output_		(dec_out  ,15,0);  // Error corrected out
	Output_	 	(error    ,1,0);   // Error syndrome code

beginmodule
//-------------------------------------------------------------------------------------------------------------------
// Decoder
//-------------------------------------------------------------------------------------------------------------------
// Syndrome creation: 7 syndrome bits are created based on the 32-bit data and its associated checkbits
	Wire_ (syndrome_chk, 5, 0);
	
	assign syndrome_chk(0) = dec_in(0)  ^ dec_in(1)  ^ dec_in(3)  ^ dec_in(4)  ^ dec_in(6)  ^ dec_in(8)  ^ dec_in(10) ^ dec_in(11) ^ dec_in(13) ^ dec_in(15);
	assign syndrome_chk(1) = dec_in(0)  ^ dec_in(2)  ^ dec_in(3)  ^ dec_in(5)  ^ dec_in(6)  ^ dec_in(9)  ^ dec_in(10) ^ dec_in(12) ^ dec_in(13);
	assign syndrome_chk(2) = dec_in(1)  ^ dec_in(2)  ^ dec_in(3)  ^ dec_in(7)  ^ dec_in(8)  ^ dec_in(9)  ^ dec_in(10) ^ dec_in(14) ^ dec_in(15);
	assign syndrome_chk(3) = dec_in(4)  ^ dec_in(5)  ^ dec_in(6)  ^ dec_in(7)  ^ dec_in(8)  ^ dec_in(9)  ^ dec_in(10);
	assign syndrome_chk(4) = dec_in(11) ^ dec_in(12) ^ dec_in(13) ^ dec_in(14) ^ dec_in(15);
	assign syndrome_chk(5) = 
	dec_in(0)  ^ dec_in(1)  ^ dec_in(2)  ^ dec_in(3)  ^ dec_in(4)  ^ dec_in(5)  ^ dec_in(6)  ^ dec_in(7)  ^ 
	dec_in(8)  ^ dec_in(9)  ^ dec_in(10) ^ dec_in(11) ^ dec_in(12) ^ dec_in(13) ^ dec_in(14) ^ dec_in(15) ^
	parity_in(4) ^ parity_in(3) ^ parity_in(2) ^ parity_in(1) ^ parity_in(0);

// Error correction mask
	Reg_ (mask, 15, 0);
	Wire_(syndrome, 5, 0);
	Reg_(error, 1, 0);

	assign syndrome = syndrome_chk ^ parity_in;

	always (syndrome) 
	begin
		begincase (syndrome)
			case1(Signal(6, 0x23)) mask = Signal(16, 0x0001);
			case1(Signal(6, 0x25)) mask = Signal(16, 0x0002);
			case1(Signal(6, 0x26)) mask = Signal(16, 0x0004);
			case1(Signal(6, 0x27)) mask = Signal(16, 0x0008);
			case1(Signal(6, 0x29)) mask = Signal(16, 0x0010);
			case1(Signal(6, 0x2a)) mask = Signal(16, 0x0020);
			case1(Signal(6, 0x2b)) mask = Signal(16, 0x0040);
			case1(Signal(6, 0x2c)) mask = Signal(16, 0x0080);
			case1(Signal(6, 0x2d)) mask = Signal(16, 0x0100);
			case1(Signal(6, 0x2e)) mask = Signal(16, 0x0200);
			case1(Signal(6, 0x2f)) mask = Signal(16, 0x0400);
			case1(Signal(6, 0x31)) mask = Signal(16, 0x0800);
			case1(Signal(6, 0x32)) mask = Signal(16, 0x1000);
			case1(Signal(6, 0x33)) mask = Signal(16, 0x2000);
			case1(Signal(6, 0x34)) mask = Signal(16, 0x4000);
			case1(Signal(6, 0x35)) mask = Signal(16, 0x8000);
			Default	               mask = Signal(16, 0x0000);
		endcase

		If (!syndrome(5))
    	begin
			If (syndrome(4,0) == Signal (5, 0))
				error = Signal(2, 0);// no error 
			Else 
				error = Signal(2, 2);// double error
		end
		Else
		begin 	// detect multiple errors errors in rows101, 110 and 111 are not valid single error
			If (syndrome(4,3) > 0)
				error = Signal(2, 3);
			Else 
				error = Signal(2, 1);// single error
		end
	end

// Corrected output
	assign dec_out = ifelse(ecc_en, (mask ^ dec_in), dec_in);

//-------------------------------------------------------------------------------------------------------------------
// Decoder syndrom error code
//-------------------------------------------------------------------------------------------------------------------
// 00 = no error/data corrected
// 01 = single bit error
// 10 = double bit error
// 11 = single check-bit error
//-------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
	endmodule
//-------------------------------------------------------------------------------------------------------------------
}
