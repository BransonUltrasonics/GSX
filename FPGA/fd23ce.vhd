--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_17236_42
-- /___/   /\     Timestamp : 03/29/2021 08:00:05
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity fd23ce is
   port ( C   : in    std_logic; 
          CE  : in    std_logic; 
          CLR : in    std_logic; 
          D_B : in    std_logic_vector (23 downto 0); 
          Q_B : out   std_logic_vector (23 downto 0));
end fd23ce;

architecture BEHAVIORAL of fd23ce is
begin
PROCESS(C,CLR)
BEGIN
if (CLR='1') then
    Q_B<= (others=>'0');
  elsif (C'event and C = '1') then
    if (CE='1') then 
     Q_B<=D_B;
    end if;
  end if;
 END PROCESS;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_fd23ce of  fd23ce is
   for BEHAVIORAL
   end for;
end CFG_fd23ce;
-- synopsys translate_on

