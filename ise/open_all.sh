#!/bin/bash
export XILINXD_LICENSE_FILE=2112@licenxilinx
export IMPACT=/home/madorsky/software/xilinx/14.7/ISE_DS/ISE/bin/lin64/impact
source /home/madorsky/software/xilinx/14.7/ISE_DS/settings64.sh

dnames="alct_LX150_192 alct_LX100_288  alct_LX100_288bn  alct_LX100_288bp  alct_LX100_288fp  alct_LX100_384  alct_LX100_384mirror  alct_LX150T_576mirror  alct_LX150T_672 alct_LX150_384mirror"

#dnames="alct_LX100_288 alct_LX100_288bn  alct_LX100_288bp  alct_LX100_288fp"

#dnames="alct_LX150T_672"

for d in $dnames
do
	cd $d
	ise $d.xise &
	cd ..
done

