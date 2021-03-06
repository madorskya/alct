   
REM ############################################################################
REM   ____  ____ 
REM  /   /\/   / 
REM /___/  \  /    Vendor: Xilinx 
REM \   \   \/     Version : 1.11
REM  \   \         Application : Spartan-6 FPGA GTP Transceiver Wizard
REM  /   /         Filename : simulate_ncsim.bat
REM /___/   /\  
REM \   \  /  \ 
REM  \___\/\___\ 
REM
REM
REM Script SIMULATE_NCSIM.BAT
REM Generated by Xilinx Spartan-6 FPGA GTP Transceiver Wizard
REM
REM *************************** Beginning of Script ***************************

                
REM Ensure the follwoing
REM The library paths for UNISIMS_VER, SIMPRIMS_VER, XILINXCORELIB_VER,
REM UNISIM, SIMPRIM, XILINXCORELIB are set correctly in the cds.lib and hdl.var files.
REM Variables LMC_HOME and XILINX are set 
REM Define the mapping for the work library in cds.lib file. DEFINE work ./work

mkdir work
REM MGT Wrapper
ncvlog -work work   ../../../s6_gtpwizard_v1_11_tile.v;
ncvlog -work work   ../../../s6_gtpwizard_v1_11.v;

 
REM Clock Modules 
ncvlog -work work  ../../example_design/mgt_usrclk_source.v;

REM Example Design modules
ncvlog -work work   ../../example_design/frame_gen.v;
ncvlog -work work   ../../example_design/frame_check.v;
ncvlog -work work   ../../example_design/s6_gtpwizard_v1_11_top.v;
ncvlog -work work   ../demo_tb.v;

REM Other modules
ncvlog -work work $XILINX/verilog/src/glbl.v;

REM Elaborate Design
ncelab -TIMESCALE 1ns/1ps -ACCESS +rw work.DEMO_TB work.glbl

REM Run simulation
ncsim work.DEMO_TB -input @"simvision -input wave_ncsim.sv"

