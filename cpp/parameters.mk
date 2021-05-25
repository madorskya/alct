# ALCT parameters.
# Change any parameter and 'make' in this directory

#version code - used only in DAQ06 format
export FW_VERSION =-DFWVER="\"6\'h5\""

# uncomment to generate Verilog HDL code
export	VERGEN =-DVGEN
# uncomment to simulate Verilog HDL code
#export	VERGEN =

# full debugging info, no optimization
export	OPTIM =-ggdb -D_VDEBUG

# speed optimization
#export	OPTIM =-O3

# Ghost cancellation logic fine tuning 
# Uncomment to allow cancellation of the tracks with equal quality
# Lower WG numbers have priority
export	KILLEQUAL = -DKILLEQUAL
# uncomment to prevent cancelling equal-quality tracks
#export KILLEQUAL =

#uncomment to reduce latency to 6 clocks
export	RED_LAT = -DRED_LAT
#uncomment to use normal latency 10 clocks
#export RED_LAT =

#uncomment for DAQ-01 format
#export	DAQFORM = -DDAQ01
#uncomment for DAQ-06 format
export	DAQFORM = -DDAQ06

#uncomment for trigger on entire chamber as one pattern
#export	TRIGWP =1

export CC         = gcc
export CL         = g++

