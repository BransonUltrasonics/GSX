--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : FPGA_DUPS_Main.vhf
-- /___/   /\     Timestamp : 07/17/2021 11:41:07
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/FPGA_DUPS_Main.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/FPGA_DUPS_Main.sch"
--Design Name: FPGA_DUPS_Main
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--
----- CELL M2_1_HXILINX_FPGA_DUPS_Main -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity M2_1_HXILINX_FPGA_DUPS_Main is
  
port(
    O   : out std_logic;

    D0  : in std_logic;
    D1  : in std_logic;
    S0  : in std_logic
  );
end M2_1_HXILINX_FPGA_DUPS_Main;

architecture M2_1_HXILINX_FPGA_DUPS_Main_V of M2_1_HXILINX_FPGA_DUPS_Main is
begin
  process (D0, D1, S0)
  begin
    case S0 is
    when '0' => O <= D0;
    when '1' => O <= D1;
    when others => NULL;
    end case;
    end process; 
end M2_1_HXILINX_FPGA_DUPS_Main_V;

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity uP_SPI_IF_MUSER_FPGA_DUPS_Main is
   port ( ADC_ERROR             : in    std_logic; 
          CLK_M                 : in    std_logic; 
          CLK_1ms               : in    std_logic; 
          CLK_4                 : in    std_logic; 
          CLK_8                 : in    std_logic; 
          GPI                   : in    std_logic_vector (7 downto 0); 
          HW_VERS_BD            : in    std_logic_vector (7 downto 0); 
          RCYC_STAT_FrequValOut : in    std_logic_vector (23 downto 0); 
          RCYC_S_PhaseValOut    : in    std_logic_vector (15 downto 0); 
          RCYC_S_StatHdx        : in    std_logic_vector (15 downto 0); 
          SPI_CLK               : in    std_logic; 
          SPI_I_MOSI            : in    std_logic; 
          SPI_N_CS              : in    std_logic; 
          X_RST                 : in    std_logic; 
          BTM                   : out   std_logic; 
          BTM_GPO               : out   std_logic_vector (7 downto 0); 
          BTM_PWMA              : out   std_logic; 
          BTM_PWMB              : out   std_logic; 
          FBIF_TEST_1           : out   std_logic; 
          FBIF_TEST_2           : out   std_logic; 
          FPGA_RUN              : out   std_logic; 
          FrequVal              : out   std_logic_vector (23 downto 0); 
          GPO                   : out   std_logic_vector (7 downto 0); 
          Phase_Val_Adc         : out   std_logic_vector (15 downto 0); 
          Pwm_Val               : out   std_logic_vector (15 downto 0); 
          SPI_O_MISO            : out   std_logic);
end uP_SPI_IF_MUSER_FPGA_DUPS_Main;

architecture BEHAVIORAL of uP_SPI_IF_MUSER_FPGA_DUPS_Main is
   signal XLXN_30               : std_logic_vector (7 downto 0);
   component FBIF_SPIIF
      port ( ADC_ERROR          : in    std_logic; 
             CLK_1ms            : in    std_logic; 
             CLK_4              : in    std_logic; 
             CLK_8              : in    std_logic; 
             CLK_M              : in    std_logic; 
             GPI                : in    std_logic_vector (7 downto 0); 
             HW_VERS_BD         : in    std_logic_vector (7 downto 0); 
             RCYC_S_FrequValOut : in    std_logic_vector (23 downto 0); 
             RCYC_S_PhaseValOut : in    std_logic_vector (15 downto 0); 
             RCYC_S_StatHdx     : in    std_logic_vector (15 downto 0); 
             SPI_CLK            : in    std_logic; 
             SPI_I_MOSI         : in    std_logic; 
             SPI_N_CS           : in    std_logic; 
             TYPE_BD            : in    std_logic_vector (7 downto 0); 
             X_RST              : in    std_logic; 
             BTM                : out   std_logic; 
             BTM_GPO            : out   std_logic_vector (7 downto 0); 
             BTM_PWMA           : out   std_logic; 
             BTM_PWMB           : out   std_logic; 
             FBIF_TEST_1        : out   std_logic; 
             FBIF_TEST_2        : out   std_logic; 
             FPGA_CTRL_TEST     : out   std_logic_vector (15 downto 0); 
             FPGA_RUN           : out   std_logic; 
             FPGA_STAT_TEST     : out   std_logic_vector (15 downto 0); 
             GPO                : out   std_logic_vector (7 downto 0); 
             SPI_O_MISO         : out   std_logic; 
             WCYC_P_CtrlHdx     : out   std_logic_vector (15 downto 0); 
             WCYC_P_FrequValIn  : out   std_logic_vector (23 downto 0); 
             WCYC_P_PhaseValIn  : out   std_logic_vector (15 downto 0); 
             WCYC_P_PWMVal      : out   std_logic_vector (15 downto 0));
   end component;
   
