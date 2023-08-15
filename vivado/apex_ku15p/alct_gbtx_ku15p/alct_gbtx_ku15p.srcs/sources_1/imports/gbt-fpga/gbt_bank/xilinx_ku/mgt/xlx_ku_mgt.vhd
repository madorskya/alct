-------------------------------------------------------
--! @file
--! @author Julian Mendez <julian.mendez@cern.ch> (CERN - EP-ESE-BE)
--! @version 6.0
--! @brief GBT-FPGA IP - Device specific transceiver
-------------------------------------------------------

--! IEEE VHDL standard library:
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

--! Xilinx devices library:
library unisim;
use unisim.vcomponents.all;

--! Custom libraries and packages:
use work.gbt_bank_package.all;
use work.vendor_specific_gbt_bank_package.all;

--! @brief MGT - Transceiver
--! @details 
--! The MGT module provides the interface to the transceivers to send the GBT-links via
--! high speed links (@4.8Gbps)
entity mgt is
   generic (
      NUM_LINKS                                      : integer := 1
   );                                                
   port (
       --=============--
       -- Clocks      --
       --=============--
       MGT_REFCLK_i                 : in  std_logic;
       
       MGT_RXUSRCLK_o               : out std_logic_vector(1 to NUM_LINKS);
       MGT_TXUSRCLK_o               : out std_logic_vector(1 to NUM_LINKS);
       
       --=============--
       -- Resets      --
       --=============--
       MGT_TXRESET_i                : in  std_logic_vector(1 to NUM_LINKS);
       MGT_RXRESET_i                : in  std_logic_vector(1 to NUM_LINKS);
       
       --=============--
       -- Status      --
       --=============--
       MGT_TXREADY_o                : out std_logic_vector(1 to NUM_LINKS);
       MGT_RXREADY_o                : out std_logic_vector(1 to NUM_LINKS);

       RX_HEADERLOCKED_o            : out std_logic_vector(1 to NUM_LINKS);
       RX_HEADERFLAG_o              : out std_logic_vector(1 to NUM_LINKS);
         MGT_RSTCNT_o                 : out gbt_reg8_A(1 to NUM_LINKS);
       
         --==============--
         -- Control      --
         --==============--
         MGT_AUTORSTEn_i              : in  std_logic_vector(1 to NUM_LINKS);
       MGT_AUTORSTONEVEN_i          : in  std_logic_vector(1 to NUM_LINKS);
         
       --==============--
       -- Data         --
       --==============--
       MGT_USRWORD_i                : in  word_mxnbit_A(1 to NUM_LINKS);
       MGT_USRWORD_o                : out word_mxnbit_A(1 to NUM_LINKS);
       
       --=============================--
       -- Device specific connections --
       --=============================--
       MGT_DEVSPEC_i                : in  mgtDeviceSpecific_i_R;
       MGT_DEVSPEC_o                : out mgtDeviceSpecific_o_R
   
   );
end mgt;

