--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_12948_68
-- /___/   /\     Timestamp : 03/10/2021 08:10:13
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity add32 is
   port ( A   : in    std_logic_vector (31 downto 0); 
          ADD : in    std_logic; 
          B   : in    std_logic_vector (31 downto 0); 
          C   : inout    std_logic; 
          S   : out   std_logic_vector (31 downto 0));
end add32;

architecture BEHAVIORAL of add32 is
SIGNAL SA : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL SB : STD_LOGIC_VECTOR(31 DOWNTO 0);
begin
SA(31 DOWNTO 0) <= A;
--SA(32)<='0';
SB(31 DOWNTO 0) <=B;
--SB(32)<='0';

PROCESS(C)
	VARIABLE SS: STD_LOGIC_VECTOR(32 DOWNTO 0);
	BEGIN
		IF (C'event and C='1') THEN
			IF(ADD='1') THEN
				SS:= conv_std_logic_vector(conv_integer(SA) + conv_integer(SB),32);
			ELSE
				SS:= conv_std_logic_vector(conv_integer(SA) - conv_integer(SB),32);
			END IF;
		 S(31 DOWNTO 0) <= SS(31 DOWNTO 0);
		END IF;
END PROCESS;

end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_add32 of  add32 is
   for BEHAVIORAL
   end for;
end CFG_add32;
-- synopsys translate_on