begin
   XLXN_30(7 downto 0) <= x"02";
   XLXI_22 : FBIF_SPIIF
      port map (ADC_ERROR=>ADC_ERROR,
                CLK_M=>CLK_M,
                CLK_1ms=>CLK_1ms,
                CLK_4=>CLK_4,
                CLK_8=>CLK_8,
                GPI(7 downto 0)=>GPI(7 downto 0),
                HW_VERS_BD(7 downto 0)=>HW_VERS_BD(7 downto 0),
                RCYC_S_FrequValOut(23 downto 0)=>RCYC_STAT_FrequValOut(23 
            downto 0),
                RCYC_S_PhaseValOut(15 downto 0)=>RCYC_S_PhaseValOut(15 downto 0),
                RCYC_S_StatHdx(15 downto 0)=>RCYC_S_StatHdx(15 downto 0),
                SPI_CLK=>SPI_CLK,
                SPI_I_MOSI=>SPI_I_MOSI,
                SPI_N_CS=>SPI_N_CS,
                TYPE_BD(7 downto 0)=>XLXN_30(7 downto 0),
                X_RST=>X_RST,
                BTM=>BTM,
                BTM_GPO(7 downto 0)=>BTM_GPO(7 downto 0),
                BTM_PWMA=>BTM_PWMA,
                BTM_PWMB=>BTM_PWMB,
                FBIF_TEST_1=>FBIF_TEST_1,
                FBIF_TEST_2=>FBIF_TEST_2,
                FPGA_CTRL_TEST=>open,
                FPGA_RUN=>FPGA_RUN,
                FPGA_STAT_TEST=>open,
                GPO(7 downto 0)=>GPO(7 downto 0),
                SPI_O_MISO=>SPI_O_MISO,
                WCYC_P_CtrlHdx=>open,
                WCYC_P_FrequValIn(23 downto 0)=>FrequVal(23 downto 0),
                WCYC_P_PhaseValIn(15 downto 0)=>Phase_Val_Adc(15 downto 0),
                WCYC_P_PWMVal(15 downto 0)=>Pwm_Val(15 downto 0));
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity ERRSTAT_MUSER_FPGA_DUPS_Main is
   port ( FSYNC_VAL_VALID : in    std_logic; 
          GPO_OL_RESET    : in    std_logic; 
          MD              : in    std_logic; 
          PHA_VAL_VALID   : in    std_logic; 
          PWM_STOP_CF     : in    std_logic; 
          RST             : in    std_logic; 
          PWM_STOP        : out   std_logic; 
          SYNC_DDS_ERR    : out   std_logic; 
          SYNC_DDS_RUN    : out   std_logic; 
          S_StatHdx       : out   std_logic_vector (15 downto 0));
end ERRSTAT_MUSER_FPGA_DUPS_Main;

