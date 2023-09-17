--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : 
--  /   /         Filename : xil_24056_43
-- /___/   /\     Timestamp : 04/02/2021 12:00:02
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

entity fjkpc is
generic(
    INIT : bit := '1'
    );
   port ( C   : in    std_logic; 
          CLR : in    std_logic; 
          J   : in    std_logic; 
          K   : in    std_logic; 
          PRE : in    std_logic; 
          Q   : out   std_logic);
end fjkpc;

architecture BEHAVIORAL of fjkpc is
signal q_tmp : std_logic := TO_X01(INIT);
begin

process(C, PRE,CLR)
begin
	if(CLR='1') then
		q_tmp<='0';
  elsif (PRE='1') then
    q_tmp <= '1';
  elsif (C'event and C = '1') then
    if(J='0') then
      if(K='1') then
      q_tmp <= '0';
    end if;
    else
      if(K='0') then
      q_tmp <= '1';
      else
      q_tmp <= not q_tmp;
      end if;
    end if;
  end if;  
  end process;
  Q <= q_tmp;
end BEHAVIORAL;

-- synopsys translate_off
configuration CFG_fjkpc of  fjkpc is
   for BEHAVIORAL
   end for;
end CFG_fjkpc;
-- synopsys translate_on
