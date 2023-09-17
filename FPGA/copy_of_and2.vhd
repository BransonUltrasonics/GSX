--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_18764_41
-- /___/   /\     Timestamp : 03/26/2021 16:26:33
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

entity copy_of_and2 is
   port ( C  : in    std_logic; 
          I0 : in    std_logic; 
          I1 : in    std_logic; 
          O  : out   std_logic);
end copy_of_and2;

architecture BEHAVIORAL of copy_of_and2 is
begin

	PROCESS(C) 
		BEGIN
			IF(C'event and C='1') THEN
				O<= I0 AND I1;
			END IF;
	END PROCESS;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_copy_of_and2 of  copy_of_and2 is
   for BEHAVIORAL
   end for;
end CFG_copy_of_and2;
-- synopsys translate_on

