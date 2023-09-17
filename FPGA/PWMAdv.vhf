--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : PWMAdv.vhf
-- /___/   /\     Timestamp : 07/17/2021 14:42:20
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/PWMAdv.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_Versions/SPI_ReducedVersion/GSX_FPGA/PWMAdv.sch"
--Design Name: PWMAdv
--Device: spartan3a
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--
----- CELL SR4CE_HXILINX_PWMAdv -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SR4CE_HXILINX_PWMAdv is
port (
    Q0  : out STD_LOGIC;
    Q1  : out STD_LOGIC;
    Q2  : out STD_LOGIC;
    Q3  : out STD_LOGIC;
    C   : in STD_LOGIC;
    CE  : in STD_LOGIC;
    CLR : in STD_LOGIC;
    SLI : in STD_LOGIC
    );
end SR4CE_HXILINX_PWMAdv;

architecture Behavioral of SR4CE_HXILINX_PWMAdv is
signal q_tmp : std_logic_vector(3 downto 0);
begin

process(C, CLR)
begin
  if (CLR='1') then
    q_tmp <= "0000";
  elsif (C'event and C = '1') then
    if (CE='1') then 
      q_tmp <= ( q_tmp(2 downto 0) & SLI );
    end if;
  end if;
end process;

Q3 <= q_tmp(3);
Q2 <= q_tmp(2);
Q1 <= q_tmp(1);
Q0 <= q_tmp(0);


end Behavioral;

----- CELL FD16CE_HXILINX_PWMAdv -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FD16CE_HXILINX_PWMAdv is
port (
    Q   : out STD_LOGIC_VECTOR(15 downto 0) := (others => '0');

    C   : in STD_LOGIC;
    CE  : in STD_LOGIC;
    CLR : in STD_LOGIC;
    D   : in STD_LOGIC_VECTOR(15 downto 0)
    );
end FD16CE_HXILINX_PWMAdv;

architecture Behavioral of FD16CE_HXILINX_PWMAdv is

begin

process(C, CLR)
begin
  if (CLR='1') then
    Q <= (others => '0');
  elsif (C'event and C = '1') then
    if (CE='1') then 
      Q <= D;
    end if;
  end if;
end process;


end Behavioral;

----- CELL FJKC_HXILINX_PWMAdv -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FJKC_HXILINX_PWMAdv is
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
end FJKC_HXILINX_PWMAdv;

architecture Behavioral of FJKC_HXILINX_PWMAdv is
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

----- CELL SR4CLE_HXILINX_PWMAdv -----


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SR4CLE_HXILINX_PWMAdv is
port (
    Q0  : out STD_LOGIC;
    Q1  : out STD_LOGIC;
    Q2  : out STD_LOGIC;
    Q3  : out STD_LOGIC;
    C   : in STD_LOGIC;
    CE  : in STD_LOGIC;
    CLR : in STD_LOGIC;
    D0  : in STD_LOGIC;
    D1  : in STD_LOGIC;
    D2  : in STD_LOGIC;
    D3  : in STD_LOGIC;
    L   : in STD_LOGIC;
    SLI : in STD_LOGIC
    );
end SR4CLE_HXILINX_PWMAdv;

architecture Behavioral of SR4CLE_HXILINX_PWMAdv is
signal q_tmp : std_logic_vector(3 downto 0);
begin

process(C, CLR)
begin
  if (CLR='1') then
    q_tmp <= "0000";
  elsif (C'event and C = '1') then
    if (L= '1') then
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


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity PWMAdv is
   port ( CLK_DDS          : in    std_logic; 
          CLK_8            : in    std_logic; 
          Phase_Acc_Val    : in    std_logic_vector (31 downto 0); 
          PWM_LVL_RST_STOP : in    std_logic; 
          PWM_STOP         : in    std_logic; 
          Pwm_Val          : in    std_logic_vector (15 downto 0); 
          RST_INV          : in    std_logic; 
          F                : out   std_logic; 
          FMOT             : out   std_logic; 
          PWMA             : out   std_logic; 
          PWMAdvTest       : out   std_logic_vector (15 downto 0); 
          PWMB             : out   std_logic; 
          PWM_Reg_31       : out   std_logic_vector (1 downto 0));
end PWMAdv;

architecture BEHAVIORAL of PWMAdv is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal DIFF             : std_logic_vector (31 downto 0);
   signal RST              : std_logic;
   signal SUM              : std_logic_vector (31 downto 0);
   signal XLXN_24          : std_logic;
   signal XLXN_28          : std_logic;
   signal XLXN_33          : std_logic;
   signal XLXN_34          : std_logic;
   signal XLXN_35          : std_logic;
   signal XLXN_36          : std_logic;
   signal XLXN_48          : std_logic;
   signal XLXN_49          : std_logic;
   signal XLXN_55          : std_logic;
   signal XLXN_56          : std_logic;
   signal XLXN_163         : std_logic;
   signal XLXN_175         : std_logic;
   signal XLXN_178         : std_logic;
   signal XLXN_182         : std_logic;
   signal XLXN_183         : std_logic;
   signal XLXN_193         : std_logic;
   signal XLXN_194         : std_logic;
   signal XLXN_227         : std_logic_vector (31 downto 0);
   signal XLXN_228         : std_logic_vector (31 downto 0);
   signal XLXN_230         : std_logic;
   signal XLXN_231         : std_logic;
   signal XLXN_241         : std_logic;
   signal XLXN_242         : std_logic;
   signal XLXN_244         : std_logic;
   signal XLXN_245         : std_logic;
   signal XLXN_248         : std_logic;
   signal XLXN_249         : std_logic;
   signal XLXN_251         : std_logic;
   signal XLXN_257         : std_logic;
   signal XLXN_262         : std_logic;
   signal XLXN_280         : std_logic;
   signal XLXN_286         : std_logic;
   signal XLXN_289         : std_logic;
   signal XLXN_293         : std_logic;
   signal XLXN_294         : std_logic;
   signal XLXN_302         : std_logic;
   signal XLXN_311         : std_logic;
   signal XLXN_350         : std_logic;
   signal XLXN_357         : std_logic;
   signal XLXN_362         : std_logic;
   signal XLXN_375         : std_logic;
   signal XLXN_376         : std_logic;
   signal XLXN_377         : std_logic;
   signal XLXN_379         : std_logic;
   signal XLXN_380         : std_logic;
   signal XLXN_381         : std_logic;
   signal XLXN_411         : std_logic_vector (31 downto 0);
   signal XLXN_416         : std_logic_vector (15 downto 0);
   signal XLXN_418         : std_logic;
   signal XLXN_421         : std_logic;
   signal XLXN_432         : std_logic;
   signal XLXN_433         : std_logic;
   signal XLXN_434         : std_logic_vector (1 downto 0);
   signal XLXN_438         : std_logic;
   signal FMOT_DUMMY       : std_logic;
   signal PWMA_DUMMY       : std_logic;
   signal PWMB_DUMMY       : std_logic;
   signal CLK_DDS_DUMMY    : std_logic;
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
   component AND2B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2B1 : component is "BLACK_BOX";
   
   component AND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND2 : component is "BLACK_BOX";
   
   component AND3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3 : component is "BLACK_BOX";
   
   component AND3B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B1 : component is "BLACK_BOX";
   
   component AND3B2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B2 : component is "BLACK_BOX";
   
   component AND3B3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND3B3 : component is "BLACK_BOX";
   
   component XOR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of XOR2 : component is "BLACK_BOX";
   
   component FJKC_HXILINX_PWMAdv
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             J   : in    std_logic; 
             K   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   
   component FD16CE_HXILINX_PWMAdv
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic_vector (15 downto 0); 
             Q   : out   std_logic_vector (15 downto 0));
   end component;
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
   component FDC
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of FDC : component is "BLACK_BOX";
   
   component comp32
      port ( A  : in    std_logic_vector (31 downto 0); 
             B  : in    std_logic_vector (31 downto 0); 
             GT : out   std_logic);
   end component;
   
   component SR4CE_HXILINX_PWMAdv
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             SLI : in    std_logic; 
             Q0  : out   std_logic; 
             Q1  : out   std_logic; 
             Q2  : out   std_logic; 
             Q3  : out   std_logic);
   end component;
   
   component SR4CLE_HXILINX_PWMAdv
      port ( C   : in    std_logic; 
             CE  : in    std_logic; 
             CLR : in    std_logic; 
             D0  : in    std_logic; 
             D1  : in    std_logic; 
             D2  : in    std_logic; 
             D3  : in    std_logic; 
             L   : in    std_logic; 
             SLI : in    std_logic; 
             Q0  : out   std_logic; 
             Q1  : out   std_logic; 
             Q2  : out   std_logic; 
             Q3  : out   std_logic);
   end component;
   
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
   
   component add32
      port ( A   : in    std_logic_vector (31 downto 0); 
             ADD : in    std_logic; 
             B   : in    std_logic_vector (31 downto 0); 
             C   : inout std_logic; 
             S   : out   std_logic_vector (31 downto 0));
   end component;
   
   component fjkpc
      port ( PRE : in    std_logic; 
             J   : in    std_logic; 
             K   : in    std_logic; 
             C   : in    std_logic; 
             Q   : out   std_logic; 
             CLR : in    std_logic);
   end component;
   
   component BUF
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of BUF : component is "BLACK_BOX";
   
   attribute HU_SET of XLXI_30 : label is "XLXI_30_48";
   attribute HU_SET of XLXI_46 : label is "XLXI_46_52";
   attribute HU_SET of XLXI_65 : label is "XLXI_65_49";
   attribute HU_SET of XLXI_66 : label is "XLXI_66_50";
   attribute HU_SET of XLXI_74 : label is "XLXI_74_51";
   attribute HU_SET of XLXI_80 : label is "XLXI_80_53";
   attribute HU_SET of XLXI_124 : label is "XLXI_124_54";
begin
   XLXN_227(31 downto 0) <= x"40000000";
   XLXN_228(31 downto 0) <= x"C0000000";
   FMOT <= FMOT_DUMMY;
   PWMA <= PWMA_DUMMY;
   PWMB <= PWMB_DUMMY;
   CLK_DDS_DUMMY <= CLK_DDS;
   XLXI_2 : OR2
      port map (I0=>RST,
                I1=>PWM_STOP,
                O=>XLXN_24);
   
   XLXI_3 : AND2B1
      port map (I0=>PWM_LVL_RST_STOP,
                I1=>XLXN_24,
                O=>XLXN_28);
   
   XLXI_4 : AND2
      port map (I0=>PWM_LVL_RST_STOP,
                I1=>RST,
                O=>XLXN_357);
   
   XLXI_5 : AND2B1
      port map (I0=>PWM_LVL_RST_STOP,
                I1=>RST,
                O=>XLXN_350);
   
   XLXI_6 : AND3
      port map (I0=>FMOT_DUMMY,
                I1=>XLXN_24,
                I2=>PWM_LVL_RST_STOP,
                O=>XLXN_33);
   
   XLXI_7 : AND3B1
      port map (I0=>XLXN_24,
                I1=>FMOT_DUMMY,
                I2=>PWM_LVL_RST_STOP,
                O=>XLXN_34);
   
   XLXI_8 : AND3B2
      port map (I0=>FMOT_DUMMY,
                I1=>PWM_LVL_RST_STOP,
                I2=>XLXN_28,
                O=>XLXN_35);
   
   XLXI_9 : AND3B3
      port map (I0=>FMOT_DUMMY,
                I1=>XLXN_28,
                I2=>PWM_LVL_RST_STOP,
                O=>XLXN_36);
   
   XLXI_17 : AND3B2
      port map (I0=>PWMA_DUMMY,
                I1=>FMOT_DUMMY,
                I2=>SUM(31),
                O=>XLXN_48);
   
   XLXI_18 : AND3B1
      port map (I0=>SUM(31),
                I1=>FMOT_DUMMY,
                I2=>PWMA_DUMMY,
                O=>XLXN_49);
   
   XLXI_19 : AND3B1
      port map (I0=>PWMB_DUMMY,
                I1=>FMOT_DUMMY,
                I2=>DIFF(31),
                O=>XLXN_55);
   
   XLXI_23 : AND3B2
      port map (I0=>DIFF(31),
                I1=>FMOT_DUMMY,
                I2=>PWMB_DUMMY,
                O=>XLXN_56);
   
   XLXI_28 : XOR2
      port map (I0=>Phase_Acc_Val(31),
                I1=>Phase_Acc_Val(30),
                O=>XLXN_163);
   
   XLXI_30 : FJKC_HXILINX_PWMAdv
      port map (C=>CLK_DDS_DUMMY,
                CLR=>RST,
                J=>XLXN_175,
                K=>XLXN_178,
                Q=>FMOT_DUMMY);
   
   XLXI_31 : AND3B1
      port map (I0=>FMOT_DUMMY,
                I1=>XLXN_183,
                I2=>XLXN_182,
                O=>XLXN_175);
   
   XLXI_32 : AND3B2
      port map (I0=>XLXN_182,
                I1=>XLXN_183,
                I2=>FMOT_DUMMY,
                O=>XLXN_178);
   
   XLXI_46 : FD16CE_HXILINX_PWMAdv
      port map (C=>CLK_8,
                CE=>XLXN_280,
                CLR=>RST,
                D(15 downto 0)=>Pwm_Val(15 downto 0),
                Q(15 downto 0)=>XLXN_416(15 downto 0));
   
   XLXI_47 : VCC
      port map (P=>XLXN_194);
   
   XLXI_48 : GND
      port map (G=>XLXN_193);
   
   XLXI_49 : INV
      port map (I=>RST_INV,
                O=>RST);
   
   XLXI_51_0 : GND
      port map (G=>XLXN_411(0));
   
   XLXI_51_1 : GND
      port map (G=>XLXN_411(1));
   
   XLXI_51_2 : GND
      port map (G=>XLXN_411(2));
   
   XLXI_51_3 : GND
      port map (G=>XLXN_411(3));
   
   XLXI_51_4 : GND
      port map (G=>XLXN_411(4));
   
   XLXI_51_5 : GND
      port map (G=>XLXN_411(5));
   
   XLXI_51_6 : GND
      port map (G=>XLXN_411(6));
   
   XLXI_51_7 : GND
      port map (G=>XLXN_411(7));
   
   XLXI_51_8 : GND
      port map (G=>XLXN_411(8));
   
   XLXI_51_9 : GND
      port map (G=>XLXN_411(9));
   
   XLXI_51_10 : GND
      port map (G=>XLXN_411(10));
   
   XLXI_51_11 : GND
      port map (G=>XLXN_411(11));
   
   XLXI_51_12 : GND
      port map (G=>XLXN_411(12));
   
   XLXI_51_13 : GND
      port map (G=>XLXN_411(13));
   
   XLXI_52_0 : GND
      port map (G=>XLXN_411(31));
   
   XLXI_52_1 : GND
      port map (G=>XLXN_411(30));
   
   XLXI_57 : FDC
      port map (C=>CLK_DDS_DUMMY,
                CLR=>RST,
                D=>XLXN_163,
                Q=>XLXN_183);
   
   XLXI_58 : FDC
      port map (C=>CLK_DDS_DUMMY,
                CLR=>RST,
                D=>XLXN_183,
                Q=>F);
   
   XLXI_60 : FDC
      port map (C=>CLK_DDS_DUMMY,
                CLR=>RST,
                D=>Phase_Acc_Val(31),
                Q=>XLXN_182);
   
   XLXI_61 : comp32
      port map (A(31 downto 0)=>Phase_Acc_Val(31 downto 0),
                B(31 downto 0)=>XLXN_227(31 downto 0),
                GT=>XLXN_376);
   
   XLXI_62 : comp32
      port map (A(31 downto 0)=>Phase_Acc_Val(31 downto 0),
                B(31 downto 0)=>XLXN_228(31 downto 0),
                GT=>XLXN_380);
   
   XLXI_65 : SR4CE_HXILINX_PWMAdv
      port map (C=>CLK_DDS_DUMMY,
                CE=>XLXN_231,
                CLR=>RST,
                SLI=>XLXN_375,
                Q0=>XLXN_245,
                Q1=>open,
                Q2=>XLXN_244,
                Q3=>open);
   
   XLXI_66 : SR4CE_HXILINX_PWMAdv
      port map (C=>CLK_DDS_DUMMY,
                CE=>XLXN_230,
                CLR=>RST,
                SLI=>XLXN_379,
                Q0=>XLXN_242,
                Q1=>open,
                Q2=>XLXN_241,
                Q3=>open);
   
   XLXI_67 : VCC
      port map (P=>XLXN_231);
   
   XLXI_68 : VCC
      port map (P=>XLXN_230);
   
   XLXI_70 : AND2B1
      port map (I0=>XLXN_241,
                I1=>XLXN_242,
                O=>XLXN_249);
   
   XLXI_71 : AND2B1
      port map (I0=>XLXN_244,
                I1=>XLXN_245,
                O=>XLXN_248);
   
   XLXI_73 : OR2
      port map (I0=>XLXN_248,
                I1=>XLXN_249,
                O=>open);
   
   XLXI_74 : SR4CLE_HXILINX_PWMAdv
      port map (C=>CLK_8,
                CE=>XLXN_262,
                CLR=>XLXN_251,
                D0=>XLXN_262,
                D1=>XLXN_257,
                D2=>XLXN_257,
                D3=>XLXN_257,
                L=>XLXN_311,
                SLI=>XLXN_257,
                Q0=>XLXN_280,
                Q1=>XLXN_286,
                Q2=>XLXN_293,
                Q3=>XLXN_289);
   
   XLXI_75 : GND
      port map (G=>XLXN_251);
   
   XLXI_76 : VCC
      port map (P=>XLXN_262);
   
   XLXI_77 : GND
      port map (G=>XLXN_257);
   
   XLXI_78 : NOR4
      port map (I0=>XLXN_280,
                I1=>XLXN_286,
                I2=>XLXN_293,
                I3=>XLXN_289,
                O=>XLXN_302);
   
   XLXI_80 : SR4CE_HXILINX_PWMAdv
      port map (C=>CLK_DDS_DUMMY,
                CE=>XLXN_294,
                CLR=>XLXN_289,
                SLI=>XLXN_293,
                Q0=>XLXN_433,
                Q1=>open,
                Q2=>XLXN_432,
                Q3=>open);
   
   XLXI_81 : VCC
      port map (P=>XLXN_294);
   
   XLXI_82 : OR3
      port map (I0=>XLXN_302,
                I1=>XLXN_289,
                I2=>RST,
                O=>XLXN_311);
   
   XLXI_86 : AND3B1
      port map (I0=>XLXN_432,
                I1=>XLXN_421,
                I2=>XLXN_433,
                O=>XLXN_418);
   
   XLXI_105 : add32
      port map (A(31 downto 0)=>Phase_Acc_Val(31 downto 0),
                ADD=>XLXN_194,
                B(31 downto 0)=>XLXN_411(31 downto 0),
                S(31 downto 0)=>SUM(31 downto 0),
                C=>CLK_DDS_DUMMY);
   
   XLXI_106 : add32
      port map (A(31 downto 0)=>Phase_Acc_Val(31 downto 0),
                ADD=>XLXN_193,
                B(31 downto 0)=>XLXN_411(31 downto 0),
                S(31 downto 0)=>DIFF(31 downto 0),
                C=>CLK_DDS_DUMMY);
   
   XLXI_109 : fjkpc
      port map (C=>CLK_DDS_DUMMY,
                CLR=>XLXN_350,
                J=>XLXN_33,
                K=>XLXN_34,
                PRE=>XLXN_357,
                Q=>XLXN_438);
   
   XLXI_110 : fjkpc
      port map (C=>CLK_DDS_DUMMY,
                CLR=>XLXN_357,
                J=>XLXN_35,
                K=>XLXN_36,
                PRE=>XLXN_350,
                Q=>XLXN_362);
   
   XLXI_111 : fjkpc
      port map (C=>CLK_DDS_DUMMY,
                CLR=>XLXN_362,
                J=>XLXN_48,
                K=>XLXN_49,
                PRE=>XLXN_438,
                Q=>PWMA_DUMMY);
   
   XLXI_112 : fjkpc
      port map (C=>CLK_DDS_DUMMY,
                CLR=>XLXN_362,
                J=>XLXN_55,
                K=>XLXN_56,
                PRE=>XLXN_438,
                Q=>PWMB_DUMMY);
   
   XLXI_113 : AND2
      port map (I0=>XLXN_376,
                I1=>XLXN_377,
                O=>XLXN_375);
   
   XLXI_114 : VCC
      port map (P=>XLXN_377);
   
   XLXI_115 : AND2
      port map (I0=>XLXN_380,
                I1=>XLXN_381,
                O=>XLXN_379);
   
   XLXI_116 : VCC
      port map (P=>XLXN_381);
   
   XLXI_124 : FD16CE_HXILINX_PWMAdv
      port map (C=>CLK_DDS_DUMMY,
                CE=>XLXN_418,
                CLR=>RST,
                D(15 downto 0)=>XLXN_416(15 downto 0),
                Q(15 downto 0)=>XLXN_411(29 downto 14));
   
   XLXI_126 : AND2
      port map (I0=>SUM(31),
                I1=>DIFF(31),
                O=>XLXN_421);
   
   XLXI_128_0 : BUF
      port map (I=>XLXN_411(14),
                O=>PWMAdvTest(0));
   
   XLXI_128_1 : BUF
      port map (I=>XLXN_411(15),
                O=>PWMAdvTest(1));
   
   XLXI_128_2 : BUF
      port map (I=>XLXN_411(16),
                O=>PWMAdvTest(2));
   
   XLXI_128_3 : BUF
      port map (I=>XLXN_411(17),
                O=>PWMAdvTest(3));
   
   XLXI_128_4 : BUF
      port map (I=>XLXN_411(18),
                O=>PWMAdvTest(4));
   
   XLXI_128_5 : BUF
      port map (I=>XLXN_411(19),
                O=>PWMAdvTest(5));
   
   XLXI_128_6 : BUF
      port map (I=>XLXN_411(20),
                O=>PWMAdvTest(6));
   
   XLXI_128_7 : BUF
      port map (I=>XLXN_411(21),
                O=>PWMAdvTest(7));
   
   XLXI_128_8 : BUF
      port map (I=>XLXN_411(22),
                O=>PWMAdvTest(8));
   
   XLXI_128_9 : BUF
      port map (I=>XLXN_411(23),
                O=>PWMAdvTest(9));
   
   XLXI_128_10 : BUF
      port map (I=>XLXN_411(24),
                O=>PWMAdvTest(10));
   
   XLXI_128_11 : BUF
      port map (I=>XLXN_411(25),
                O=>PWMAdvTest(11));
   
   XLXI_128_12 : BUF
      port map (I=>XLXN_411(26),
                O=>PWMAdvTest(12));
   
   XLXI_128_13 : BUF
      port map (I=>XLXN_411(27),
                O=>PWMAdvTest(13));
   
   XLXI_128_14 : BUF
      port map (I=>XLXN_411(28),
                O=>PWMAdvTest(14));
   
   XLXI_128_15 : BUF
      port map (I=>XLXN_411(29),
                O=>PWMAdvTest(15));
   
   XLXI_129_0 : BUF
      port map (I=>XLXN_434(0),
                O=>PWM_Reg_31(0));
   
   XLXI_129_1 : BUF
      port map (I=>XLXN_434(1),
                O=>PWM_Reg_31(1));
   
   XLXI_130 : BUF
      port map (I=>SUM(31),
                O=>XLXN_434(0));
   
   XLXI_131 : BUF
      port map (I=>DIFF(31),
                O=>XLXN_434(1));
   
end BEHAVIORAL;


