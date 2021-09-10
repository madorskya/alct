#include "collider.h"


#define H "1'b1"
#define L "1'b0"
#define NOTRK "7'h7f"

void collider::operator()
(
	Signal qp0, Signal qp1, Signal qp2, Signal qp3, 
	Signal w1, Signal w2, 
	Signal q1, Signal q2, 
	Signal v1, Signal v2,
	Signal clk
)
{
initio	

	Input_(qp0, LYWG-1, 0); // quality positional codes
	Input_(qp1, LYWG-1, 0); 
	Input_(qp2, LYWG-1, 0);
	Input_(qp3, LYWG-1, 0);
	
	OutReg_(w1, KEYBITS-1, 0); // wiregroups for 2 best tracks
	OutReg_(w2, KEYBITS-1, 0);

	OutReg_(q1, QBITS-1, 0); // qualities for 2 best tracks
	OutReg_(q2, QBITS-1, 0);

	OutReg (v1); // valid flags for 2 best tracks
	OutReg (v2);

	Input (clk);

beginmodule

	Reg_(n3_1, KEYBITS-1, 0);
	Reg_(n3_2, KEYBITS-1, 0);
		 	 					
	Reg_(n2_1, KEYBITS-1, 0);
	Reg_(n2_2, KEYBITS-1, 0);
		 	 					
	Reg_(n1_1, KEYBITS-1, 0);
	Reg_(n1_2, KEYBITS-1, 0);

	Reg_(n0_1, KEYBITS-1, 0);
	Reg_(n0_2, KEYBITS-1, 0);

	int i;

	always (qp0 or qp1 or qp2 or qp3)
	begin
		n3_1 = NOTRK;
		n2_1 = NOTRK;
		n1_1 = NOTRK;
		n0_1 = NOTRK;
		for (i = 0; i < LYWG; i++) // encoder 1 works from 0 to max wg
		{
			If (qp3(i)) n3_1 = i;
			If (qp2(i)) n2_1 = i;
			If (qp1(i)) n1_1 = i;
			If (qp0(i)) n0_1 = i;
		}

		n3_2 = NOTRK;
		n2_2 = NOTRK;
		n1_2 = NOTRK;
		n0_2 = NOTRK;
		for (i = LYWG-1; i >= 0; i--) // encoder 2 works from max wg to 0
		{
			If (qp3(i)) n3_2 = i;
			If (qp2(i)) n2_2 = i;
			If (qp1(i)) n1_2 = i;
			If (qp0(i)) n0_2 = i;
		}

		q1 = 0;	q2 = 0;
		w1 = 0; w2 = 0;
		v1 = 0; v2 = 0;


		If (n3_1 != NOTRK)
		begin
			w1 = n3_1; q1 = 3; v1 = 1;
			If (n3_1 != n3_2)  begin w2 = n3_2; q2 = 3; v2 = 1; end Else 
			If (n2_1 != NOTRK) begin w2 = n2_1; q2 = 2; v2 = 1; end Else 
			If (n1_1 != NOTRK) begin w2 = n1_1; q2 = 1; v2 = 1; end Else
			If (n0_1 != NOTRK) begin w2 = n0_1; q2 = 0; v2 = 1; end
		end
		Else
		begin
			If (n2_1 != NOTRK)
			begin
				w1 = n2_1; q1 = 2; v1 = 1;
				If (n2_1 != n2_2)  begin w2 = n2_2; q2 = 2; v2 = 1; end Else 
				If (n1_1 != NOTRK) begin w2 = n1_1; q2 = 1; v2 = 1; end Else 
				If (n0_1 != NOTRK) begin w2 = n0_1; q2 = 0; v2 = 1; end
			end
			Else
			begin
				If (n1_1 != NOTRK)
				begin
					w1 = n1_1; q1 = 1; v1 = 1;
					If (n1_1 != n1_2)  begin w2 = n1_2; q2 = 1; v2 = 1; end Else 
					If (n0_1 != NOTRK) begin w2 = n0_1; q2 = 0; v2 = 1; end
				end
				Else
				begin
					If (n0_1 != NOTRK)
					begin
						w1 = n0_1; q1 = 0; v1 = 1;
						If (n0_1 != n0_2) begin w2 = n0_2; q2 = 0; v2 = 1; end
					end
				end
			end
		end
	end
endmodule
}
