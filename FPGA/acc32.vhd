--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_12948_64
-- /___/   /\     Timestamp : 03/08/2021 12:32:14
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity acc32 is
   port ( ADD : in    std_logic; 
          B   : in    std_logic_vector (31 downto 0); 
          C   : in    std_logic; 
          CE  : in    std_logic; 
          CI  : in    std_logic; 
          D   : in    std_logic_vector (31 downto 0); 
          L   : in    std_logic; 
          R   : in    std_logic; 
          CO  : out   std_logic; 
          OFL : out   std_logic; 
          Q   : out   std_logic_vector (31 downto 0));
end acc32;

architecture BEHAVIORAL of acc32 is
begin
process(C)
   variable adsu_tmp : STD_LOGIC_VECTOR(32 downto 0);
   variable q_tmp    : STD_LOGIC;
   begin
     if (C'event and C ='1') then
       if (R='1') then
        adsu_tmp := (others => '0');
        q_tmp := '0';
       elsif (L='1') then
         adsu_tmp(31 downto 0) := D;
       elsif (CE='1') then 
         q_tmp := adsu_tmp(31);
         if(ADD = '1') then
           adsu_tmp := conv_std_logic_vector((conv_integer(adsu_tmp(31 downto 0)) + conv_integer(B) + conv_integer(CI)),33);
         else
           adsu_tmp := conv_std_logic_vector((conv_integer(adsu_tmp(31 downto 0)) - conv_integer(not CI) - conv_integer(B)),33);
         end if;
       end if;
     end if;

   Q <= adsu_tmp(31 downto 0);

   if (ADD='1') then
     CO <= adsu_tmp(32);
     OFL <=  ( q_tmp and B(31) and (not adsu_tmp(31)) ) or ( (not q_tmp) and (not B(31)) and adsu_tmp(31) );  
   else
     CO <= not adsu_tmp(32);
     OFL <=  ( q_tmp and (not B(31)) and (not adsu_tmp(31)) ) or ( (not q_tmp) and B(31) and adsu_tmp(31) );  
   end if;

   end process;
end BEHAVIORAL;


