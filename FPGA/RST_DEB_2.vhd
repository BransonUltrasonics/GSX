----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:08:46 03/24/2021 
-- Design Name: 
-- Module Name:    RST_DEB_2 - Structure 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity RST_DEB_2 is
    Port ( CLK_25MHZ : in  STD_LOGIC;
           X_RST_IN_2 : in  STD_LOGIC;
           X_RST_OUT_2 : out  STD_LOGIC);
end RST_DEB_2;

architecture Structure of RST_DEB_2 is

-- Component Declarations

--Signal Declarations

SIGNAL X_RST_DEB:		STD_LOGIC;
SIGNAL i_X_RST_IN_DEB_CNT:	INTEGER RANGE 0 TO 15;


begin

--Debounce X_RST_IN

p_xrstin_deb: PROCESS (CLK_25MHZ)
BEGIN
	IF(Rising_Edge(CLK_25MHZ)) THEN
		IF(X_RST_IN_2=X_RST_DEB) THEN
			i_X_RST_IN_DEB_CNT<=0;
		ELSE
			i_X_RST_IN_DEB_CNT<=i_X_RST_IN_DEB_CNT+1;
		END IF;
		IF(i_X_RST_IN_DEB_CNT>=5) THEN  --250ns
			X_RST_DEB<=X_RST_IN_2;
		END IF;
	END IF;
END PROCESS p_xrstin_deb;

--Assign Output
X_RST_OUT_2<=X_RST_DEB;


end Structure;

