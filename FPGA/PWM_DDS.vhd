----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:01:00 07/14/2021 
-- Design Name: 
-- Module Name:    PWM_DDS - Behavioral 
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
Use IEEE.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity PWM_DDS is Port (
	CLK_DDS		: in	STD_LOGIC;
	CLK_8			: in STD_LOGIC;
	SONICS_ON    : in STD_LOGIC;
	X_RST			: in	STD_LOGIC;
	Pwm_Val		: in STD_LOGIC_VECTOR(15 DOWNTO 0);
	Freq_Val		: in STD_LOGIC_VECTOR(23 DOWNTO 0);
	PhaseA		: out 	STD_LOGIC;
	PhaseB		: out 	STD_LOGIC;
	Motion		: out 	STD_LOGIC;
	Quad			: out 	STD_LOGIC
	);
end PWM_DDS;

architecture Behavioral of PWM_DDS is

SIGNAL PhaseAcc	: STD_LOGIC_VECTOR (31 downto 0);
SIGNAL AmpReg		: STD_LOGIC_VECTOR (31 downto 0);
SIGNAL FreqReg    : STD_LOGIC_VECTOR (23 DOWNTO 0);
SIGNAL PhaseAReg	: STD_LOGIC_VECTOR (31 downto 0);
SIGNAL PhaseBReg	: STD_LOGIC_VECTOR (31 downto 0);

SIGNAL CLK_25MHZ_LAST_1	: STD_LOGIC;
SIGNAL CLK_25MHZ_LAST_2 : STD_LOGIC;
SIGNAL CLK_25MHZ_LAST_3 : STD_LOGIC;
SIGNAL CLK_25MHZ_POS_STRB: STD_LOGIC;
SIGNAL CLK_25MHZ_NEG_STRB: STD_LOGIC;

SIGNAL FreqLatch	: STD_LOGIC;

--Shift Register 4
SIGNAL SR_Q : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL SR_SLI: STD_LOGIC;

begin


p_DDS_PWM_LATCH: process(X_RST,CLK_DDS,CLK_8)
	begin
		if (X_RST ='0') then
			AmpReg <= (OTHERS=>'0');
			FreqReg <= (OTHERS=> '0');
			PhaseAcc <= (OTHERS => '0');
			CLK_25MHZ_LAST_1 <= '0';
			CLK_25MHZ_LAST_2 <= '0';
			CLK_25MHZ_LAST_3 <= '0';
			CLK_25MHZ_POS_STRB <= '0';
			CLK_25MHZ_NEG_STRB<= '0';
		elsif (rising_edge(CLK_DDS)) then
		
			--sync to CLK_DDS
				CLK_25MHZ_LAST_3 <= CLK_25MHZ_LAST_2;
				CLK_25MHZ_LAST_2 <= CLK_25MHZ_LAST_1;
				CLK_25MHZ_LAST_1 <= CLK_8;
				
				IF ((CLK_25MHZ_LAST_2 = '1') AND (CLK_25MHZ_LAST_3 = '0')) THEN
                CLK_25MHZ_POS_STRB <= '1';                           --Strobe
            ELSIF ((CLK_25MHZ_LAST_2 = '0') AND (CLK_25MHZ_LAST_3 = '1')) THEN
                CLK_25MHZ_NEG_STRB <= '1';                           --Strobe
            END IF;
				
				IF (CLK_25MHZ_POS_STRB = '1') THEN
                CLK_25MHZ_POS_STRB <= '0';
						--PWM LATCH
							IF((PhaseAReg(31)='1') AND 
								(PhaseBReg(31)='1')) THEN
									AmpReg(31 DOWNTO 30) <= "00";
									AmpReg(29 DOWNTO 14)<=Pwm_Val;
									AmpReg(13 DOWNTO 0) <= (OTHERS=>'0');
							END IF;
						--FREQ LATCH
							IF((FreqLatch = '1') OR 
								(FreqReg = X"00000000")) THEN 
									FreqReg <= Freq_Val;
							END IF;
				END IF;
			PhaseAcc <= PhaseAcc + (X"00" & FreqReg); --Accumulate Phase
			IF((PhaseAcc> X"40000000") AND
				(PhaseAcc < X"C0000000")) THEN
					SR_SLI <= '1';
			ELSE
					SR_SLI <= '0';
			END IF;
			
		end if;
	end process;
	
	--Shift Register process
	FreqLatch <= SR_Q(0) AND (NOT SR_Q(1));
	
	p_ShiftReg : process (X_RST,CLK_8)
	
	BEGIN
		if (X_RST ='0') then 
			SR_Q <= (OTHERS => '0');
		ELSIF(Rising_Edge(CLK_8)) THEN
			SR_Q(3 DOWNTO 0) <= SR_Q(2 DOWNTO 0) & SR_SLI;
		END IF;
	
	END PROCESS;
												  
	--Motion <= PhaseReg(27);
	Motion <= not PhaseAcc(31);
	Quad <= PhaseAcc(31) xor PhaseAcc(30);

	--32-bit DDS Resolution fix and now add all 16-bits to to PWM
	PhaseAReg <= PhaseAcc + AmpReg;
   PhaseBReg <= PhaseAcc - AmpReg;

	with SONICS_ON select PhaseA <=
										  PhaseAReg(31) when '1',
										'0' when others;--drive mosfet in any other case

	with SONICS_ON select PhaseB <=
										 PhaseBReg(31) when '1',
										'0' when others;--drive mosfet in any other case

end Behavioral;