--! @brief MGT - Transceiver
--! @details The MGT module implements all the logic required to send the GBT frame on high speed
--! links: resets modules for the transceiver, Tx PLL and alignement logic to align the received word with the 
--! GBT frame header.
architecture structural of mgt is
   --================================ Signal Declarations ================================--

   --==============================--
   -- RX phase alignment (bitslip) --
   --==============================--
   
   
       signal rx_wordclk_sig                         : std_logic_vector(1 to NUM_LINKS);
       signal tx_wordclk_sig                         : std_logic_vector(1 to NUM_LINKS);
       
       signal rxoutclk_sig                           : std_logic_vector(1 to NUM_LINKS);
       signal txoutclk_sig                           : std_logic_vector(1 to NUM_LINKS);
       
       signal rx_reset_done                          : std_logic_vector(1 to NUM_LINKS);
       signal tx_reset_done                          : std_logic_vector(1 to NUM_LINKS);
       
       signal rxResetDone_r3                         : std_logic_vector         (1 to NUM_LINKS);
       signal txResetDone_r2                         : std_logic_vector         (1 to NUM_LINKS);
       signal rxResetDone_r2                         : std_logic_vector         (1 to NUM_LINKS);
       signal txResetDone_r                          : std_logic_vector         (1 to NUM_LINKS);   
       signal rxResetDone_r                          : std_logic_vector         (1 to NUM_LINKS);    
       
       signal rxfsm_reset_done                       : std_logic_vector(1 to NUM_LINKS);
       signal txfsm_reset_done                       : std_logic_vector(1 to NUM_LINKS);
       
       signal txuserclkRdy                           : std_logic_vector(1 to NUM_LINKS);
       signal rxuserclkRdy                           : std_logic_vector(1 to NUM_LINKS);
       
       signal gtwiz_buffbypass_tx_reset_in_s         : std_logic_vector(1 to NUM_LINKS);
       signal gtwiz_buffbypass_rx_reset_in_s         : std_logic_vector(1 to NUM_LINKS);
       
       signal rxpmaresetdone                         : std_logic_vector(1 to NUM_LINKS);
       signal txpmaresetdone                         : std_logic_vector(1 to NUM_LINKS);
          
       signal run_to_rxBitSlipControl                : std_logic_vector         (1 to NUM_LINKS);
       signal rxBitSlip_from_rxBitSlipControl        : std_logic_vector         (1 to NUM_LINKS);
       signal rxBitSlip_to_gtx                       : std_logic_vector         (1 to NUM_LINKS);   
       signal done_from_rxBitSlipControl             : std_logic_vector         (1 to NUM_LINKS);
                 
       type rstBitSlip_FSM_t                 is (idle, reset_tx, reset_rx);
       type rstBitSlip_FSM_t_A              is array (natural range <>) of rstBitSlip_FSM_t; 
       signal rstBitSlip_FSM                : rstBitSlip_FSM_t_A(1 to NUM_LINKS);
            
       signal mgtRst_from_bitslipCtrl       : std_logic_vector(1 to NUM_LINKS);
       signal rx_reset_sig                                  : std_logic_vector(1 to NUM_LINKS);
       signal tx_reset_sig                                  : std_logic_vector(1 to NUM_LINKS);
       
       signal resetGtxRx_from_rxBitSlipControl            : std_logic_vector         (1 to NUM_LINKS);   
       signal resetGtxTx_from_rxBitSlipControl            : std_logic_vector         (1 to NUM_LINKS);  
       
       signal txprgdivresetdone_int      : std_logic_vector(1 to NUM_LINKS);
       signal gtwiz_userclk_tx_reset_int : std_logic_vector(1 to NUM_LINKS);
       signal gtwiz_userclk_rx_reset_int : std_logic_vector(1 to NUM_LINKS);
       signal gtwiz_userclk_tx_active_int: std_logic_vector(1 to NUM_LINKS);
       signal gtwiz_userclk_rx_active_int: std_logic_vector(1 to NUM_LINKS);
       signal rxBuffBypassRst            : std_logic_vector(1 to NUM_LINKS);
       signal resetAllMgt                : std_logic_vector(1 to NUM_LINKS);
       
       signal MGT_USRWORD_s              : gbt_reg40_A(1 to NUM_LINKS);
       signal bitSlipCmd_to_bitSlipCtrller : std_logic_vector(1 to NUM_LINKS);
       signal ready_from_bitSlipCtrller    : std_logic_vector(1 to NUM_LINKS);
 
       signal rx_headerlocked_s            : std_logic_vector(1 to NUM_LINKS);
       signal rx_bitslipIsEven_s           : std_logic_vector(1 to NUM_LINKS);
       
       COMPONENT xlx_ku_mgt_ip
         PORT (    
           gtwiz_userclk_tx_active_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_userclk_rx_active_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_tx_reset_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_tx_start_user_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_tx_done_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_tx_error_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_rx_reset_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_rx_start_user_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_rx_done_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_buffbypass_rx_error_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_clk_freerun_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_all_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_tx_pll_and_datapath_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_tx_datapath_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_rx_pll_and_datapath_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_rx_datapath_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_rx_cdr_stable_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_tx_done_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_reset_rx_done_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtwiz_userdata_tx_in : IN STD_LOGIC_VECTOR(39 DOWNTO 0);
           gtwiz_userdata_rx_out : OUT STD_LOGIC_VECTOR(39 DOWNTO 0);
           drpaddr_in : IN STD_LOGIC_VECTOR(9 DOWNTO 0);
           drpclk_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           drpdi_in : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
           drpen_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           drpwe_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gthrxn_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gthrxp_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           gtrefclk0_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           loopback_in : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
           rxpolarity_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           rxslide_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           rxusrclk_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           rxusrclk2_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           txdiffctrl_in : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
           txpolarity_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           txpostcursor_in : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
           txprecursor_in : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
           txusrclk_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           txusrclk2_in : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
           cplllock_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           drpdo_out : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
           drprdy_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gthtxn_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           gthtxp_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           rxoutclk_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           rxpmaresetdone_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           txoutclk_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
           txpmaresetdone_out : OUT STD_LOGIC_VECTOR(0 DOWNTO 0)
         );
       END COMPONENT;
   
