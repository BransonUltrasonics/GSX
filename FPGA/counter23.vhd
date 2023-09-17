--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_23296_114
-- /___/   /\     Timestamp : 03/30/2021 10:48:48
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: 
--

library ieee;
use ieee.std_logic_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity counter23 is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          CEO : out   std_logic; 
          Q   : out   std_logic_vector (23 downto 0); 
          TC  : out   std_logic);
end counter23;

architecture BEHAVIORAL of counter23 is
signal COUNT : STD_LOGIC_VECTOR(23 downto 0) := (others => '0');
  constant TERMINAL_COUNT : STD_LOGIC_VECTOR(23 downto 0) := (others => '1');
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

TC  <= '1' when (COUNT = TERMINAL_COUNT) else '0';
CEO <= '1' when ((COUNT = TERMINAL_COUNT) and CE='1') else '0';
Q   <= COUNT;

end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_counter23 of  counter23 is
   for BEHAVIORAL
   end for;
end CFG_counter23;
-- synopsys translate_on

