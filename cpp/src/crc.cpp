#include "crc.h"


void crc::operator()
(
	Signal d,
	Signal crc,
	Signal calc,
	Signal dav,
	Signal clk
)
{
initio

	Input_(d, 18, 0);
	OutReg_(crc, 18, 0);
	Input_(calc, 2, 0);
	Input (dav);
	Input(clk);

beginmodule

	Reg_(ncrc, 21, 0);
	Reg_(i, 4, 0);
	Reg (t);

comment("// This module implements CRC generation algorithm")
comment("// with the following parameters:")
comment("// Poly           = 10000000000000000000011")
comment("// Data width     = 16")
comment("// CRC width      = 22")
comment("// CRC init       = 0")
comment("// Data bit first = MSB")

	always (posedge (clk))
	begin
		begincase (calc)

			case1(0) // reset CRC
			begin
				ncrc = 0;		
				crc = d;
			end

			case1(1) // calculate
			begin
				For (i = 16, i > 0, i--)
				begin
					t = d(i-1) ^ ncrc(21);
					ncrc(21,2) = ncrc(20,1);
					ncrc(1) = t ^ ncrc(0);
					ncrc(0) = t;
				end
				crc = d;
			end

			case1(2) // send LSW
			begin
				crc = ((Signal)"4'hd", "1'b0", ncrc(10,0));
			end

			case1(3) // send MSW
			begin
				crc = ((Signal)"4'hd", "1'b0", ncrc(21,11));
			end

			case1(4) // freeze CRC
			begin
				crc = d;
			end

		endcase

		crc(17) = dav;
	end

endmodule
}