architecture BEHAVIORAL of ERRSTAT_MUSER_FPGA_DUPS_Main is
   attribute BOX_TYPE   : string ;
   signal XLXN_1             : std_logic;
   signal XLXN_3             : std_logic;
   signal XLXN_4             : std_logic;
   signal XLXN_14            : std_logic;
   signal XLXN_15            : std_logic;
   signal XLXN_16            : std_logic;
   signal XLXN_23            : std_logic;
   signal XLXN_28            : std_logic;
   signal XLXN_29            : std_logic;
   signal XLXN_36            : std_logic;
   signal SYNC_DDS_RUN_DUMMY : std_logic;
   signal SYNC_DDS_ERR_DUMMY : std_logic;
   signal S_StatHdx_DUMMY    : std_logic_vector (15 downto 0);
   component AND3B3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B3 : component is "BLACK_BOX";
   
   component AND3B2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B2 : component is "BLACK_BOX";
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component FDC
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of FDC : component is "BLACK_BOX";
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
   component OR2B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2B1 : component is "BLACK_BOX";
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   component fd2c
      port ( D0  : in    std_logic; 
             D1  : in    std_logic; 
             C   : in    std_logic; 
             Q0  : out   std_logic; 
             Q1  : out   std_logic; 
             CLR : in    std_logic);
   end component;
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
begin
   SYNC_DDS_ERR <= SYNC_DDS_ERR_DUMMY;
   SYNC_DDS_RUN <= SYNC_DDS_RUN_DUMMY;
   S_StatHdx(15 downto 0) <= S_StatHdx_DUMMY(15 downto 0);
   XLXI_1 : AND3B3
      port map (I0=>SYNC_DDS_ERR_DUMMY,
                I1=>PWM_STOP_CF,
                I2=>MD,
                O=>S_StatHdx_DUMMY(0));
   
   XLXI_2 : AND3B2
      port map (I0=>SYNC_DDS_ERR_DUMMY,
                I1=>PWM_STOP_CF,
                I2=>MD,
                O=>SYNC_DDS_RUN_DUMMY);
   
   XLXI_3 : AND3B2
      port map (I0=>FSYNC_VAL_VALID,
                I1=>SYNC_DDS_ERR_DUMMY,
                I2=>MD,
                O=>XLXN_4);
   
   XLXI_4 : OR2
      port map (I0=>XLXN_3,
                I1=>XLXN_4,
                O=>S_StatHdx_DUMMY(8));
   
   XLXI_11 : FDC
      port map (C=>S_StatHdx_DUMMY(8),
                CLR=>XLXN_23,
                D=>XLXN_1,
                Q=>XLXN_3);
   
   XLXI_12 : VCC
      port map (P=>XLXN_1);
   
   XLXI_13 : AND3B2
      port map (I0=>PHA_VAL_VALID,
                I1=>SYNC_DDS_ERR_DUMMY,
                I2=>MD,
                O=>XLXN_16);
   
   XLXI_14 : OR2
      port map (I0=>XLXN_15,
                I1=>XLXN_16,
                O=>S_StatHdx_DUMMY(9));
   
   XLXI_15 : FDC
      port map (C=>S_StatHdx_DUMMY(9),
                CLR=>XLXN_23,
                D=>XLXN_14,
                Q=>XLXN_15);
   
   XLXI_16 : VCC
      port map (P=>XLXN_14);
   
   XLXI_17 : OR2B1
      port map (I0=>RST,
                I1=>GPO_OL_RESET,
                O=>XLXN_23);
   
   XLXI_18 : OR2
      port map (I0=>S_StatHdx_DUMMY(9),
                I1=>S_StatHdx_DUMMY(8),
                O=>XLXN_28);
   
   XLXI_19 : FDC
      port map (C=>XLXN_28,
                CLR=>XLXN_23,
                D=>XLXN_29,
                Q=>SYNC_DDS_ERR_DUMMY);
   
   XLXI_20 : VCC
      port map (P=>XLXN_29);
   
   XLXI_21 : BUF
      port map (I=>SYNC_DDS_ERR_DUMMY,
                O=>PWM_STOP);
   
   XLXI_25 : fd2c
      port map (C=>SYNC_DDS_ERR_DUMMY,
                CLR=>XLXN_23,
                D0=>XLXN_36,
                D1=>MD,
                Q0=>S_StatHdx_DUMMY(4),
                Q1=>S_StatHdx_DUMMY(5));
   
   XLXI_27 : INV
      port map (I=>MD,
                O=>XLXN_36);
   
   XLXI_28 : GND
      port map (G=>S_StatHdx_DUMMY(2));
   
   XLXI_29 : BUF
      port map (I=>SYNC_DDS_RUN_DUMMY,
                O=>S_StatHdx_DUMMY(1));
   
   XLXI_31 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(3));
   
   XLXI_32 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(6));
   
   XLXI_33 : BUF
      port map (I=>SYNC_DDS_ERR_DUMMY,
                O=>S_StatHdx_DUMMY(7));
   
   XLXI_34 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(10));
   
   XLXI_35 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(11));
   
   XLXI_36 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(12));
   
   XLXI_37 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(13));
   
   XLXI_38 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(14));
   
   XLXI_39 : BUF
      port map (I=>S_StatHdx_DUMMY(2),
                O=>S_StatHdx_DUMMY(15));
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity FPGA_DUPS_Main is
   port ( ALIVE          : in    std_logic; 
          CLK_DDS        : in    std_logic; 
          CLK_M          : in    std_logic; 
          CLK_1ms        : in    std_logic; 
          CLK_4          : in    std_logic; 
          CLK_8          : in    std_logic; 
          HW_VERS_BD     : in    std_logic_vector (7 downto 0); 
          SPI_CLK        : in    std_logic; 
          SPI_I_MOSI     : in    std_logic; 
          SPI_N_CS       : in    std_logic; 
          s_DSPI_ADC     : in    std_logic_vector (4 downto 0); 
          X_RST          : in    std_logic; 
          DIFF31         : out   std_logic; 
          F              : out   std_logic; 
          FBIF_TEST_1    : out   std_logic; 
          FBIF_TEST_2    : out   std_logic; 
          GPLED1_5       : out   std_logic; 
          GPLED2_6       : out   std_logic; 
          GPLED3_7       : out   std_logic; 
          N_FMOT_to_FMOT : out   std_logic; 
          N_PWMA_to_PWMB : out   std_logic; 
          N_PWMB_to_PWMA : out   std_logic; 
          SPI_O_MISO     : out   std_logic; 
          SUM31          : out   std_logic; 
          GPIO           : inout std_logic_vector (6 downto 0));
