
#include "promoter.h"


void Promoter::operator()
(
	Signal bestc, 
	Signal bestcn, 
	Signal besta, 
	Signal bestan, 
	Signal PromoteColl, 
	Signal best, 
	Signal bestn, 
	Signal fa		
)
{
initio
	bestc.input(QBITS-1, 0, "bestc");
	bestcn.input(6, 0, "bestcn");
	besta.input(QBITS-1, 0, "besta");
	bestan.input(6, 0, "bestan");
	PromoteColl.input("PromoteColl");
	best.output(QBITS-1, 0, "best", makereg);
	bestn.output(6, 0, "bestn", makereg);
	fa.output("fa", makereg);

beginmodule
	mode.reg(2, 0, "mode");
    always (bestc or bestcn or besta or bestan or PromoteColl) 
    begin
		mode(0) = ifelse((besta == 0), "1'b0", "1'b1");
		mode(1) = ifelse((bestc == 0), "1'b0", "1'b1");
		mode(2) = PromoteColl;
    	begincase (mode)
    		case1("3'b000") begin best = 0;     bestn = 0;      fa = 0; end
    		case1("3'b001") begin best = besta; bestn = bestan; fa = 1; end
    		case1("3'b010") begin best = bestc; bestn = bestcn; fa = 0; end
    		case1("3'b011") begin best = besta; bestn = bestan; fa = 1; end
    		case1("3'b100") begin best = 0;     bestn = 0;      fa = 0; end
    		case1("3'b101") begin best = besta; bestn = bestan; fa = 1; end
    		case1("3'b110") begin best = bestc; bestn = bestcn; fa = 0; end
    		case1("3'b111") begin best = bestc; bestn = bestcn; fa = 0; end
    	endcase
    end

endmodule
}

