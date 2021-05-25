#!/bin/bash

echo "ALCT generated code copier"

# lists of versions and their parameters
type=(ALCT192 ALCT288 ALCT288 ALCT288 ALCT288 ALCT384 ALCT384 ALCT576 ALCT672 ALCT672 ALCT288 ALCT288 ALCT288 ALCT288 ALCT384 ALCT384)
rval=(8       9       9       9       9       12      12      18      21      21      9       9       9       9       12      12)
me11=(NO      ME11BN  ME11BP  ME11FP  NO      NO      NO      NO      NO      NO      ME11BN  ME11BP  ME11FP  NO      NO      NO)
mirr=(NO      NO      NO      NO      NO      MIRROR  NO      MIRROR  MIRROR  NO      NO      NO      NO      NO      MIRROR  NO)
chip=(VE      VE      VE      VE      VE      VE      VE      VE      VE      VE      S6      S6      S6      S6      S6      S6)

# take project prefix from argument 1
if [ $1 ]
then
	export PRJPFX=$1
else
	echo "Usage: copy.sh project_prefix type_1 type_2"
	exit
fi

# loop over all versions
if [ $2 ]
then
	t=$2
	if [ $3 ]
	then
		tt=$(( $3 + 1 ))
	else
		tt=$(( $2 + 1 ))
	fi
else
	t=0
	tt=10
fi

while [ $t -ne $tt ]
do

  # set parameters
  export ALCTTYPE=-D${type[$t]}
  export DRVALS=-DRVALS=${rval[$t]}
  export ME11TYPE=-D${me11[$t]}
  export MIRROR=-D${mirr[$t]}
  export CHIP=-D${chip[$t]}

  #show them
  echo $ALCTTYPE $DRVALS $ME11TYPE $MIRROR $CHIP

  #copy
  cd bin
  rm -rf verilog
  cp -rv verilog_${type[$t]}_${me11[$t]}_${mirr[$t]}_${chip[$t]} verilog
  make cp
  rm -rf verilog
  cd ..

  # next version
  t=$(( $t + 1 ))
done