end FPGA_DUPS_Main;

architecture BEHAVIORAL of FPGA_DUPS_Main is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal BTM                                       : std_logic;
   signal FPGA_RUN                                  : std_logic;
   signal GPI                                       : std_logic_vector (7 
         downto 0);
   signal GPO                                       : std_logic_vector (7 
         downto 0);
   signal SYNC_DDS_ERR                              : std_logic;
   signal SYNC_DDS_RUN                              : std_logic;
   signal XLXN_168                                  : std_logic;
   signal XLXN_204                                  : std_logic;
   signal XLXN_221                                  : std_logic;
   signal XLXN_226                                  : std_logic;
   signal XLXN_232                                  : std_logic;
   signal XLXN_236                                  : std_logic;
   signal XLXN_241                                  : std_logic;
   signal XLXN_242                                  : std_logic;
   signal XLXN_259                                  : std_logic;
   signal XLXN_260                                  : std_logic;
   signal XLXN_267                                  : std_logic;
   signal XLXN_268                                  : std_logic;
   signal XLXN_271                                  : std_logic;
   signal XLXN_275                                  : std_logic;
   signal XLXN_276                                  : std_logic_vector (15 
         downto 0);
   signal XLXN_277                                  : std_logic_vector (23 
         downto 0);
   signal XLXN_281                                  : std_logic;
   signal XLXN_283                                  : std_logic;
   signal XLXN_284                                  : std_logic;
   signal XLXI_63_FSYNC_VAL_VALID_openSignal        : std_logic;
   signal XLXI_63_PHA_VAL_VALID_openSignal          : std_logic;
   signal XLXI_104_ADC_ERROR_openSignal             : std_logic;
   signal XLXI_104_RCYC_STAT_FrequValOut_openSignal : std_logic_vector (23 
         downto 0);
   signal XLXI_104_RCYC_S_PhaseValOut_openSignal    : std_logic_vector (15 
         downto 0);
   signal XLXI_104_RCYC_S_StatHdx_openSignal        : std_logic_vector (15 
         downto 0);
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   component ERRSTAT_MUSER_FPGA_DUPS_Main
      port ( FSYNC_VAL_VALID : in    std_logic; 
             PHA_VAL_VALID   : in    std_logic; 
             MD              : in    std_logic; 
             RST             : in    std_logic; 
             PWM_STOP_CF     : in    std_logic; 
             GPO_OL_RESET    : in    std_logic; 
             S_StatHdx       : out   std_logic_vector (15 downto 0); 
             SYNC_DDS_RUN    : out   std_logic; 
             SYNC_DDS_ERR    : out   std_logic; 
             PWM_STOP        : out   std_logic);
   end component;
   
   component M2_1_HXILINX_FPGA_DUPS_Main
      port ( D0 : in    std_logic; 
             D1 : in    std_logic; 
             S0 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   component or2clk
      port ( I1 : in    std_logic; 
             O  : out   std_logic; 
             I0 : in    std_logic; 
             C  : in    std_logic);
   end component;
   
   component uP_SPI_IF_MUSER_FPGA_DUPS_Main
      port ( ADC_ERROR             : in    std_logic; 
             CLK_1ms               : in    std_logic; 
             CLK_4                 : in    std_logic; 
             CLK_8                 : in    std_logic; 
             CLK_M                 : in    std_logic; 
             GPI                   : in    std_logic_vector (7 downto 0); 
             HW_VERS_BD            : in    std_logic_vector (7 downto 0); 
             RCYC_STAT_FrequValOut : in    std_logic_vector (23 downto 0); 
             RCYC_S_PhaseValOut    : in    std_logic_vector (15 downto 0); 
             RCYC_S_StatHdx        : in    std_logic_vector (15 downto 0); 
             SPI_CLK               : in    std_logic; 
             SPI_I_MOSI            : in    std_logic; 
             SPI_N_CS              : in    std_logic; 
             X_RST                 : in    std_logic; 
             BTM_GPO               : inout std_logic_vector (7 downto 0); 
             BTM                   : out   std_logic; 
             BTM_PWMA              : out   std_logic; 
             BTM_PWMB              : out   std_logic; 
             FBIF_TEST_1           : out   std_logic; 
             FBIF_TEST_2           : out   std_logic; 
             FPGA_RUN              : out   std_logic; 
             FrequVal              : out   std_logic_vector (23 downto 0); 
             GPO                   : out   std_logic_vector (7 downto 0); 
             Pwm_Val               : out   std_logic_vector (15 downto 0); 
             SPI_O_MISO            : out   std_logic);
   end component;
   
   component PWM_DDS
      port ( CLK_DDS   : in    std_logic; 
             Motion    : out   std_logic; 
             PhaseA    : out   std_logic; 
             PhaseB    : out   std_logic; 
             Quad      : out   std_logic; 
             Pwm_Val   : in    std_logic_vector (15 downto 0); 
             Freq_Val  : in    std_logic_vector (23 downto 0); 
             X_RST     : in    std_logic; 
             CLK_8     : in    std_logic; 
             SONICS_ON : in    std_logic);
   end component;
   
   attribute HU_SET of XLXI_65 : label is "XLXI_65_4";
   attribute HU_SET of XLXI_71 : label is "XLXI_71_0";
   attribute HU_SET of XLXI_72 : label is "XLXI_72_1";
   attribute HU_SET of XLXI_80 : label is "XLXI_80_2";
   attribute HU_SET of XLXI_81 : label is "XLXI_81_3";
begin
   XLXI_4 : INV
      port map (I=>XLXN_281,
                O=>N_PWMA_to_PWMB);
   
   XLXI_5 : INV
      port map (I=>XLXN_283,
                O=>N_PWMB_to_PWMA);
   
   XLXI_6 : INV
      port map (I=>XLXN_284,
                O=>N_FMOT_to_FMOT);
   
   XLXI_23_0 : GND
      port map (G=>GPI(0));
   
   XLXI_23_1 : GND
      port map (G=>GPI(1));
   
   XLXI_28 : BUF
      port map (I=>s_DSPI_ADC(4),
                O=>GPI(2));
   
   XLXI_29 : BUF
      port map (I=>GPIO(0),
                O=>GPI(3));
   
   XLXI_30 : BUF
      port map (I=>GPIO(1),
                O=>GPI(4));
   
   XLXI_31 : BUF
      port map (I=>GPIO(2),
                O=>GPI(5));
   
   XLXI_32 : BUF
      port map (I=>GPIO(3),
                O=>GPI(6));
   
   XLXI_35 : BUF
      port map (I=>GPIO(4),
                O=>GPI(7));
   
   XLXI_36 : INV
      port map (I=>GPO(1),
                O=>XLXN_204);
   
   XLXI_37 : BUF
      port map (I=>GPO(0),
                O=>GPIO(5));
   
   XLXI_38 : BUF
      port map (I=>XLXN_226,
                O=>GPIO(6));
   
   XLXI_63 : ERRSTAT_MUSER_FPGA_DUPS_Main
      port map (FSYNC_VAL_VALID=>XLXI_63_FSYNC_VAL_VALID_openSignal,
                GPO_OL_RESET=>GPO(0),
                MD=>XLXN_232,
                PHA_VAL_VALID=>XLXI_63_PHA_VAL_VALID_openSignal,
                PWM_STOP_CF=>XLXN_204,
                RST=>FPGA_RUN,
                PWM_STOP=>XLXN_221,
                SYNC_DDS_ERR=>SYNC_DDS_ERR,
                SYNC_DDS_RUN=>SYNC_DDS_RUN,
                S_StatHdx=>open);
   
   XLXI_65 : M2_1_HXILINX_FPGA_DUPS_Main
      port map (D0=>XLXN_271,
                D1=>XLXN_204,
                S0=>BTM,
                O=>XLXN_226);
   
   XLXI_66 : GND
      port map (G=>XLXN_232);
   
   XLXI_71 : M2_1_HXILINX_FPGA_DUPS_Main
      port map (D0=>XLXN_242,
                D1=>XLXN_267,
                S0=>XLXN_241,
                O=>XLXN_259);
   
   XLXI_72 : M2_1_HXILINX_FPGA_DUPS_Main
      port map (D0=>X_RST,
                D1=>XLXN_268,
                S0=>XLXN_241,
                O=>XLXN_260);
   
   XLXI_75 : BUF
      port map (I=>ALIVE,
                O=>GPLED1_5);
   
   XLXI_79 : INV
      port map (I=>ALIVE,
                O=>XLXN_236);
   
   XLXI_80 : M2_1_HXILINX_FPGA_DUPS_Main
      port map (D0=>XLXN_259,
                D1=>XLXN_236,
                S0=>BTM,
                O=>GPLED2_6);
   
   XLXI_81 : M2_1_HXILINX_FPGA_DUPS_Main
      port map (D0=>XLXN_260,
                D1=>XLXN_236,
                S0=>BTM,
                O=>GPLED3_7);
   
   XLXI_82 : INV
      port map (I=>FPGA_RUN,
                O=>XLXN_242);
   
   XLXI_83 : AND2
      port map (I0=>XLXN_236,
                I1=>SYNC_DDS_ERR,
                O=>XLXN_267);
   
   XLXI_84 : AND2
      port map (I0=>XLXN_236,
                I1=>SYNC_DDS_RUN,
                O=>XLXN_268);
   
   XLXI_85 : GND
      port map (G=>XLXN_241);
   
   XLXI_90 : AND2
      port map (I0=>X_RST,
                I1=>XLXN_168,
                O=>FPGA_RUN);
   
   XLXI_95 : or2clk
      port map (C=>CLK_8,
                I0=>XLXN_204,
                I1=>XLXN_221,
                O=>XLXN_271);
   
   XLXI_104 : uP_SPI_IF_MUSER_FPGA_DUPS_Main
      port map (ADC_ERROR=>XLXI_104_ADC_ERROR_openSignal,
                CLK_M=>CLK_M,
                CLK_1ms=>CLK_1ms,
                CLK_4=>CLK_4,
                CLK_8=>CLK_8,
                GPI(7 downto 0)=>GPI(7 downto 0),
                HW_VERS_BD(7 downto 0)=>HW_VERS_BD(7 downto 0),
                RCYC_STAT_FrequValOut(23 downto 
            0)=>XLXI_104_RCYC_STAT_FrequValOut_openSignal(23 downto 0),
                RCYC_S_PhaseValOut(15 downto 
            0)=>XLXI_104_RCYC_S_PhaseValOut_openSignal(15 downto 0),
                RCYC_S_StatHdx(15 downto 
            0)=>XLXI_104_RCYC_S_StatHdx_openSignal(15 downto 0),
                SPI_CLK=>SPI_CLK,
                SPI_I_MOSI=>SPI_I_MOSI,
                SPI_N_CS=>SPI_N_CS,
                X_RST=>X_RST,
                BTM=>BTM,
                BTM_PWMA=>open,
                BTM_PWMB=>open,
                FBIF_TEST_1=>FBIF_TEST_1,
                FBIF_TEST_2=>FBIF_TEST_2,
                FPGA_RUN=>XLXN_168,
                FrequVal(23 downto 0)=>XLXN_277(23 downto 0),
                GPO(7 downto 0)=>GPO(7 downto 0),
                Pwm_Val(15 downto 0)=>XLXN_276(15 downto 0),
                SPI_O_MISO=>SPI_O_MISO,
                BTM_GPO=>open);
   
   XLXI_134 : BUF
      port map (I=>XLXN_275,
                O=>SUM31);
   
   XLXI_135 : BUF
      port map (I=>XLXN_275,
                O=>DIFF31);
   
   XLXI_136 : GND
      port map (G=>XLXN_275);
   
   XLXI_137 : PWM_DDS
      port map (CLK_DDS=>CLK_DDS,
                CLK_8=>CLK_8,
                Freq_Val(23 downto 0)=>XLXN_277(23 downto 0),
                Pwm_Val(15 downto 0)=>XLXN_276(15 downto 0),
                SONICS_ON=>GPO(1),
                X_RST=>FPGA_RUN,
                Motion=>XLXN_284,
                PhaseA=>XLXN_283,
                PhaseB=>XLXN_281,
                Quad=>F);
   
end BEHAVIORAL;


