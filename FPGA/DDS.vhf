--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : DDS.vhf
-- /___/   /\     Timestamp : 07/17/2021 14:42:18
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/DDS.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/DDS.sch"
--Design Name: DDS
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--
----- CELL SR4RLE_HXILINX_DDS -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SR4RLE_HXILINX_DDS is
port (
    Q0  : out STD_LOGIC;
    Q1  : out STD_LOGIC;
    Q2  : out STD_LOGIC;
    Q3  : out STD_LOGIC;
    C   : in STD_LOGIC;
    CE  : in STD_LOGIC;
    D0  : in STD_LOGIC;
    D1  : in STD_LOGIC;
    D2  : in STD_LOGIC;
    D3  : in STD_LOGIC;
    L   : in STD_LOGIC; 
    R   : in STD_LOGIC;
    SLI : in STD_LOGIC
    );
end SR4RLE_HXILINX_DDS;

architecture Behavioral of SR4RLE_HXILINX_DDS is
signal q_tmp : STD_LOGIC_VECTOR(3 downto 0);
begin

process(C)
begin
  if (C'event and C = '1') then
    if (R='1') then
      q_tmp <= "0000";
    elsif (L= '1') then
      q_tmp <= D3&D2&D1&D0;
    elsif (CE='1') then 
      q_tmp <= ( q_tmp(2 downto 0) & SLI );
    end if;
  end if;
end process;
Q3 <= q_tmp(3);
Q2 <= q_tmp(2);
Q1 <= q_tmp(1);
Q0 <= q_tmp(0);

end Behavioral;

----- CELL FJKC_HXILINX_DDS -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FJKC_HXILINX_DDS is
generic(
    INIT : bit := '0'
    );

  port (
    Q   : out STD_LOGIC := '0';
    C   : in STD_LOGIC;
    CLR : in STD_LOGIC;
    J   : in STD_LOGIC;
    K   : in STD_LOGIC
    );
end FJKC_HXILINX_DDS;

architecture Behavioral of FJKC_HXILINX_DDS is
signal q_tmp : std_logic := TO_X01(INIT);

begin

process(C, CLR)
begin
  if (CLR='1') then
    q_tmp <= '0';
  elsif (C'event and C = '1') then
    if(J='0') then
      if(K='1') then
      q_tmp <= '0';
    end if;
    else
      if(K='0') then
      q_tmp <= '1';
      else
      q_tmp <= not q_tmp;
      end if;
    end if;
  end if;  
end process;

Q <= q_tmp;

end Behavioral;

----- CELL SR4RE_HXILINX_DDS -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SR4RE_HXILINX_DDS is
port (
    Q0  : out STD_LOGIC;
    Q1  : out STD_LOGIC;
    Q2  : out STD_LOGIC;
    Q3  : out STD_LOGIC;
    C   : in STD_LOGIC;
    CE  : in STD_LOGIC;
    R   : in STD_LOGIC;
    SLI : in STD_LOGIC
    );
end SR4RE_HXILINX_DDS;

architecture Behavioral of SR4RE_HXILINX_DDS is
signal q_tmp : STD_LOGIC_VECTOR(3 downto 0);
begin

process(C)
begin
  if (C'event and C = '1') then
    if (R='1') then
      q_tmp <= "0000";
    elsif (CE='1') then 
      q_tmp <= ( q_tmp(2 downto 0) & SLI );
    end if;
  end if;
end process;
Q3 <= q_tmp(3);
Q2 <= q_tmp(2);
Q1 <= q_tmp(1);
Q0 <= q_tmp(0);

end Behavioral;


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity DDS_FGEN_PH_ACC_MUSER_DDS is
   port ( CLK_DDS       : in    std_logic; 
          CLK_8         : in    std_logic; 
          Frequ_Val     : in    std_logic_vector (23 downto 0); 
          LOAD          : in    std_logic; 
          Load_Val      : in    std_logic_vector (31 downto 0); 
          RST           : in    std_logic; 
          Phase_Acc_Val : out   std_logic_vector (31 downto 0));
end DDS_FGEN_PH_ACC_MUSER_DDS;

architecture BEHAVIORAL of DDS_FGEN_PH_ACC_MUSER_DDS is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal accBus              : std_logic_vector (31 downto 0);
   signal XLXN_2              : std_logic_vector (23 downto 0);
   signal XLXN_15             : std_logic_vector (23 downto 0);
   signal XLXN_16             : std_logic;
   signal XLXN_18             : std_logic;
   signal XLXN_26             : std_logic_vector (23 downto 0);
   signal XLXN_27             : std_logic;
   signal XLXN_29             : std_logic;
   signal XLXN_32             : std_logic;
   signal XLXN_34             : std_logic;
   signal XLXN_36             : std_logic;
   signal XLXN_37             : std_logic;
   signal XLXN_38             : std_logic;
   signal XLXN_41             : std_logic;
   signal XLXN_42             : std_logic;
   signal XLXN_45             : std_logic;
   signal XLXN_47             : std_logic;
   signal XLXN_49             : std_logic;
   signal XLXN_65             : std_logic;
   signal XLXN_66             : std_logic;
   signal XLXN_67             : std_logic;
   signal XLXN_68             : std_logic;
   signal XLXN_69             : std_logic;
   signal XLXN_95             : std_logic;
   signal XLXN_105            : std_logic;
   signal XLXN_114            : std_logic;
   signal XLXN_115            : std_logic;
   signal XLXN_116            : std_logic;
   signal XLXN_117            : std_logic;
   signal XLXN_118            : std_logic;
   signal XLXN_128            : std_logic;
   signal XLXN_129            : std_logic;
   signal XLXN_130            : std_logic;
   signal XLXN_131            : std_logic;
   signal XLXN_132            : std_logic;
   signal XLXN_133            : std_logic;
   signal XLXN_136            : std_logic;
   signal XLXN_137            : std_logic;
   signal XLXN_139            : std_logic_vector (31 downto 0);
   signal XLXN_140            : std_logic_vector (31 downto 0);
   signal XLXN_146            : std_logic;
   signal XLXN_147            : std_logic;
   signal Phase_Acc_Val_DUMMY : std_logic_vector (31 downto 0);
   component fd23ce
      port ( D_B : in    std_logic_vector (23 downto 0); 
             CE  : in    std_logic; 
             C   : in    std_logic; 
             CLR : in    std_logic; 
             Q_B : out   std_logic_vector (23 downto 0));
   end component;
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component comp23clk
      port ( A  : in    std_logic_vector (23 downto 0); 
             B  : in    std_logic_vector (23 downto 0); 
             C  : in    std_logic; 
             EQ : out   std_logic);
   end component;
   
   component SR4RLE_HXILINX_DDS
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             D0  : in    std_logic; 
             D1  : in    std_logic; 
             D2  : in    std_logic; 
             D3  : in    std_logic; 
             L   : in    std_logic; 
             R   : in    std_logic; 
             SLI : in    std_logic; 
             Q0  : out   std_logic; 
             Q1  : out   std_logic; 
             Q2  : out   std_logic; 
             Q3  : out   std_logic);
   end component;
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component NOR4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of NOR4 : component is "BLACK_BOX";
   
   component OR3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR3 : component is "BLACK_BOX";
   
   component SR4RE_HXILINX_DDS
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             R   : in    std_logic; 
             SLI : in    std_logic; 
             Q0  : out   std_logic; 
             Q1  : out   std_logic; 
             Q2  : out   std_logic; 
             Q3  : out   std_logic);
   end component;
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component acc32
      port ( CI  : in    std_logic; 
             B   : in    std_logic_vector (31 downto 0); 
             D   : in    std_logic_vector (31 downto 0); 
             L   : in    std_logic; 
             ADD : in    std_logic; 
             CE  : in    std_logic; 
             C   : in    std_logic; 
             R   : in    std_logic; 
             Q   : out   std_logic_vector (31 downto 0); 
             CO  : out   std_logic; 
             OFL : out   std_logic);
   end component;
   
   component comp32
      port ( A  : in    std_logic_vector (31 downto 0); 
             B  : in    std_logic_vector (31 downto 0); 
             GT : out   std_logic);
   end component;
   
   component FJKC_HXILINX_DDS
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             J   : in    std_logic; 
             K   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   attribute HU_SET of XLXI_15 : label is "XLXI_15_42";
   attribute HU_SET of XLXI_25 : label is "XLXI_25_43";
   attribute HU_SET of XLXI_31 : label is "XLXI_31_44";
   attribute HU_SET of XLXI_57 : label is "XLXI_57_45";
   attribute HU_SET of XLXI_61 : label is "XLXI_61_46";
   attribute HU_SET of XLXI_62 : label is "XLXI_62_47";
begin
   XLXN_139(31 downto 0) <= x"C0000000";
   XLXN_140(31 downto 0) <= x"40000000";
   Phase_Acc_Val(31 downto 0) <= Phase_Acc_Val_DUMMY(31 downto 0);
   XLXI_5 : fd23ce
      port map (C=>CLK_8,
                CE=>XLXN_16,
                CLR=>RST,
                D_B(23 downto 0)=>Frequ_Val(23 downto 0),
                Q_B(23 downto 0)=>XLXN_15(23 downto 0));
   
   XLXI_6 : fd23ce
      port map (C=>CLK_8,
                CE=>XLXN_32,
                CLR=>RST,
                D_B(23 downto 0)=>XLXN_15(23 downto 0),
                Q_B(23 downto 0)=>XLXN_2(23 downto 0));
   
   XLXI_7 : fd23ce
      port map (C=>CLK_DDS,
                CE=>XLXN_49,
                CLR=>RST,
                D_B(23 downto 0)=>XLXN_2(23 downto 0),
                Q_B(23 downto 0)=>accBus(23 downto 0));
   
   XLXI_8 : OR2
      port map (I0=>XLXN_18,
                I1=>XLXN_69,
                O=>XLXN_16);
   
   XLXI_13 : comp23clk
      port map (A(23 downto 0)=>XLXN_26(23 downto 0),
                B(23 downto 0)=>XLXN_15(23 downto 0),
                C=>CLK_8,
                EQ=>XLXN_18);
   
   XLXI_15 : SR4RLE_HXILINX_DDS
      port map (C=>CLK_8,
                CE=>XLXN_34,
                D0=>XLXN_34,
                D1=>XLXN_29,
                D2=>XLXN_29,
                D3=>XLXN_29,
                L=>XLXN_41,
                R=>XLXN_27,
                SLI=>XLXN_29,
                Q0=>XLXN_32,
                Q1=>XLXN_36,
                Q2=>XLXN_37,
                Q3=>XLXN_42);
   
   XLXI_18 : VCC
      port map (P=>XLXN_34);
   
   XLXI_20_0 : GND
      port map (G=>XLXN_26(0));
   
   XLXI_20_1 : GND
      port map (G=>XLXN_26(1));
   
   XLXI_20_2 : GND
      port map (G=>XLXN_26(2));
   
   XLXI_20_3 : GND
      port map (G=>XLXN_26(3));
   
   XLXI_20_4 : GND
      port map (G=>XLXN_26(4));
   
   XLXI_20_5 : GND
      port map (G=>XLXN_26(5));
   
   XLXI_20_6 : GND
      port map (G=>XLXN_26(6));
   
   XLXI_20_7 : GND
      port map (G=>XLXN_26(7));
   
   XLXI_20_8 : GND
      port map (G=>XLXN_26(8));
   
   XLXI_20_9 : GND
      port map (G=>XLXN_26(9));
   
   XLXI_20_10 : GND
      port map (G=>XLXN_26(10));
   
   XLXI_20_11 : GND
      port map (G=>XLXN_26(11));
   
   XLXI_20_12 : GND
      port map (G=>XLXN_26(12));
   
   XLXI_20_13 : GND
      port map (G=>XLXN_26(13));
   
   XLXI_20_14 : GND
      port map (G=>XLXN_26(14));
   
   XLXI_20_15 : GND
      port map (G=>XLXN_26(15));
   
   XLXI_20_16 : GND
      port map (G=>XLXN_26(16));
   
   XLXI_20_17 : GND
      port map (G=>XLXN_26(17));
   
   XLXI_20_18 : GND
      port map (G=>XLXN_26(18));
   
   XLXI_20_19 : GND
      port map (G=>XLXN_26(19));
   
   XLXI_20_20 : GND
      port map (G=>XLXN_26(20));
   
   XLXI_20_21 : GND
      port map (G=>XLXN_26(21));
   
   XLXI_20_22 : GND
      port map (G=>XLXN_26(22));
   
   XLXI_20_23 : GND
      port map (G=>XLXN_26(23));
   
   XLXI_21 : GND
      port map (G=>XLXN_27);
   
   XLXI_22 : GND
      port map (G=>XLXN_29);
   
   XLXI_23 : NOR4
      port map (I0=>XLXN_32,
                I1=>XLXN_36,
                I2=>XLXN_37,
                I3=>XLXN_42,
                O=>XLXN_38);
   
   XLXI_24 : OR3
      port map (I0=>XLXN_38,
                I1=>XLXN_42,
                I2=>RST,
                O=>XLXN_41);
   
   XLXI_25 : SR4RE_HXILINX_DDS
      port map (C=>CLK_DDS,
                CE=>XLXN_45,
                R=>XLXN_42,
                SLI=>XLXN_37,
                Q0=>XLXN_47,
                Q1=>open,
                Q2=>XLXN_146,
                Q3=>open);
   
   XLXI_28 : VCC
      port map (P=>XLXN_45);
   
   XLXI_31 : SR4RE_HXILINX_DDS
      port map (C=>CLK_8,
                CE=>XLXN_65,
                R=>RST,
                SLI=>XLXN_137,
                Q0=>XLXN_68,
                Q1=>XLXN_66,
                Q2=>open,
                Q3=>open);
   
   XLXI_33 : VCC
      port map (P=>XLXN_65);
   
   XLXI_34 : AND2
      port map (I0=>XLXN_68,
                I1=>XLXN_67,
                O=>XLXN_69);
   
   XLXI_35 : INV
      port map (I=>XLXN_66,
                O=>XLXN_67);
   
   XLXI_36 : acc32
      port map (ADD=>XLXN_105,
                B(31 downto 0)=>accBus(31 downto 0),
                C=>CLK_DDS,
                CE=>XLXN_105,
                CI=>XLXN_95,
                D(31 downto 0)=>Load_Val(31 downto 0),
                L=>LOAD,
                R=>RST,
                CO=>open,
                OFL=>open,
                Q(31 downto 0)=>Phase_Acc_Val_DUMMY(31 downto 0));
   
   XLXI_53 : GND
      port map (G=>XLXN_95);
   
   XLXI_54 : VCC
      port map (P=>XLXN_105);
   
   XLXI_55 : comp32
      port map (A(31 downto 0)=>Phase_Acc_Val_DUMMY(31 downto 0),
                B(31 downto 0)=>XLXN_139(31 downto 0),
                GT=>XLXN_114);
   
   XLXI_56 : comp32
      port map (A(31 downto 0)=>Phase_Acc_Val_DUMMY(31 downto 0),
                B(31 downto 0)=>XLXN_140(31 downto 0),
                GT=>XLXN_132);
   
   XLXI_57 : SR4RE_HXILINX_DDS
      port map (C=>CLK_DDS,
                CE=>XLXN_118,
                R=>RST,
                SLI=>XLXN_114,
                Q0=>XLXN_117,
                Q1=>XLXN_115,
                Q2=>open,
                Q3=>open);
   
   XLXI_58 : AND2
      port map (I0=>XLXN_117,
                I1=>XLXN_116,
                O=>XLXN_128);
   
   XLXI_59 : INV
      port map (I=>XLXN_115,
                O=>XLXN_116);
   
   XLXI_60 : VCC
      port map (P=>XLXN_118);
   
   XLXI_61 : FJKC_HXILINX_DDS
      port map (C=>CLK_DDS,
                CLR=>RST,
                J=>XLXN_136,
                K=>XLXN_128,
                Q=>XLXN_137);
   
   XLXI_62 : SR4RE_HXILINX_DDS
      port map (C=>CLK_DDS,
                CE=>XLXN_133,
                R=>RST,
                SLI=>XLXN_132,
                Q0=>XLXN_129,
                Q1=>XLXN_131,
                Q2=>open,
                Q3=>open);
   
   XLXI_63 : AND2
      port map (I0=>XLXN_129,
                I1=>XLXN_130,
                O=>XLXN_136);
   
   XLXI_64 : INV
      port map (I=>XLXN_131,
                O=>XLXN_130);
   
   XLXI_65 : VCC
      port map (P=>XLXN_133);
   
   XLXI_71_0 : GND
      port map (G=>accBus(24));
   
   XLXI_71_1 : GND
      port map (G=>accBus(25));
   
   XLXI_71_2 : GND
      port map (G=>accBus(26));
   
   XLXI_71_3 : GND
      port map (G=>accBus(27));
   
   XLXI_71_4 : GND
      port map (G=>accBus(28));
   
   XLXI_71_5 : GND
      port map (G=>accBus(29));
   
   XLXI_71_6 : GND
      port map (G=>accBus(30));
   
   XLXI_71_7 : GND
      port map (G=>accBus(31));
   
   XLXI_72 : AND2
      port map (I0=>XLXN_147,
                I1=>XLXN_47,
                O=>XLXN_49);
   
   XLXI_73 : INV
      port map (I=>XLXN_146,
                O=>XLXN_147);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity DDS_FGEN_MUSER_DDS is
   port ( CLK_DDS       : in    std_logic; 
          CLK_8         : in    std_logic; 
          Frequ_Val     : in    std_logic_vector (23 downto 0); 
          RST_INV       : in    std_logic; 
          Phase_Acc_Val : out   std_logic_vector (31 downto 0));
end DDS_FGEN_MUSER_DDS;

architecture BEHAVIORAL of DDS_FGEN_MUSER_DDS is
   attribute BOX_TYPE   : string ;
   signal XLXN_6        : std_logic_vector (31 downto 0);
   signal XLXN_9        : std_logic;
   signal XLXN_10       : std_logic;
   component DDS_FGEN_PH_ACC_MUSER_DDS
      port ( CLK_8         : in    std_logic; 
             CLK_DDS       : in    std_logic; 
             Frequ_Val     : in    std_logic_vector (23 downto 0); 
             LOAD          : in    std_logic; 
             Load_Val      : in    std_logic_vector (31 downto 0); 
             RST           : in    std_logic; 
             Phase_Acc_Val : out   std_logic_vector (31 downto 0));
   end component;
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
begin
   XLXI_1 : DDS_FGEN_PH_ACC_MUSER_DDS
      port map (CLK_DDS=>CLK_DDS,
                CLK_8=>CLK_8,
                Frequ_Val(23 downto 0)=>Frequ_Val(23 downto 0),
                LOAD=>XLXN_9,
                Load_Val(31 downto 0)=>XLXN_6(31 downto 0),
                RST=>XLXN_10,
                Phase_Acc_Val(31 downto 0)=>Phase_Acc_Val(31 downto 0));
   
   XLXI_3_0 : GND
      port map (G=>XLXN_6(0));
   
   XLXI_3_1 : GND
      port map (G=>XLXN_6(1));
   
   XLXI_3_2 : GND
      port map (G=>XLXN_6(2));
   
   XLXI_3_3 : GND
      port map (G=>XLXN_6(3));
   
   XLXI_3_4 : GND
      port map (G=>XLXN_6(4));
   
   XLXI_3_5 : GND
      port map (G=>XLXN_6(5));
   
   XLXI_3_6 : GND
      port map (G=>XLXN_6(6));
   
   XLXI_3_7 : GND
      port map (G=>XLXN_6(7));
   
   XLXI_3_8 : GND
      port map (G=>XLXN_6(8));
   
   XLXI_3_9 : GND
      port map (G=>XLXN_6(9));
   
   XLXI_3_10 : GND
      port map (G=>XLXN_6(10));
   
   XLXI_3_11 : GND
      port map (G=>XLXN_6(11));
   
   XLXI_3_12 : GND
      port map (G=>XLXN_6(12));
   
   XLXI_3_13 : GND
      port map (G=>XLXN_6(13));
   
   XLXI_3_14 : GND
      port map (G=>XLXN_6(14));
   
   XLXI_3_15 : GND
      port map (G=>XLXN_6(15));
   
   XLXI_3_16 : GND
      port map (G=>XLXN_6(16));
   
   XLXI_3_17 : GND
      port map (G=>XLXN_6(17));
   
   XLXI_3_18 : GND
      port map (G=>XLXN_6(18));
   
   XLXI_3_19 : GND
      port map (G=>XLXN_6(19));
   
   XLXI_3_20 : GND
      port map (G=>XLXN_6(20));
   
   XLXI_3_21 : GND
      port map (G=>XLXN_6(21));
   
   XLXI_3_22 : GND
      port map (G=>XLXN_6(22));
   
   XLXI_3_23 : GND
      port map (G=>XLXN_6(23));
   
   XLXI_3_24 : GND
      port map (G=>XLXN_6(24));
   
   XLXI_3_25 : GND
      port map (G=>XLXN_6(25));
   
   XLXI_3_26 : GND
      port map (G=>XLXN_6(26));
   
   XLXI_3_27 : GND
      port map (G=>XLXN_6(27));
   
   XLXI_3_28 : GND
      port map (G=>XLXN_6(28));
   
   XLXI_3_29 : GND
      port map (G=>XLXN_6(29));
   
   XLXI_3_30 : GND
      port map (G=>XLXN_6(30));
   
   XLXI_3_31 : GND
      port map (G=>XLXN_6(31));
   
   XLXI_4 : GND
      port map (G=>XLXN_9);
   
   XLXI_5 : INV
      port map (I=>RST_INV,
                O=>XLXN_10);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity DDS is
   port ( CLK_DDS       : in    std_logic; 
          CLK_8         : in    std_logic; 
          F             : in    std_logic; 
          FMOT          : in    std_logic; 
          Frequ_Val     : in    std_logic_vector (23 downto 0); 
          RST           : in    std_logic; 
          Phase_Acc_Val : out   std_logic_vector (31 downto 0));
end DDS;

architecture BEHAVIORAL of DDS is
   component DDS_FGEN_MUSER_DDS
      port ( CLK_8         : in    std_logic; 
             CLK_DDS       : in    std_logic; 
             Frequ_Val     : in    std_logic_vector (23 downto 0); 
             RST_INV       : in    std_logic; 
             Phase_Acc_Val : out   std_logic_vector (31 downto 0));
   end component;
   
begin
   XLXI_4 : DDS_FGEN_MUSER_DDS
      port map (CLK_DDS=>CLK_DDS,
                CLK_8=>CLK_8,
                Frequ_Val(23 downto 0)=>Frequ_Val(23 downto 0),
                RST_INV=>RST,
                Phase_Acc_Val(31 downto 0)=>Phase_Acc_Val(31 downto 0));
   
end BEHAVIORAL;


