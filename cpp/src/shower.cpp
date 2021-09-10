#include "shower.h"
#define SHOWER_PIPE_N 7
#define SHOWER_IN_TIME 6,4
#define SHOWER_OUT_TIME 3,0

void shower::operator ()
(
     Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
     Signal th_loose,
     Signal th_nominal,
     Signal th_tight,
     Signal shower_int,
     Signal shower_oot,
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

  OutReg_(shower_int, 1,0);
  OutReg_(shower_oot, 1,0);
  
  Input  (clk);
beginmodule

  Reg_(i, 6,0);
  Reg_(count, 9, 0); // to accommodate max or 672 
  Reg__(slc, 9,0, SHOWER_PIPE_N,0); // 8 pipeline counters
  Reg_(loose,   SHOWER_PIPE_N,0);
  Reg_(nominal, SHOWER_PIPE_N,0);
  Reg_(tight,   SHOWER_PIPE_N,0);
  
  always (posedge (clk))
  begin
      
      tight = 0;
      nominal = 0;
      loose = 0;
      For (i = 0, i < SHOWER_PIPE_N, i++)
      begin
 	  If      (slc[i] >= th_tight)   tight  (i) = 1;
          Else If (slc[i] >= th_nominal) nominal(i) = 1;
          Else If (slc[i] >= th_loose)   loose  (i) = 1;
      end

      shower_int = 0;
      shower_oot = 0;
      If      (tight  (SHOWER_IN_TIME) != 0) shower_int = 3;
      Else If (nominal(SHOWER_IN_TIME) != 0) shower_int = 2;
      Else If (loose  (SHOWER_IN_TIME) != 0) shower_int = 1;

      If      (tight  (SHOWER_OUT_TIME) != 0) shower_oot = 3;
      Else If (nominal(SHOWER_OUT_TIME) != 0) shower_oot = 2;
      Else If (loose  (SHOWER_OUT_TIME) != 0) shower_oot = 1;
      
      // push the pipeline 
      For (i = 0, i < SHOWER_PIPE_N, i++) slc[i] = slc[i+1];
      slc[7] = count;
      // count hits in current BX
      count = 0;
      For (i = 0, i < LYWG, i++) count = count + ly0(i);
      For (i = 0, i < LYWG, i++) count = count + ly1(i);
      For (i = 0, i < LYWG, i++) count = count + ly2(i);
      For (i = 0, i < LYWG, i++) count = count + ly3(i);
      For (i = 0, i < LYWG, i++) count = count + ly4(i);
      For (i = 0, i < LYWG, i++) count = count + ly5(i);
  end
  
endmodule
}
