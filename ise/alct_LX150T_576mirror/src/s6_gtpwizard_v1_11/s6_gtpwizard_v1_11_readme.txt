                Core name: Xilinx LogiCORE Spartan-6 FPGA GTP Transceiver Wizard
                Version: 1.11
                Release Date: October 19, 2011


================================================================================

This document contains the following sections:

1. Introduction
2. New Features
3. Supported Devices
4. Resolved Issues
5. Known Issues
6. Technical Support
7. Other Information 
8. Core Release History
9. Legal Disclaimer

================================================================================


1. INTRODUCTION

For installation instructions for this release, please go to:

  http://www.xilinx.com/ipcenter/coregen/ip_update_install_instructions.htm

For system requirements:

   http://www.xilinx.com/ipcenter/coregen/ip_update_system_requirements.htm

This file contains release notes for the Xilinx LogiCORE IP Spartan-6 FPGA
GTP Transceiver Wizard v1.11 solution. For the latest core updates, see the product page at:

   http://www.xilinx.com/products/intellectual-property/S6_FPGA_GTP_Transceiver_Wizard.htm

2. NEW FEATURES

  - ISE 13.3 software support


3. SUPPORTED DEVICES
 
The following device families are supported by the core for this release.

Spartan-6 XC LX/LXT 
Spartan-6 XA LX/LXT
Spartan-6 XQ LX/LXT       

4. RESOLVED ISSUES

  1. Attribute Updates

   Description: The following Attributes were updated :
   - PMA_RX_CFG setting for PCIE
   - RCV_TERM_VTTRX for SRIO, XAUI, SDI protocols
   
   Version(s) Fixed:  
   CR 615296, 613303

  2. Renamed REFCLKOUT to REFCLKPLL in GUI

   Description: In page 4 of GUI, the options to choose TX/RXUSRCLK 
   source has been modified - REFCLKOUT is renamed to REFCLKPLL
   
   Version(s) Fixed:  
   CR 620887


5. KNOWN ISSUES

The most recent information, including known issues, workarounds, and
resolutions for this version is provided in the IP Release Notes Guide
located at

   www.xilinx.com/support/documentation/user_guides/xtp025.pdf


6. TECHNICAL SUPPORT

To obtain technical support, create a WebCase at www.xilinx.com/support.
Questions are routed to a team with expertise using this product.

Xilinx provides technical support for use of this product when used
according to the guidelines described in the core documentation, and
cannot guarantee timing, functionality, or support of this product for
designs that do not follow specified guidelines.


7. OTHER INFORMATION 


8. CORE RELEASE HISTORY

Date        By            Version      Description
================================================================================
10/19/2011  Xilinx. Inc.  1.11         ISE 13.3 support
06/22/2011  Xilinx, Inc.  1.10         ISE 13.2 support
03/01/2011  Xilinx, Inc.  1.9          ISE 13.1 support
12/14/2010  Xilinx, Inc.  1.8          ISE 12.4 support
09/21/2010  Xilinx, Inc.  1.7          ISE 12.3 support
07/23/2010  Xilinx, Inc.  1.6          ISE 12.2 support
04/19/2010  Xilinx, Inc.  1.5          ISE 12.1 support
12/02/2009  Xilinx, Inc.  1.4          ISE 11.4 support
09/16/2009  Xilinx, Inc.  1.3          ISE 11.3 support
================================================================================


9. LEGAL DISCLAIMER

(c) Copyright 2009 - 2011 Xilinx, Inc. All rights reserved.

This file contains confidential and proprietary information
of Xilinx, Inc. and is protected under U.S. and 
international copyright and other intellectual property
laws.

DISCLAIMER
This disclaimer is not a license and does not grant any
rights to the materials distributed herewith. Except as
otherwise provided in a valid license issued to you by
Xilinx, and to the maximum extent permitted by applicable
law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
(2) Xilinx shall not be liable (whether in contract or tort,
including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature
related to, arising under or in connection with these
materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage
(including loss of data, profits, goodwill, or any type of
loss or damage suffered as a result of any action brought
by a third party) even if such damage or loss was
reasonably foreseeable or Xilinx had been advised of the
possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-
safe, or for use in any application requiring fail-safe
performance, such as life-support or safety devices or
systems, Class III medical devices, nuclear facilities,
applications related to the deployment of airbags, or any
other applications that could lead to death, personal
injury, or severe property or environmental damage
(individually and collectively, "Critical
Applications"). Customer assumes the sole risk and
liability of any use of Xilinx products in Critical
Applications, subject only to applicable laws and
regulations governing limitations on product liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
PART OF THIS FILE AT ALL TIMES.

