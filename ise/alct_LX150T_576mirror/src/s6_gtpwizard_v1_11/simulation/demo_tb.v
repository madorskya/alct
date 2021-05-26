////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version : 1.11
//  \   \         Application : Spartan-6 FPGA GTP Transceiver Wizard
//  /   /         Filename : demo_tb.v
// /___/   /\       
// \   \  /  \ 
//  \___\/\___\
//
//
// Module DEMO_TB
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

module DEMO_TB;

//*************************Parameter Declarations******************************

    parameter   GTP0_REFCLK_PERIOD   =   12.52;
    parameter   GTP1_REFCLK_PERIOD   =   12.52;
  
//************************Internal Register Declarations***********************

//************************** Register Declarations ****************************        

    reg             gtp0_refclk_n_r;
    reg             gtp1_refclk_n_r;
    reg             drp_clk_r;
    reg             gsr_r;
    reg             gts_r;
    reg             reset_i;
    reg             track_data_high_r;
    reg             track_data_low_r;

//********************************Wire Declarations**********************************

    //--------------------------------- Global Signals ------------------------------
    wire            gtp0_refclk_p_r;
    wire            gtp1_refclk_p_r;
    
    //-------------------------- Example Module Connections -------------------------
    wire            track_data_i;
    wire    [1:0]   rxn_in_i;
    wire    [1:0]   rxp_in_i;
    wire    [1:0]   txn_out_i;
    wire    [1:0]   txp_out_i;
    
//*********************************Main Body of Code**********************************


    // ------------------------------- Tie offs -------------------------------- 
    
    assign   tied_to_ground_i     =    1'b0;
    
    // ------------------------- MGT Serial Connections ------------------------

    assign   rxn_in_i           =  txn_out_i;
    assign   rxp_in_i           =  txp_out_i;  

    //------------------------------ Global Signals ----------------------------
    
    //Simultate the global reset that occurs after configuration at the beginning
    //of the simulation. 
    assign glbl.GSR = gsr_r;
    assign glbl.GTS = gts_r;

    initial
        begin
            gts_r = 1'b0;        
            gsr_r = 1'b1;
            #(16*GTP0_REFCLK_PERIOD);
            gsr_r = 1'b0;
    end


    //----------------------- Generate Reference Clock -----------------------
    
    initial begin
        gtp0_refclk_n_r = 1'b1;
    end

    always
        #(GTP0_REFCLK_PERIOD/2) gtp0_refclk_n_r = !gtp0_refclk_n_r;

    assign gtp0_refclk_p_r = !gtp0_refclk_n_r;

    initial begin
        gtp1_refclk_n_r = 1'b1;
    end

    always
        #(GTP1_REFCLK_PERIOD/2) gtp1_refclk_n_r = !gtp1_refclk_n_r;

    assign gtp1_refclk_p_r = !gtp1_refclk_n_r;
                 
      
    
    //------------------------------- Resets -----------------------------------
    
    initial
    begin
        reset_i = 1'b1;
        #100 reset_i = 1'b0;
    end

    //----------------------------- Track Data ---------------------------------
    
    initial
    begin
        track_data_high_r = 1'b0;
        #47000;
        if (track_data_i == 1) begin
            track_data_high_r = 1'b1;
        end
        #2000;
        if ((track_data_high_r == 1'b1) && (track_data_low_r == 1'b0))
        begin
            $display("------- TEST PASSED -------");
        end
        else
        begin
            $display("####### ERROR: TEST FAILED ! #######");
        end

        $stop;
    end

    initial
    begin
        track_data_low_r = 1'b0;
        #47000;
        @(negedge track_data_i) begin
            track_data_low_r = 1'b1;
        end
    end

    //----------------- Instantiate an s6_gtpwizard_v1_11_top module  -----------------

    s6_gtpwizard_v1_11_top #
    (
        .EXAMPLE_SIMULATION             (1),        // Set to 1 for simulation
        .EXAMPLE_SIM_GTPRESET_SPEEDUP   (1),        // Speedup is turned on for simulation
        .EXAMPLE_USE_CHIPSCOPE          (0)         //1 - use chipscope to drive resets,
                                                    //0 - drive resets from top level ports
    )
    s6_gtpwizard_v1_11_top_i
    (
        .TILE0_GTP0_REFCLK_PAD_N_IN          (gtp0_refclk_n_r),
        .TILE0_GTP0_REFCLK_PAD_P_IN          (gtp0_refclk_p_r),
        .GTP0_RESET_IN                       (reset_i),
        .GTP1_RESET_IN                       (reset_i),
        .TILE0_GTP0_PLLLKDET_OUT             (tile0_gtp0plllkdet_i),
        .TILE0_GTP1_PLLLKDET_OUT             (tile0_gtp1plllkdet_i),
        .TRACK_DATA_OUT                      (track_data_i),
        .RXN_IN                              (rxn_in_i),
        .RXP_IN                              (rxp_in_i),
        .TXN_OUT                             (txn_out_i),
        .TXP_OUT                             (txp_out_i)
    );


endmodule

