--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_22896_79
-- /___/   /\     Timestamp : 03/30/2021 15:47:33
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

entity or2clk is
   port ( C  : in    std_logic; 
          I0 : in    std_logic; 
          I1 : in    std_logic; 
          O  : out   std_logic);
end or2clk;

architecture BEHAVIORAL of or2clk is
begin
PROCESS(C) 
		BEGIN
			IF(C'event and C='1') THEN
				O<= I0 OR I1;
			END IF;
	END PROCESS;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_or2clk of  or2clk is
   for BEHAVIORAL
   end for;
end CFG_or2clk;
-- synopsys translate_on

