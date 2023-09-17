--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_17236_41
-- /___/   /\     Timestamp : 03/29/2021 07:56:26
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

entity comp32 is
   port ( A  : in    std_logic_vector (31 downto 0); 
          B  : in    std_logic_vector (31 downto 0); 
          GT : out   std_logic);
end comp32;

architecture BEHAVIORAL of comp32 is
begin
 GT <= '1' when ( A > B ) else '0';

end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_comp32 of  comp32 is
   for BEHAVIORAL
   end for;
end CFG_comp32;
-- synopsys translate_on

