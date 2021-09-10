#!/bin/bash

echo "ALCT simulation and generation code compile script"
# legacy versions removed. See compile_legacy script for legacy versions.
# lists of versions and their parameters
type=(ALCT192 ALCT288 ALCT288 ALCT288 ALCT288 ALCT384 ALCT384 ALCT576 ALCT672)
rval=(8       9       9       9       9       12      12      18      21     )
me11=(NO      ME11BN  ME11BP  ME11FP  NO      NO      NO      NO      NO     )
mirr=(NO      NO      NO      NO      NO      MIRROR  NO      MIRROR  NO     )
chip=(S6      S6      S6      S6      S6      S6      S6      S6      S6     )
flav=(LX150   LX100   LX100   LX100   LX100   LX100   LX100   LX150T  LX150T )

# loop over all versions
clean=0
# if first version is set
if [ $1 ]
then
	t=$1 # t contains first version
	if [ $2 ] # if last version is set
	then
		tt=$(( $2 + 1 )) # tt is last version + 1
		clean=1 # need to clean up between versions
	else
		tt=$(( $1 + 1 )) # if only one version is defined, compile without cleanup
	fi
else
	# if no versions, show usage
	t=0
	tt=0
	clean=0
	echo "Usage: ./compile.sh first [last(max 8) [generate]]"
	echo "first and last are ALCT versions to compile:"
	echo -e "Index\tType\tME11\tMirror\tChip\tFlavor"
	for i in 0 1 2 3 4 5 6 7 8
	do
		echo -e $i'\t'${type[$i]}'\t'${me11[$i]}'\t'${mirr[$i]}'\t'${chip[$i]}'\t'${flav[$i]}
	done;
	
	echo ""
	echo "passing first and last will make it clean up after each version, needed for Verilog generation"
	echo "passing generate parameter will make it generate Verilog code for each selected version (define VGEN)"
	echo "Examples:"
	echo "./compile.sh 6     - compile ALCT384, don't generate or clean up"
	echo "./compile.sh 6 6   - compile ALCT384, clean up before, don't generate"
	echo "./compile.sh 0 6 1 - compile versions from ALCT192 to ALCT384, clean up before each, generate verilog"
fi

# go over all requested versions first->last
while [ $t -ne $tt ]
do

  # set parameters
  export ALCTTYPE=-D${type[$t]}
  export DRVALS=-DRVALS=${rval[$t]}
  export ME11TYPE=-D${me11[$t]}
  export MIRROR=-D${mirr[$t]}
  export CHIP=-D${chip[$t]}
  export FLAVOR=-D${flav[$t]}

  #show them
  echo $ALCTTYPE $DRVALS $ME11TYPE $MIRROR $CHIP $FLAVOR $VERGEN

  # clean only if user builds multiple versions
  if [ "$clean" = "1" ] 
  then
	  make clean
  fi

  echo 'caling make from script'
  make

  # run test fixture only if user generates verilog
  if [ $3 ]
  then
	  echo 'cd bin'
	  cd bin
	  echo 'rm verilog'
	  rm -rf verilog
	  echo 'mkdir verilog'
	  mkdir -p -m755 verilog
	  echo 'make verilog'
	  make verilog
	  echo 'rm old files'
	  rm -rf     verilog_${type[$t]}_${me11[$t]}_${mirr[$t]}_${flav[$t]}
	  echo 'mv new files'
	  mv verilog verilog_${type[$t]}_${me11[$t]}_${mirr[$t]}_${flav[$t]}
	  cd ..
  fi
  # next version
  t=$(( $t + 1 ))
done
