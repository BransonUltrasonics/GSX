--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : uP_SPI_IF.vhf
-- /___/   /\     Timestamp : 07/17/2021 11:41:11
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/uP_SPI_IF.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/uP_SPI_IF.sch"
--Design Name: uP_SPI_IF
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity uP_SPI_IF is
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
end uP_SPI_IF;

architecture BEHAVIORAL of uP_SPI_IF is
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


