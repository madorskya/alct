#include "alctver.h"
#include "vlib.h"
#include "vmac.h"

class shower: public module
{
public:
  Signal i;
  Signal count; // to accommodate max or 672 
  memory slc; // counter pipeline 
  Signal loose;
  Signal nominal;
  Signal tight;
	Signal ly_count;
	Signal ly_threshold;
  
  void operator ()
  (
     Signal ly0, Signal ly1, Signal ly2, Signal ly3, Signal ly4, Signal ly5,
     Signal th_loose,
     Signal th_nominal,
     Signal th_tight,
		 Signal drifttime,
     Signal shower_int,
     Signal clk
  );
};
