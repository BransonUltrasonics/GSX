--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_12948_65
-- /___/   /\     Timestamp : 03/08/2021 13:43:56
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

entity comp23clk is
   port ( A  : in    std_logic_vector (23 downto 0):=(OTHERS => '0'); 
          B  : in    std_logic_vector (23 downto 0):=(OTHERS => 'U'); 
          C  : in    std_logic := 'U'; 
          EQ : out   std_logic := '0');
end comp23clk;

architecture BEHAVIORAL of comp23clk is
SIGNAL SA: std_logic_vector(23 downto 0);
SIGNAL SB: std_logic_vector (23 downto 0);
begin
SA<=A;
SB<=B;
	PROCESS(C)
		BEGIN
			IF C'Event and C = '1' THEN
				IF (SA=SB) THEN
					EQ<='1';
				ELSE
					EQ<='0';
				END IF;
			END IF;
		END PROCESS;
end BEHAVIORAL;


