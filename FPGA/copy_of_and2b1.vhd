--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_12948_66
-- /___/   /\     Timestamp : 03/08/2021 15:26:46
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

entity copy_of_and2b1 is
   port ( c  : in    std_logic; 
          I0 : in    std_logic; 
          I1 : in    std_logic; 
          O  : out   std_logic);
end copy_of_and2b1;

architecture BEHAVIORAL of copy_of_and2b1 is
begin
  PROCESS(c)
  BEGIN
	IF c'Event and c='1' THEN
		O <= NOT(I0) AND I1;
	END IF;
	END PROCESS;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_copy_of_and2b1 of  copy_of_and2b1 is
   for BEHAVIORAL
   end for;
end CFG_copy_of_and2b1;
-- synopsys translate_on

