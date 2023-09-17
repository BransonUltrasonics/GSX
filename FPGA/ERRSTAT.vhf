--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : ERRSTAT.vhf
-- /___/   /\     Timestamp : 07/17/2021 11:41:11
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -sympath "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ipcore_dir" -intstyle ise -family spartan3a -flat -suppress -vhdl "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ERRSTAT.vhf" -w "C:/Users/76053/Documents/Branson/Proyectos/Next Generation/FPGA/GSX_FPGA/ERRSTAT.sch"
--Design Name: ERRSTAT
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

entity ERRSTAT is
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
end ERRSTAT;

architecture BEHAVIORAL of ERRSTAT is
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


