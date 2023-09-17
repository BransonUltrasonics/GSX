--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : FPGA_DUPS.vhf
-- /___/   /\     Timestamp : 07/17/2021 11:41:09
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/FPGA_DUPS.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/FPGA_DUPS.sch"
--Design Name: FPGA_DUPS
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--
----- CELL CC16CE_HXILINX_FPGA_DUPS -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity CC16CE_HXILINX_FPGA_DUPS is
port (
    CEO : out STD_LOGIC;
    Q   : out STD_LOGIC_VECTOR(15 downto 0);
    TC  : out STD_LOGIC;
    C   : in  STD_LOGIC;
    CE  : in  STD_LOGIC;
    CLR : in  STD_LOGIC
    );
end CC16CE_HXILINX_FPGA_DUPS;

architecture Behavioral of CC16CE_HXILINX_FPGA_DUPS is

  signal COUNT : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
  constant TERMINAL_COUNT : STD_LOGIC_VECTOR(15 downto 0) := (others => '1');
  
begin

  process(C, CLR)
  begin
    if (CLR='1') then
      COUNT <= (others => '0');
    elsif (C'event and C = '1') then
      if (CE='1') then 
      COUNT <= COUNT+1;
      end if;
    end if;
  end process;


  TC <= '0' when (CLR = '1') else
        '1' when (COUNT = TERMINAL_COUNT) else '0';
  CEO <= '1' when ((COUNT = TERMINAL_COUNT) and CE='1') else '0';
  Q<=COUNT;

end Behavioral;
----- CELL FTC_HXILINX_FPGA_DUPS -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FTC_HXILINX_FPGA_DUPS is
generic(
    INIT : bit := '0'
    );

  port (
    Q   : out STD_LOGIC := '0';
    C   : in STD_LOGIC;
    CLR : in STD_LOGIC;
    T   : in STD_LOGIC
    );
end FTC_HXILINX_FPGA_DUPS;

architecture Behavioral of FTC_HXILINX_FPGA_DUPS is
signal q_tmp : std_logic := TO_X01(INIT);
begin

process(C, CLR)
begin
  if (CLR='1') then
    q_tmp <= '0';
  elsif (C'event and C = '1') then
    if(T='1') then
      q_tmp <= not q_tmp;
    end if;
  end if;  
end process;

Q <= q_tmp;

end Behavioral;

----- CELL COMPM16_HXILINX_FPGA_DUPS -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.std_logic_unsigned.all;

entity COMPM16_HXILINX_FPGA_DUPS is
port(
    GT  : out std_logic;
    LT  : out std_logic;

    A   : in std_logic_vector(15 downto 0);
    B   : in std_logic_vector(15 downto 0)
  );
end COMPM16_HXILINX_FPGA_DUPS;

architecture COMPM16_HXILINX_FPGA_DUPS_V of COMPM16_HXILINX_FPGA_DUPS is
begin
     
  GT <= '1' when ( A > B ) else '0';
  LT <= '1' when ( A < B ) else '0';

end COMPM16_HXILINX_FPGA_DUPS_V;
----- CELL M2_1_HXILINX_FPGA_DUPS -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity M2_1_HXILINX_FPGA_DUPS is
  
port(
    O   : out std_logic;

    D0  : in std_logic;
    D1  : in std_logic;
    S0  : in std_logic
  );
end M2_1_HXILINX_FPGA_DUPS;

architecture M2_1_HXILINX_FPGA_DUPS_V of M2_1_HXILINX_FPGA_DUPS is
begin
  process (D0, D1, S0)
  begin
    case S0 is
    when '0' => O <= D0;
    when '1' => O <= D1;
    when others => NULL;
    end case;
    end process; 
end M2_1_HXILINX_FPGA_DUPS_V;
----- CELL AND7_HXILINX_FPGA_DUPS -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity AND7_HXILINX_FPGA_DUPS is
  
port(
    O  : out std_logic;

    I0  : in std_logic;
    I1  : in std_logic;
    I2  : in std_logic;
    I3  : in std_logic;
    I4  : in std_logic;
    I5  : in std_logic;
    I6  : in std_logic
  );
end AND7_HXILINX_FPGA_DUPS;

architecture AND7_HXILINX_FPGA_DUPS_V of AND7_HXILINX_FPGA_DUPS is
begin
  O <= I0 and I1 and I2 and I3 and I4 and I5 and I6;
end AND7_HXILINX_FPGA_DUPS_V;

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity uP_SPI_IF_MUSER_FPGA_DUPS is
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
end uP_SPI_IF_MUSER_FPGA_DUPS;

architecture BEHAVIORAL of uP_SPI_IF_MUSER_FPGA_DUPS is
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

entity ERRSTAT_MUSER_FPGA_DUPS is
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
end ERRSTAT_MUSER_FPGA_DUPS;

architecture BEHAVIORAL of ERRSTAT_MUSER_FPGA_DUPS is
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

entity FPGA_DUPS_Main_MUSER_FPGA_DUPS is
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
end FPGA_DUPS_Main_MUSER_FPGA_DUPS;

architecture BEHAVIORAL of FPGA_DUPS_Main_MUSER_FPGA_DUPS is
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
   
   component ERRSTAT_MUSER_FPGA_DUPS
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
   
   component M2_1_HXILINX_FPGA_DUPS
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
   
   component uP_SPI_IF_MUSER_FPGA_DUPS
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
   
   attribute HU_SET of XLXI_65 : label is "XLXI_65_15";
   attribute HU_SET of XLXI_71 : label is "XLXI_71_11";
   attribute HU_SET of XLXI_72 : label is "XLXI_72_12";
   attribute HU_SET of XLXI_80 : label is "XLXI_80_13";
   attribute HU_SET of XLXI_81 : label is "XLXI_81_14";
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
   
   XLXI_63 : ERRSTAT_MUSER_FPGA_DUPS
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
   
   XLXI_65 : M2_1_HXILINX_FPGA_DUPS
      port map (D0=>XLXN_271,
                D1=>XLXN_204,
                S0=>BTM,
                O=>XLXN_226);
   
   XLXI_66 : GND
      port map (G=>XLXN_232);
   
   XLXI_71 : M2_1_HXILINX_FPGA_DUPS
      port map (D0=>XLXN_242,
                D1=>XLXN_267,
                S0=>XLXN_241,
                O=>XLXN_259);
   
   XLXI_72 : M2_1_HXILINX_FPGA_DUPS
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
   
   XLXI_80 : M2_1_HXILINX_FPGA_DUPS
      port map (D0=>XLXN_259,
                D1=>XLXN_236,
                S0=>BTM,
                O=>GPLED2_6);
   
   XLXI_81 : M2_1_HXILINX_FPGA_DUPS
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
   
   XLXI_104 : uP_SPI_IF_MUSER_FPGA_DUPS
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



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity CLK_GEN_MUSER_FPGA_DUPS is
   port ( CLK_25MHZ  : in    std_logic; 
          RST        : in    std_logic; 
          ALIVE      : out   std_logic; 
          CLK_DDS    : out   std_logic; 
          CLK_FADC   : out   std_logic; 
          CLK_M      : out   std_logic; 
          CLK_1ms    : out   std_logic; 
          CLK_4      : out   std_logic; 
          CLK_8      : out   std_logic; 
          SUD_LOCKED : out   std_logic);
end CLK_GEN_MUSER_FPGA_DUPS;

architecture BEHAVIORAL of CLK_GEN_MUSER_FPGA_DUPS is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal Clock_Manager_FXOUT : std_logic;
   signal LCKD1               : std_logic;
   signal LCKD2               : std_logic;
   signal LCKD3               : std_logic;
   signal LCKD4               : std_logic;
   signal q                   : std_logic_vector (2 downto 0);
   signal Reset               : std_logic;
   signal S_CLK_DDS           : std_logic;
   signal S_CLK_M             : std_logic;
   signal S_CLK_4             : std_logic;
   signal S_CLK_8             : std_logic;
   signal XLXN_2              : std_logic;
   signal XLXN_6              : std_logic;
   signal XLXN_7              : std_logic;
   signal XLXN_9              : std_logic;
   signal XLXN_39             : std_logic;
   signal XLXN_44             : std_logic;
   signal XLXN_53             : std_logic;
   signal XLXN_57             : std_logic_vector (15 downto 0);
   signal XLXN_58             : std_logic_vector (15 downto 0);
   signal XLXN_62             : std_logic;
   signal XLXN_63             : std_logic;
   signal XLXN_66             : std_logic;
   signal XLXN_70             : std_logic;
   signal XLXN_77             : std_logic;
   signal XLXN_79             : std_logic_vector (15 downto 0);
   signal XLXN_86             : std_logic;
   signal XLXN_92             : std_logic;
   signal XLXN_95             : std_logic;
   signal XLXN_96             : std_logic;
   signal XLXN_106            : std_logic;
   signal XLXN_107            : std_logic;
   signal XLXN_112            : std_logic;
   signal XLXN_113            : std_logic;
   signal XLXN_164            : std_logic;
   signal XLXN_172            : std_logic;
   signal XLXN_181            : std_logic;
   signal CLK_4_DUMMY         : std_logic;
   signal CLK_1ms_DUMMY       : std_logic;
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component counter3cr
      port ( clk  : in    std_logic; 
             ce   : in    std_logic; 
             sclr : in    std_logic; 
             q    : out   std_logic_vector (2 downto 0));
   end component;
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
   component LDC
      generic( INIT : bit :=  '0');
      port ( CLR : in    std_logic; 
             D   : in    std_logic; 
             G   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of LDC : component is "BLACK_BOX";
   
   component FDC
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of FDC : component is "BLACK_BOX";
   
   component AND2B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2B1 : component is "BLACK_BOX";
   
   component FDCE
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of FDCE : component is "BLACK_BOX";
   
   component CC16CE_HXILINX_FPGA_DUPS
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             CEO : out   std_logic; 
             Q   : out   std_logic_vector (15 downto 0); 
             TC  : out   std_logic);
   end component;
   
   component COMPM16_HXILINX_FPGA_DUPS
      port ( A  : in    std_logic_vector (15 downto 0); 
             B  : in    std_logic_vector (15 downto 0); 
             GT : out   std_logic; 
             LT : out   std_logic);
   end component;
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component FTC_HXILINX_FPGA_DUPS
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             T   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   component AND3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3 : component is "BLACK_BOX";
   
   component counter23
      port ( Q   : out   std_logic_vector (23 downto 0); 
             TC  : out   std_logic; 
             CLR : in    std_logic; 
             C   : in    std_logic; 
             CE  : in    std_logic; 
             CEO : out   std_logic);
   end component;
   
   component AND7_HXILINX_FPGA_DUPS
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             I4 : in    std_logic; 
             I5 : in    std_logic; 
             I6 : in    std_logic; 
             O  : out   std_logic);
   end component;
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component CLKMAN_1
      port ( RST_IN     : in    std_logic; 
             CLKIN_IN   : in    std_logic; 
             LOCKED_OUT : out   std_logic; 
             CLK90_OUT  : out   std_logic; 
             CLK0_OUT   : out   std_logic);
   end component;
   
   component CLKMAN_2
      port ( RST_IN     : in    std_logic; 
             CLKIN_IN   : in    std_logic; 
             CLK0_OUT   : out   std_logic; 
             LOCKED_OUT : out   std_logic; 
             CLK90_OUT  : out   std_logic);
   end component;
   
   component CLKMAN_3
      port ( RST_IN     : in    std_logic; 
             CLKIN_IN   : in    std_logic; 
             LOCKED_OUT : out   std_logic; 
             CLK90_OUT  : out   std_logic; 
             CLK0_OUT   : out   std_logic);
   end component;
   
   component CLKMAN_4
      port ( RST_IN     : in    std_logic; 
             CLKIN_IN   : in    std_logic; 
             LOCKED_OUT : out   std_logic; 
             CLK90_OUT  : out   std_logic; 
             CLK0_OUT   : out   std_logic);
   end component;
   
   component CLOCK_MANAGER
      port ( CLKIN_IN   : in    std_logic; 
             RST_IN     : in    std_logic; 
             CLKFX_OUT  : out   std_logic; 
             LOCKED_OUT : out   std_logic);
   end component;
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   attribute HU_SET of XLXI_28 : label is "XLXI_28_16";
   attribute HU_SET of XLXI_31 : label is "XLXI_31_17";
   attribute HU_SET of XLXI_37 : label is "XLXI_37_18";
   attribute HU_SET of XLXI_46 : label is "XLXI_46_19";
   attribute HU_SET of XLXI_50 : label is "XLXI_50_20";
   attribute HU_SET of XLXI_57 : label is "XLXI_57_21";
begin
   XLXN_58(15 downto 0) <= x"61A8";
   CLK_1ms <= CLK_1ms_DUMMY;
   CLK_4 <= CLK_4_DUMMY;
   XLXI_3 : INV
      port map (I=>RST,
                O=>XLXN_2);
   
   XLXI_4 : counter3cr
      port map (ce=>XLXN_6,
                clk=>Clock_Manager_FXOUT,
                sclr=>Reset,
                q(2 downto 0)=>q(2 downto 0));
   
   XLXI_6 : VCC
      port map (P=>XLXN_6);
   
   XLXI_7 : INV
      port map (I=>XLXN_7,
                O=>Reset);
   
   XLXI_9 : LDC
      port map (CLR=>Reset,
                D=>Clock_Manager_FXOUT,
                G=>XLXN_9,
                Q=>S_CLK_M);
   
   XLXI_10 : VCC
      port map (P=>XLXN_9);
   
   XLXI_16 : FDC
      port map (C=>Clock_Manager_FXOUT,
                CLR=>Reset,
                D=>q(0),
                Q=>S_CLK_DDS);
   
   XLXI_20 : FDC
      port map (C=>Clock_Manager_FXOUT,
                CLR=>Reset,
                D=>q(1),
                Q=>S_CLK_4);
   
   XLXI_22 : FDC
      port map (C=>Clock_Manager_FXOUT,
                CLR=>Reset,
                D=>q(2),
                Q=>S_CLK_8);
   
   XLXI_24 : AND2B1
      port map (I0=>XLXN_39,
                I1=>q(2),
                O=>XLXN_44);
   
   XLXI_25 : FDC
      port map (C=>Clock_Manager_FXOUT,
                CLR=>Reset,
                D=>q(2),
                Q=>XLXN_39);
   
   XLXI_26 : FDCE
      port map (C=>Clock_Manager_FXOUT,
                CE=>XLXN_44,
                CLR=>Reset,
                D=>XLXN_70,
                Q=>CLK_1ms_DUMMY);
   
   XLXI_28 : CC16CE_HXILINX_FPGA_DUPS
      port map (C=>CLK_4_DUMMY,
                CE=>XLXN_53,
                CLR=>XLXN_62,
                CEO=>open,
                Q(15 downto 0)=>XLXN_57(15 downto 0),
                TC=>open);
   
   XLXI_30 : VCC
      port map (P=>XLXN_53);
   
   XLXI_31 : COMPM16_HXILINX_FPGA_DUPS
      port map (A(15 downto 0)=>XLXN_57(15 downto 0),
                B(15 downto 0)=>XLXN_58(15 downto 0),
                GT=>XLXN_63,
                LT=>open);
   
   XLXI_35 : OR2
      port map (I0=>XLXN_63,
                I1=>Reset,
                O=>XLXN_62);
   
   XLXI_37 : FTC_HXILINX_FPGA_DUPS
      port map (C=>XLXN_63,
                CLR=>Reset,
                T=>XLXN_66,
                Q=>XLXN_70);
   
   XLXI_38 : VCC
      port map (P=>XLXN_66);
   
   XLXI_46 : CC16CE_HXILINX_FPGA_DUPS
      port map (C=>CLK_1ms_DUMMY,
                CE=>XLXN_77,
                CLR=>Reset,
                CEO=>open,
                Q(15 downto 0)=>XLXN_79(15 downto 0),
                TC=>open);
   
   XLXI_47 : VCC
      port map (P=>XLXN_77);
   
   XLXI_50 : CC16CE_HXILINX_FPGA_DUPS
      port map (C=>CLK_25MHZ,
                CE=>XLXN_86,
                CLR=>XLXN_2,
                CEO=>open,
                Q=>open,
                TC=>XLXN_112);
   
   XLXI_51 : INV
      port map (I=>XLXN_112,
                O=>XLXN_86);
   
   XLXI_53 : AND3
      port map (I0=>XLXN_112,
                I1=>XLXN_7,
                I2=>XLXN_96,
                O=>XLXN_107);
   
   XLXI_54 : counter23
      port map (C=>CLK_25MHZ,
                CE=>XLXN_107,
                CLR=>XLXN_92,
                CEO=>open,
                Q=>open,
                TC=>XLXN_95);
   
   XLXI_55 : OR2
      port map (I0=>XLXN_2,
                I1=>XLXN_181,
                O=>XLXN_92);
   
   XLXI_56 : INV
      port map (I=>XLXN_95,
                O=>XLXN_96);
   
   XLXI_57 : AND7_HXILINX_FPGA_DUPS
      port map (I0=>XLXN_164,
                I1=>LCKD4,
                I2=>LCKD3,
                I3=>LCKD2,
                I4=>LCKD1,
                I5=>XLXN_95,
                I6=>XLXN_7,
                O=>XLXN_106);
   
   XLXI_58 : VCC
      port map (P=>XLXN_164);
   
   XLXI_61 : VCC
      port map (P=>XLXN_113);
   
   XLXI_62 : GND
      port map (G=>CLK_FADC);
   
   XLXI_81 : CLKMAN_1
      port map (CLKIN_IN=>S_CLK_M,
                RST_IN=>Reset,
                CLK0_OUT=>CLK_M,
                CLK90_OUT=>open,
                LOCKED_OUT=>LCKD1);
   
   XLXI_87 : CLKMAN_2
      port map (CLKIN_IN=>S_CLK_DDS,
                RST_IN=>Reset,
                CLK0_OUT=>CLK_DDS,
                CLK90_OUT=>open,
                LOCKED_OUT=>LCKD2);
   
   XLXI_95 : CLKMAN_3
      port map (CLKIN_IN=>S_CLK_4,
                RST_IN=>Reset,
                CLK0_OUT=>CLK_4_DUMMY,
                CLK90_OUT=>open,
                LOCKED_OUT=>LCKD3);
   
   XLXI_98 : CLKMAN_4
      port map (CLKIN_IN=>S_CLK_8,
                RST_IN=>Reset,
                CLK0_OUT=>CLK_8,
                CLK90_OUT=>open,
                LOCKED_OUT=>LCKD4);
   
   XLXI_103 : CLOCK_MANAGER
      port map (CLKIN_IN=>CLK_25MHZ,
                RST_IN=>XLXN_2,
                CLKFX_OUT=>Clock_Manager_FXOUT,
                LOCKED_OUT=>XLXN_7);
   
   XLXI_105 : BUF
      port map (I=>XLXN_172,
                O=>SUD_LOCKED);
   
   XLXI_106 : AND2
      port map (I0=>XLXN_172,
                I1=>XLXN_79(8),
                O=>ALIVE);
   
   XLXI_107 : AND2
      port map (I0=>XLXN_106,
                I1=>XLXN_113,
                O=>XLXN_172);
   
   XLXI_110 : INV
      port map (I=>XLXN_112,
                O=>XLXN_181);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity FPGA_DUPS is
   port ( GPI_OL_TEMP      : in    std_logic; 
          HW_VERS_BD       : in    std_logic_vector (7 downto 0); 
          I_CLK_25MHZ      : in    std_logic; 
          I_DSPI_CLK       : in    std_logic; 
          I_DSPI_CS_ADC    : in    std_logic; 
          I_DSPI_RXD       : in    std_logic; 
          I_DSPI_TXD       : in    std_logic; 
          I_N_RESET        : in    std_logic; 
          I_N_RESET_UP_OUT : in    std_logic; 
          I_PHASE_SIGN     : in    std_logic; 
          I_SPI_CLK        : in    std_logic; 
          I_SPI_MOSI       : in    std_logic; 
          I_SPI_N_CS       : in    std_logic; 
          OL_CURRENT       : in    std_logic; 
          OL_POWER         : in    std_logic; 
          OL_VOLTAGE       : in    std_logic; 
          OVL_FUNCTION     : in    std_logic; 
          FMOT             : out   std_logic; 
          FQUAD            : out   std_logic; 
          GPO_DDS_STOP     : out   std_logic; 
          GPO_OL_RESET     : out   std_logic; 
          O_N_LED5         : out   std_logic; 
          O_N_LED6         : out   std_logic; 
          O_N_LED7         : out   std_logic; 
          O_SPI_MISO       : out   std_logic; 
          O_TP_51          : out   std_logic; 
          O_TP_52          : out   std_logic; 
          O_TP_53          : out   std_logic; 
          O_TP_54          : out   std_logic; 
          PWMA             : out   std_logic; 
          PWMB             : out   std_logic);
end FPGA_DUPS;

architecture BEHAVIORAL of FPGA_DUPS is
   attribute BOX_TYPE         : string ;
   attribute IOSTANDARD       : string ;
   attribute IBUF_DELAY_VALUE : string ;
   attribute IFD_DELAY_VALUE  : string ;
   signal GPIO             : std_logic_vector (6 downto 0);
   signal s_DSPI_ADC       : std_logic_vector (4 downto 0);
   signal XLXN_1           : std_logic;
   signal XLXN_2           : std_logic;
   signal XLXN_56          : std_logic;
   signal XLXN_107         : std_logic;
   signal XLXN_116         : std_logic;
   signal XLXN_122         : std_logic;
   signal XLXN_139         : std_logic;
   signal XLXN_140         : std_logic;
   signal XLXN_141         : std_logic;
   signal XLXN_148         : std_logic;
   signal XLXN_150         : std_logic;
   signal XLXN_152         : std_logic;
   signal XLXN_176         : std_logic;
   signal XLXN_180         : std_logic;
   signal XLXN_205         : std_logic;
   signal XLXN_206         : std_logic;
   signal XLXN_209         : std_logic;
   signal XLXN_210         : std_logic;
   signal PWMA_DUMMY       : std_logic;
   signal PWMB_DUMMY       : std_logic;
   component RST_DEB_1
      port ( CLK_25MHZ   : in    std_logic; 
             X_RST_IN_1  : in    std_logic; 
             X_RST_OUT_1 : out   std_logic);
   end component;
   
   component RST_DEB_2
      port ( CLK_25MHZ   : in    std_logic; 
             X_RST_IN_2  : in    std_logic; 
             X_RST_OUT_2 : out   std_logic);
   end component;
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component CLK_GEN_MUSER_FPGA_DUPS
      port ( CLK_M      : out   std_logic; 
             CLK_DDS    : out   std_logic; 
             CLK_4      : out   std_logic; 
             CLK_8      : out   std_logic; 
             CLK_1ms    : out   std_logic; 
             CLK_FADC   : out   std_logic; 
             ALIVE      : out   std_logic; 
             SUD_LOCKED : out   std_logic; 
             RST        : in    std_logic; 
             CLK_25MHZ  : in    std_logic);
   end component;
   
   component IBUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute IOSTANDARD of IBUF : component is "DEFAULT";
   attribute IBUF_DELAY_VALUE of IBUF : component is "0";
   attribute IFD_DELAY_VALUE of IBUF : component is "AUTO";
   attribute BOX_TYPE of IBUF : component is "BLACK_BOX";
   
   component FPGA_DUPS_Main_MUSER_FPGA_DUPS
      port ( ALIVE          : in    std_logic; 
             CLK_1ms        : in    std_logic; 
             CLK_4          : in    std_logic; 
             CLK_8          : in    std_logic; 
             CLK_DDS        : in    std_logic; 
             CLK_M          : in    std_logic; 
             HW_VERS_BD     : in    std_logic_vector (7 downto 0); 
             SPI_CLK        : in    std_logic; 
             SPI_I_MOSI     : in    std_logic; 
             SPI_N_CS       : in    std_logic; 
             s_DSPI_ADC     : in    std_logic_vector (4 downto 0); 
             X_RST          : in    std_logic; 
             GPIO           : inout std_logic_vector (6 downto 0); 
             F              : out   std_logic; 
             GPLED1_5       : out   std_logic; 
             GPLED2_6       : out   std_logic; 
             GPLED3_7       : out   std_logic; 
             N_FMOT_to_FMOT : out   std_logic; 
             N_PWMA_to_PWMB : out   std_logic; 
             N_PWMB_to_PWMA : out   std_logic; 
             SPI_O_MISO     : out   std_logic; 
             FBIF_TEST_1    : out   std_logic; 
             FBIF_TEST_2    : out   std_logic; 
             SUM31          : out   std_logic; 
             DIFF31         : out   std_logic);
   end component;
   
begin
   PWMA <= PWMA_DUMMY;
   PWMB <= PWMB_DUMMY;
   XLXI_3 : RST_DEB_1
      port map (CLK_25MHZ=>XLXN_122,
                X_RST_IN_1=>I_N_RESET,
                X_RST_OUT_1=>XLXN_1);
   
   XLXI_4 : RST_DEB_2
      port map (CLK_25MHZ=>XLXN_122,
                X_RST_IN_2=>I_N_RESET_UP_OUT,
                X_RST_OUT_2=>XLXN_2);
   
   XLXI_5 : AND2
      port map (I0=>XLXN_2,
                I1=>XLXN_1,
                O=>XLXN_56);
   
   XLXI_6 : BUF
      port map (I=>I_DSPI_CLK,
                O=>s_DSPI_ADC(0));
   
   XLXI_7 : BUF
      port map (I=>I_DSPI_CS_ADC,
                O=>s_DSPI_ADC(1));
   
   XLXI_8 : BUF
      port map (I=>I_DSPI_RXD,
                O=>s_DSPI_ADC(2));
   
   XLXI_9 : BUF
      port map (I=>I_DSPI_TXD,
                O=>s_DSPI_ADC(3));
   
   XLXI_10 : BUF
      port map (I=>I_PHASE_SIGN,
                O=>s_DSPI_ADC(4));
   
   XLXI_11 : INV
      port map (I=>XLXN_139,
                O=>O_N_LED5);
   
   XLXI_12 : INV
      port map (I=>XLXN_140,
                O=>O_N_LED6);
   
   XLXI_13 : INV
      port map (I=>XLXN_141,
                O=>O_N_LED7);
   
   XLXI_14 : BUF
      port map (I=>GPI_OL_TEMP,
                O=>GPIO(0));
   
   XLXI_15 : BUF
      port map (I=>OL_POWER,
                O=>GPIO(1));
   
   XLXI_16 : BUF
      port map (I=>OL_VOLTAGE,
                O=>GPIO(2));
   
   XLXI_17 : BUF
      port map (I=>OL_CURRENT,
                O=>GPIO(3));
   
   XLXI_18 : BUF
      port map (I=>OVL_FUNCTION,
                O=>GPIO(4));
   
   XLXI_19 : BUF
      port map (I=>GPIO(5),
                O=>GPO_OL_RESET);
   
   XLXI_20 : BUF
      port map (I=>GPIO(6),
                O=>GPO_DDS_STOP);
   
   XLXI_33 : CLK_GEN_MUSER_FPGA_DUPS
      port map (CLK_25MHZ=>XLXN_122,
                RST=>XLXN_56,
                ALIVE=>XLXN_116,
                CLK_DDS=>XLXN_150,
                CLK_FADC=>open,
                CLK_M=>XLXN_148,
                CLK_1ms=>XLXN_180,
                CLK_4=>XLXN_152,
                CLK_8=>XLXN_176,
                SUD_LOCKED=>XLXN_107);
   
   XLXI_34 : IBUF
      port map (I=>I_CLK_25MHZ,
                O=>XLXN_122);
   
   XLXI_47 : FPGA_DUPS_Main_MUSER_FPGA_DUPS
      port map (ALIVE=>XLXN_116,
                CLK_DDS=>XLXN_150,
                CLK_M=>XLXN_148,
                CLK_1ms=>XLXN_180,
                CLK_4=>XLXN_152,
                CLK_8=>XLXN_176,
                HW_VERS_BD(7 downto 0)=>HW_VERS_BD(7 downto 0),
                SPI_CLK=>I_SPI_CLK,
                SPI_I_MOSI=>I_SPI_MOSI,
                SPI_N_CS=>I_SPI_N_CS,
                s_DSPI_ADC(4 downto 0)=>s_DSPI_ADC(4 downto 0),
                X_RST=>XLXN_107,
                DIFF31=>XLXN_210,
                F=>FQUAD,
                FBIF_TEST_1=>open,
                FBIF_TEST_2=>open,
                GPLED1_5=>XLXN_139,
                GPLED2_6=>XLXN_140,
                GPLED3_7=>XLXN_141,
                N_FMOT_to_FMOT=>FMOT,
                N_PWMA_to_PWMB=>PWMB_DUMMY,
                N_PWMB_to_PWMA=>PWMA_DUMMY,
                SPI_O_MISO=>O_SPI_MISO,
                SUM31=>XLXN_209,
                GPIO(6 downto 0)=>GPIO(6 downto 0));
   
   XLXI_53 : BUF
      port map (I=>XLXN_206,
                O=>O_TP_53);
   
   XLXI_54 : BUF
      port map (I=>XLXN_205,
                O=>O_TP_51);
   
   XLXI_57 : INV
      port map (I=>PWMA_DUMMY,
                O=>XLXN_205);
   
   XLXI_58 : INV
      port map (I=>PWMB_DUMMY,
                O=>XLXN_206);
   
   XLXI_59 : BUF
      port map (I=>XLXN_209,
                O=>O_TP_52);
   
   XLXI_60 : BUF
      port map (I=>XLXN_210,
                O=>O_TP_54);
   
end BEHAVIORAL;


