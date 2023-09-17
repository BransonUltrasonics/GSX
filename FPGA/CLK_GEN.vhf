--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : CLK_GEN.vhf
-- /___/   /\     Timestamp : 07/17/2021 11:41:08
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/CLK_GEN.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/CLK_GEN.sch"
--Design Name: CLK_GEN
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--
----- CELL CC16CE_HXILINX_CLK_GEN -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity CC16CE_HXILINX_CLK_GEN is
port (
    CEO : out STD_LOGIC;
    Q   : out STD_LOGIC_VECTOR(15 downto 0);
    TC  : out STD_LOGIC;
    C   : in  STD_LOGIC;
    CE  : in  STD_LOGIC;
    CLR : in  STD_LOGIC
    );
end CC16CE_HXILINX_CLK_GEN;

architecture Behavioral of CC16CE_HXILINX_CLK_GEN is

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
----- CELL FTC_HXILINX_CLK_GEN -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FTC_HXILINX_CLK_GEN is
generic(
    INIT : bit := '0'
    );

  port (
    Q   : out STD_LOGIC := '0';
    C   : in STD_LOGIC;
    CLR : in STD_LOGIC;
    T   : in STD_LOGIC
    );
end FTC_HXILINX_CLK_GEN;

architecture Behavioral of FTC_HXILINX_CLK_GEN is
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

----- CELL COMPM16_HXILINX_CLK_GEN -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.std_logic_unsigned.all;

entity COMPM16_HXILINX_CLK_GEN is
port(
    GT  : out std_logic;
    LT  : out std_logic;

    A   : in std_logic_vector(15 downto 0);
    B   : in std_logic_vector(15 downto 0)
  );
end COMPM16_HXILINX_CLK_GEN;

architecture COMPM16_HXILINX_CLK_GEN_V of COMPM16_HXILINX_CLK_GEN is
begin
     
  GT <= '1' when ( A > B ) else '0';
  LT <= '1' when ( A < B ) else '0';

end COMPM16_HXILINX_CLK_GEN_V;
----- CELL AND7_HXILINX_CLK_GEN -----
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity AND7_HXILINX_CLK_GEN is
  
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
end AND7_HXILINX_CLK_GEN;

architecture AND7_HXILINX_CLK_GEN_V of AND7_HXILINX_CLK_GEN is
begin
  O <= I0 and I1 and I2 and I3 and I4 and I5 and I6;
end AND7_HXILINX_CLK_GEN_V;

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity CLK_GEN is
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
end CLK_GEN;

architecture BEHAVIORAL of CLK_GEN is
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
   
   component CC16CE_HXILINX_CLK_GEN
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             CEO : out   std_logic; 
             Q   : out   std_logic_vector (15 downto 0); 
             TC  : out   std_logic);
   end component;
   
   component COMPM16_HXILINX_CLK_GEN
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
   
   component FTC_HXILINX_CLK_GEN
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
   
   component AND7_HXILINX_CLK_GEN
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
   
   attribute HU_SET of XLXI_28 : label is "XLXI_28_5";
   attribute HU_SET of XLXI_31 : label is "XLXI_31_6";
   attribute HU_SET of XLXI_37 : label is "XLXI_37_7";
   attribute HU_SET of XLXI_46 : label is "XLXI_46_8";
   attribute HU_SET of XLXI_50 : label is "XLXI_50_9";
   attribute HU_SET of XLXI_57 : label is "XLXI_57_10";
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
   
   XLXI_28 : CC16CE_HXILINX_CLK_GEN
      port map (C=>CLK_4_DUMMY,
                CE=>XLXN_53,
                CLR=>XLXN_62,
                CEO=>open,
                Q(15 downto 0)=>XLXN_57(15 downto 0),
                TC=>open);
   
   XLXI_30 : VCC
      port map (P=>XLXN_53);
   
   XLXI_31 : COMPM16_HXILINX_CLK_GEN
      port map (A(15 downto 0)=>XLXN_57(15 downto 0),
                B(15 downto 0)=>XLXN_58(15 downto 0),
                GT=>XLXN_63,
                LT=>open);
   
   XLXI_35 : OR2
      port map (I0=>XLXN_63,
                I1=>Reset,
                O=>XLXN_62);
   
   XLXI_37 : FTC_HXILINX_CLK_GEN
      port map (C=>XLXN_63,
                CLR=>Reset,
                T=>XLXN_66,
                Q=>XLXN_70);
   
   XLXI_38 : VCC
      port map (P=>XLXN_66);
   
   XLXI_46 : CC16CE_HXILINX_CLK_GEN
      port map (C=>CLK_1ms_DUMMY,
                CE=>XLXN_77,
                CLR=>Reset,
                CEO=>open,
                Q(15 downto 0)=>XLXN_79(15 downto 0),
                TC=>open);
   
   XLXI_47 : VCC
      port map (P=>XLXN_77);
   
   XLXI_50 : CC16CE_HXILINX_CLK_GEN
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
   
   XLXI_57 : AND7_HXILINX_CLK_GEN
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


