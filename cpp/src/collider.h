#include "vlib.h"
#include "vmac.h"
#include "alctver.h"

class collider: public module
{
 public:
	void operator()
	(
		Signal qp0, Signal qp1, Signal qp2, Signal qp3, 
		Signal w1, Signal w2, 
		Signal q1, Signal q2, 
		Signal v1, Signal v2, 
		Signal clk
	);

	Signal n3_1, n3_2, n2_1, n2_2, n1_1, n1_2, n0_1, n0_2;

};
