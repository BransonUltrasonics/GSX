--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_22896_78
-- /___/   /\     Timestamp : 03/30/2021 14:45:46
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

entity copy_of_or2 is
   port ( C  : in    std_logic; 
          I0 : in    std_logic; 
          I1 : in    std_logic; 
          O  : out   std_logic);
end copy_of_or2;

architecture BEHAVIORAL of copy_of_or2 is
begin
 PROCESS(C)
  BEGIN
	IF C'Event and C='1' THEN
		O <= I0 OR I1;
	END IF;
	END PROCESS;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_copy_of_or2 of  copy_of_or2 is
   for BEHAVIORAL
   end for;
end CFG_copy_of_or2;
-- synopsys translate_on

