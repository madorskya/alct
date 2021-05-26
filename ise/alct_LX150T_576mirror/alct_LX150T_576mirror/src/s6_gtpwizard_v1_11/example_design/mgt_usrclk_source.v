///////////////////////////////////////////////////////////////////////////////
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /    Vendor: Xilinx 
// \   \   \/     Version : 1.11
//  \   \         Application : Spartan-6 FPGA GTP Transceiver Wizard
//  /   /         Filename : mgt_usrclk_source.v
// /___/   /\       
// \   \  /  \ 
//  \___\/\___\ 
//
//
// Module MGT_USRCLK_SOURCE (for use with GTP Transceivers)
// Generated by Xilinx Spartan-6 FPGA GTP Transceiver Wizard
// 
// 
// (c) Copyright 2009 - 2011 Xilinx, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES. 


`timescale 1ns / 1ps

//***********************************Entity Declaration*******************************
module MGT_USRCLK_SOURCE #
(
    parameter FREQUENCY_MODE   = "LOW",
    parameter DIVIDE_2         = "FALSE",
    parameter FEEDBACK         = "1X", 
    parameter DIVIDE           = 2.0
)
(
    output wire         DIV1_OUT,
    output wire         DIV2_OUT,
    output wire         CLK2X_OUT,
    output wire         DCM_LOCKED_OUT,
    input  wire         CLK_IN,
    input  wire         FB_IN,
    input  wire         DCM_RESET_IN
);


`define DLY #1

//*********************************Wire Declarations**********************************

    wire    [31:0]  not_connected_i;
    wire            clkdv_i;
    wire            clk0_i;
    wire            clk2x_i;

//*********************************** Beginning of Code *******************************


    // Instantiate a DCM module to divide the reference clock.
    DCM_SP #
    (
        .CLKDV_DIVIDE               (DIVIDE),
        .DFS_FREQUENCY_MODE         ("LOW"),
        .CLKIN_DIVIDE_BY_2          (DIVIDE_2),
        .CLK_FEEDBACK               (FEEDBACK),   
        .DLL_FREQUENCY_MODE         (FREQUENCY_MODE)
    )
    clock_divider_i
    (
        .CLK0                       (clk0_i),
        .CLK180                     (not_connected_i[0]),
        .CLK270                     (not_connected_i[1]),
        .CLK2X                      (clk2x_i),
        .CLK2X180                   (not_connected_i[3]),
        .CLK90                      (not_connected_i[4]),
        .CLKDV                      (clkdv_i),
        .CLKFX                      (not_connected_i[5]),
        .CLKFX180                   (not_connected_i[6]),
        .PSDONE                     (not_connected_i[7]),
        .STATUS                     (not_connected_i[15:8]),
        .DSSEN                      (not_connected_i[16]),
        .PSCLK                      (not_connected_i[17]),
        .PSEN                       (not_connected_i[18]),
        .PSINCDEC                   (not_connected_i[19]),
        .LOCKED                     (DCM_LOCKED_OUT),
        .CLKFB                      (FB_IN),
        .CLKIN                      (CLK_IN),
        .RST                        (DCM_RESET_IN)
    );

    
    BUFG dcm_1x_bufg_i
    (
        .I                          (clk0_i),
        .O                          (DIV1_OUT)
    );

    BUFG dcm_2x_bufg_i
    (
        .I                          (clk2x_i),
        .O                          (CLK2X_OUT)
    );


    BUFG dcm_div2_bufg_i
    (
        .I                          (clkdv_i),
        .O                          (DIV2_OUT)
    );


endmodule

