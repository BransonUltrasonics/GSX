--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_14888_23
-- /___/   /\     Timestamp : 03/17/2021 09:44:17
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

entity fd2c is
   port ( C   : in    std_logic; 
          CLR : in    std_logic; 
          D0  : in    std_logic; 
          D1  : in    std_logic; 
          Q0  : out   std_logic; 
          Q1  : out   std_logic);
end fd2c;

architecture BEHAVIORAL of fd2c is
begin

process(C,CLR)
begin
	IF(CLR='1') THEN
		Q0<='0';
		Q1<='0';
	ELSIF(C'event and C='1') THEN
		Q0<=D0;
		Q1<=D1;
	END IF;
end process;
end BEHAVIORAL;


