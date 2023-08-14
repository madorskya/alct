// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.2 (lin64) Build 3671981 Fri Oct 14 04:59:54 MDT 2022
// Date        : Mon Aug 14 16:15:37 2023
// Host        : endcap-tf2 running 64-bit Ubuntu 18.04.6 LTS
// Command     : write_verilog -force -mode synth_stub
//               /home/madorsky/github/ise/alct/vivado/apex_ku15p/alct_gbtx_ku15p/alct_gbtx_ku15p.srcs/rx_frmclk_pll/ip/rx_frmclk_pll/rx_frmclk_pll_stub.v
// Design      : rx_frmclk_pll
// Purpose     : Stub declaration of top-level module interface
// Device      : xcku15p-ffva1760-2-e
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