--=================================================================================================--
begin                 --========####   Architecture Body   ####========-- 
--=================================================================================================--
   
   --==================================== User Logic =====================================--
   gtxLatOpt_gen: for i in 1 to NUM_LINKS generate
             
      --=============--
      -- Assignments --
      --=============--              
         MGT_TXREADY_o(i)          <= tx_reset_done(i) and txfsm_reset_done(i);
         MGT_RXREADY_o(i)          <= rx_reset_done(i) and rxfsm_reset_done(i) and done_from_rxBitSlipControl(i);
           
         MGT_RXUSRCLK_o(i)         <= rx_wordclk_sig(i);   
         MGT_TXUSRCLK_o(i)         <= tx_wordclk_sig(i);
          
         MGT_USRWORD_o(i)    <= MGT_USRWORD_s(i);
      
         rx_reset_sig(i)                  <= MGT_RXRESET_i(i) or resetGtxRx_from_rxBitSlipControl(i);
         tx_reset_sig(i)                  <= MGT_TXRESET_i(i) or resetGtxTx_from_rxBitSlipControl(i);      
          
--          txResetDoneSync: entity work.xlx_ku_mgt_ip_reset_synchronizer
--              PORT MAP(
--                clk_in                                   => tx_wordclk_sig(i),
--                rst_in                                   => tx_reset_done(i),
--                rst_out                                  => txResetDone_r2(i)
--              ); 
              
