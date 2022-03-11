#include "shower.h"
#define SHOWER_PIPE_N 4

void shower::operator ()
(
     Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
     Signal th_loose,
     Signal th_nominal,
     Signal th_tight,
		 Signal drifttime,
     Signal shower_int,
     Signal clk
)
{
initio
  Input_(ly0, LYWG-1, 0);
  Input_(ly1, LYWG-1, 0);
  Input_(ly2, LYWG-1, 0);
  Input_(ly3, LYWG-1, 0);
  Input_(ly4, LYWG-1, 0);
  Input_(ly5, LYWG-1, 0);

  Input_(th_loose  , 9,0);
  Input_(th_nominal, 9,0);
  Input_(th_tight  , 9,0);

	Input_(drifttime, 2,0);

  OutReg_(shower_int, 1,0);
  
  Input  (clk);
beginmodule

  Reg_(i, 6,0);
  Reg_(count, 9, 0); // to accommodate max or 672 
	Reg_(ly_count, 2, 0);
  Reg__(slc, 12,0, SHOWER_PIPE_N-1,0); // hit counter pipeline 
  Reg (loose);
  Reg (nominal);
  Reg (tight);

	Reg_(ly_threshold, 2,0);
  
  always (posedge (clk))
  begin
      
		// logic reworked according to Sven's message from 2021-10-29
      tight = 0;
      nominal = 0;
      loose = 0;
			// check counter at the pipeline exit
			If (slc[0](12,10) >= Signal(3, 5)) // must have 5 or more layers
			begin
				If      (slc[0](9,0) >= th_tight)   tight   = 1;
        Else If (slc[0](9,0) >= th_nominal) nominal = 1;
        Else If (slc[0](9,0) >= th_loose)   loose   = 1;
			end

      shower_int = 0;
			// set output flag accordingly
      If      (tight   == 1) shower_int = 3;
      Else If (nominal == 1) shower_int = 2;
      Else If (loose   == 1) shower_int = 1;

      // push the pipeline 
      For (i = 0, i < SHOWER_PIPE_N-1, i++) 
			begin
				slc[i] = slc[i+1];
			end

			// latency depends on configured drifttime
			// should actually be drifttime + 2, but OTMB sees HMT bits being late, so setting here according to them
			// see msg from Tao 2022-03-10
 		  slc[drifttime] = (ly_count, count); 
      // count hits in current BX
      count = 0;
			ly_count = 0;
      For (i = 0, i < LYWG, i++) count = count + ly0(i);
      For (i = 0, i < LYWG, i++) count = count + ly1(i);
      For (i = 0, i < LYWG, i++) count = count + ly2(i);
      For (i = 0, i < LYWG, i++) count = count + ly3(i);
      For (i = 0, i < LYWG, i++) count = count + ly4(i);
      For (i = 0, i < LYWG, i++) count = count + ly5(i);

			// count layers
			If (ly0 != 0) ly_count++;
			If (ly1 != 0) ly_count++;
			If (ly2 != 0) ly_count++;
			If (ly3 != 0) ly_count++;
			If (ly4 != 0) ly_count++;
			If (ly5 != 0) ly_count++;

			ly_threshold = Signal(3, 5); // default layer threshold initially
			// lower layer threshold if loose threshold is set below 5
			If (th_loose <= Signal(10, 5)) ly_threshold = th_loose(2,0); 
  end
  
endmodule
}
