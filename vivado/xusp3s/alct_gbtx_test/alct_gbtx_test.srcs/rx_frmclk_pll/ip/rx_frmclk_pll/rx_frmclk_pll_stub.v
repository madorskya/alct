// Copyright 1986-2023 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.2.2 (win64) Build 3788238 Tue Feb 21 20:00:34 MST 2023
// Date        : Sat Aug 12 00:27:46 2023
// Host        : uf-eng-srv-1 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/github/alct/vivado/xusp3s/alct_gbtx_test/alct_gbtx_test.srcs/rx_frmclk_pll/ip/rx_frmclk_pll/rx_frmclk_pll_stub.v
// Design      : rx_frmclk_pll
// Purpose     : Stub declaration of top-level module interface
// Device      : xcvu080-ffvb2104-2-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module rx_frmclk_pll(clkfb_in, clk_out1, clkfb_out, reset, locked, 
  clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clkfb_in,clk_out1,clkfb_out,reset,locked,clk_in1" */;
  input clkfb_in;
  output clk_out1;
  output clkfb_out;
  input reset;
  output locked;
  input clk_in1;
endmodule