--          rxResetSync: entity work.xlx_ku_mgt_ip_reset_synchronizer
--            PORT MAP(
--              clk_in                                   => rx_wordClk_sig(i),
--              rst_in                                   => rx_reset_done(i),
--              rst_out                                  => rxResetDone_r3(i)
--            ); 
       
          rxBuffBypassRst(i) <= not(gtwiz_userclk_rx_active_int(i)) or not(txfsm_reset_done(i));
          
          resetDoneSynch_rx: entity work.xlx_ku_mgt_ip_reset_synchronizer
             PORT MAP(
               clk_in                                   => rx_wordClk_sig(i),
               rst_in                                   => rxBuffBypassRst(i),
               rst_out                                  => gtwiz_buffbypass_rx_reset_in_s(i)
             );
             
          
          resetSynch_tx: entity work.xlx_ku_mgt_ip_reset_synchronizer
             PORT MAP(
               clk_in                                   => tx_wordclk_sig(i),
               rst_in                                   => not(gtwiz_userclk_tx_active_int(i)),
               rst_out                                  => gtwiz_buffbypass_tx_reset_in_s(i)
             );
             
          
          gtwiz_userclk_tx_reset_int(i) <= not(txpmaresetdone(i));
          gtwiz_userclk_rx_reset_int(i) <= not(rxpmaresetdone(i));
          
          rxWordClkBuf_inst: bufg_gt
              port map (
                 O                                        => rx_wordclk_sig(i), 
                 I                                        => rxoutclk_sig(i),
                 CE                                       => not(gtwiz_userclk_rx_reset_int(i)),
                 DIV                                      => "000",
                 CLR                                      => gtwiz_userclk_rx_reset_int(i), -- Maq
                 CLRMASK                                  => '0',
                 CEMASK                                   => '0'
              ); 
                            
            txWordClkBuf_inst: bufg_gt
              port map (
                 O                                        => tx_wordclk_sig(i), 
                 I                                        => txoutclk_sig(i),
                 CE                                       => not(gtwiz_userclk_tx_reset_int(i)),
                 DIV                                      => "000",
                 CLR                                      => gtwiz_userclk_tx_reset_int(i), --Maq
                 CLRMASK                                  => '0',
                 CEMASK                                   => '0'
              ); 
          
          activetxUsrClk_proc: process(gtwiz_userclk_tx_reset_int(i), tx_wordclk_sig(i))
          begin
            if gtwiz_userclk_tx_reset_int(i) = '1' then
                gtwiz_userclk_tx_active_int(i) <= '0';
            elsif rising_edge(tx_wordclk_sig(i)) then
                gtwiz_userclk_tx_active_int(i) <= '1';
            end if;
            
          end process;
          
                    
          activerxUsrClk_proc: process(gtwiz_userclk_rx_reset_int(i), rx_wordclk_sig(i))
          begin
            if gtwiz_userclk_rx_reset_int(i) = '1' then
                gtwiz_userclk_rx_active_int(i) <= '0';
            elsif rising_edge(rx_wordclk_sig(i)) then
                gtwiz_userclk_rx_active_int(i) <= '1';
            end if;
          
          end process;
                    
          xlx_ku_mgt_std_i: xlx_ku_mgt_ip
             PORT MAP (          
                 rxusrclk_in(0) => rx_wordclk_sig(i),
                 rxusrclk2_in(0) => rx_wordclk_sig(i),
                 txusrclk_in(0) => tx_wordclk_sig(i),
                 txusrclk2_in(0) => tx_wordclk_sig(i),
                 rxoutclk_out(0) => rxoutclk_sig(i),
                 txoutclk_out(0) => txoutclk_sig(i),
                 
                 gtwiz_userclk_tx_active_in(0)          => gtwiz_userclk_tx_active_int(i),
                 gtwiz_userclk_rx_active_in(0)          => gtwiz_userclk_rx_active_int(i),                 
                 
                 gtwiz_buffbypass_tx_reset_in(0)        => gtwiz_buffbypass_tx_reset_in_s(i),
                 gtwiz_buffbypass_tx_start_user_in(0)   => '0',
                 gtwiz_buffbypass_tx_done_out(0)        => txfsm_reset_done(i),
                 gtwiz_buffbypass_tx_error_out          => open,
                 
                 gtwiz_buffbypass_rx_reset_in(0)        => gtwiz_buffbypass_rx_reset_in_s(i),
                 gtwiz_buffbypass_rx_start_user_in(0)   => '0',
                 gtwiz_buffbypass_rx_done_out(0)        => rxfsm_reset_done(i),
                 gtwiz_buffbypass_rx_error_out          => open,
                 
                 gtwiz_reset_clk_freerun_in(0)          => MGT_DEVSPEC_i.reset_freeRunningClock(i),
                 
                 gtwiz_reset_all_in(0)                  => '0',
                 
                 gtwiz_reset_tx_pll_and_datapath_in(0)  => tx_reset_sig(i),
                 gtwiz_reset_tx_datapath_in(0)          => '0',
                 
                 gtwiz_reset_rx_pll_and_datapath_in(0)  => '0', -- Same PLL is used for TX and RX !
                 gtwiz_reset_rx_datapath_in(0)          => rx_reset_sig(i),
                 gtwiz_reset_rx_cdr_stable_out          => open,
                 
                 gtwiz_reset_tx_done_out(0)             => tx_reset_done(i),
                 gtwiz_reset_rx_done_out(0)             => rx_reset_done(i),
                 
                 gtwiz_userdata_tx_in                   => MGT_USRWORD_i(i),
                 gtwiz_userdata_rx_out                  => MGT_USRWORD_s(i),
                 
                 drpaddr_in                             => b"0" & MGT_DEVSPEC_i.drp_addr(i),--Maq
                 drpclk_in(0)                           => MGT_DEVSPEC_i.drp_clk(i),
                 drpdi_in                               => MGT_DEVSPEC_i.drp_di(i),
                 drpen_in(0)                            => MGT_DEVSPEC_i.drp_en(i),
                 drpwe_in(0)                            => MGT_DEVSPEC_i.drp_we(i),
                 drpdo_out                              => MGT_DEVSPEC_o.drp_do(i),
                 drprdy_out(0)                          => MGT_DEVSPEC_o.drp_rdy(i),
                 
                 gthrxn_in(0)                           => MGT_DEVSPEC_i.rx_n(i),
                 gthrxp_in(0)                           => MGT_DEVSPEC_i.rx_p(i),
                 gthtxn_out(0)                          => MGT_DEVSPEC_o.tx_n(i),
                 gthtxp_out(0)                          => MGT_DEVSPEC_o.tx_p(i),
                 
                 gtrefclk0_in(0)                        => MGT_REFCLK_i,
                 
                 loopback_in                            => MGT_DEVSPEC_i.loopBack(i),        
                 rxpolarity_in(0)                       => MGT_DEVSPEC_i.conf_rxPol(i),
                 txpolarity_in(0)                       => MGT_DEVSPEC_i.conf_txPol(i),
                 
                 rxslide_in(0)                          => rxBitSlip_to_gtx(i),
                 
                 txdiffctrl_in                          => b"0" & MGT_DEVSPEC_i.conf_diffCtrl(i),--Maq
                 txpostcursor_in                        => MGT_DEVSPEC_i.conf_postCursor(i),
                 txprecursor_in                         => MGT_DEVSPEC_i.conf_preCursor(i),
                 
                 cplllock_out                           => open,
                 
                 rxpmaresetdone_out(0)                  => rxpmaresetdone(i),
                 txpmaresetdone_out(0)                  => txpmaresetdone(i)
             );
             
          --====================--
          -- RX phase alignment --
          --====================--                   
            -- Reset on bitslip control module:
            -----------------------------------
               bitslipResetFSM_proc: PROCESS(MGT_DEVSPEC_i.reset_freeRunningClock(i), MGT_RXRESET_i(i))
                 variable timer :integer range 0 to GBTRX_BITSLIP_MGT_RX_RESET_DELAY;  
              variable rstcnt: unsigned(7 downto 0);              
               begin
         
                   if MGT_RXRESET_i(i) = '1' then
                        resetGtxRx_from_rxBitSlipControl(i) <= '0';
                        resetGtxTx_from_rxBitSlipControl(i) <= '0';
                        timer  := 0;
                        rstcnt := (others => '0');
                        rstBitSlip_FSM(i) <= idle;
                     
                   elsif rising_edge(MGT_DEVSPEC_i.reset_freeRunningClock(i)) then
                                     
                        case rstBitSlip_FSM(i) is
                           when idle      =>       resetGtxRx_from_rxBitSlipControl(i)     <= '0';
                                                   resetGtxTx_from_rxBitSlipControl(i)     <= '0';
                                                
                                                   if mgtRst_from_bitslipCtrl(i) = '1' then
                                                    
                                                       resetGtxRx_from_rxBitSlipControl(i) <= '1';
                                                       resetGtxTx_from_rxBitSlipControl(i) <= '1';
                                                       rstBitSlip_FSM(i) <= reset_tx;
                                                       timer := 0;
                                                       
                                                       rstcnt := rstcnt+1;
                                                    
                                                   end if;
                                                             
                           when reset_tx  => if timer = GBTRX_BITSLIP_MGT_RX_RESET_DELAY-1 then
                                                        resetGtxTx_from_rxBitSlipControl(i)     <= '0';
                                                        rstBitSlip_FSM(i)                       <= reset_rx;
                                                        timer                                   := 0;
                                                   else
                                                        timer := timer + 1;
                                                   end if;
                                                
                           when reset_rx  => if timer = GBTRX_BITSLIP_MGT_RX_RESET_DELAY-1 then
                                                         resetGtxRx_from_rxBitSlipControl(i)     <= '0';
                                                         rstBitSlip_FSM(i)                       <= idle;
                                                         timer                                   := 0;
                                                    else
                                                         timer := timer + 1;
                                                    end if;
                                                
                        end case;
                     
                       MGT_RSTCNT_o(i)   <= std_logic_vector(rstcnt);
                   end if;
                
               end process; 
               
            rxBitSlipControl: entity work.mgt_bitslipctrl 
               port map (
                       RX_RESET_I          => not(rx_reset_done(i) and rxfsm_reset_done(i)),
                       RX_WORDCLK_I        => rx_wordclk_sig(i),
                       MGT_CLK_I           => MGT_DEVSPEC_i.reset_freeRunningClock(i),
                                      
                       RX_BITSLIPCMD_i     => bitSlipCmd_to_bitSlipCtrller(i),
                       RX_BITSLIPCMD_o     => rxBitSlip_to_gtx(i),
                       
                       RX_HEADERLOCKED_i   => rx_headerlocked_s(i),
                       RX_BITSLIPISEVEN_i  => rx_bitslipIsEven_s(i),
                       RX_RSTONBITSLIP_o   => mgtRst_from_bitslipCtrl(i),
                       RX_ENRST_i          => MGT_AUTORSTEn_i(i),
                       RX_RSTONEVEN_i      => MGT_AUTORSTONEVEN_i(i),
                       
                       DONE_o              => done_from_rxBitSlipControl(i),
                       READY_o             => ready_from_bitSlipCtrller(i)
               );
                   
               patternSearch: entity work.mgt_framealigner_pattsearch
                   port map (
                       RX_RESET_I                          => not(rx_reset_done(i) and rxfsm_reset_done(i)),
                       RX_WORDCLK_I                        => rx_wordclk_sig(i),
                       
                       RX_BITSLIP_CMD_O                    => bitSlipCmd_to_bitSlipCtrller(i),
                       MGT_BITSLIPDONE_i                   => ready_from_bitSlipCtrller(i),
                             
                       RX_HEADER_LOCKED_O                  => rx_headerlocked_s(i),
                       RX_HEADER_FLAG_O                    => RX_HEADERFLAG_o(i),
                       RX_BITSLIPISEVEN_o                  => rx_bitslipIsEven_s(i), 
                       
                       RX_WORD_I                           => MGT_USRWORD_s(i)
                   );
                   
                   RX_HEADERLOCKED_o(i) <= rx_headerlocked_s(i);
                   
  end generate;     
end structural;
--=================================================================================================--
--#################################################################################################--
--=================================================================================================--