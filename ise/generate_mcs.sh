#!/bin/bash
export IMPACT=/home/madorsky/software/xilinx/14.7/ISE_DS/ISE/bin/lin64/impact
source /home/madorsky/software/xilinx/14.7/ISE_DS/settings64.sh

dnames="alct_LX150_192 alct_LX100_288  alct_LX100_288bn  alct_LX100_288bp  alct_LX100_288fp  alct_LX100_384  alct_LX100_384mirror  alct_LX150T_576mirror  alct_LX150T_672"

#dnames="alct_LX100_288  alct_LX100_288bn  alct_LX100_288bp  alct_LX100_288fp"

#dnames="alct_LX150T_672"

for d in $dnames
do
	cd $d
	impact -batch $d.mcs.impact
	cd ..
done

