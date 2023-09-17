----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:41:29 03/16/2021 
-- Design Name: 
-- Module Name:    DSPI_ADC - Structure 
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

entity DSPI_ADC is
 port (

        X_RST              : in    std_logic;
        CLK_M              : in    std_logic;
        CLK_8              : in    std_logic;
        CLK_1ms            : in    std_logic;

        ADC_ERROR          : out   std_logic;

        I_DSPI_CLK         : in    std_logic;
        I_DSPI_CS_ADC      : in    std_logic;
        I_DSPI_RXD         : in    std_logic;
        I_DSPI_TXD         : in    std_logic;
        I_PHASE_SIGN       : in    std_logic;

        Ampl_Val_Dspi      : out   std_logic_vector(7 downto 0);
        Phase_Val_Dspi     : out   std_logic_vector(15 downto 0);
        Curr_Val_Dspi      : out   std_logic_vector(7 downto 0);
        Pwr_Val_Dspi       : out   std_logic_vector(7 downto 0);

        FSYNC_LVL_VALID    : out   std_logic;
        FCURR_LVL_VALID    : out   std_logic

    );
end DSPI_ADC;

architecture Structure of DSPI_ADC is
-- Component Declarations


-- Signal Declarations


--    CONSTANT ci_WDOG_TIMEOUT_MS:    INTEGER RANGE 0 TO 7 := 5;

--    SIGNAL ADBUG_AMP_VAL_S13:       STD_LOGIC_VECTOR (13 DOWNTO 0);           --Temp. Signals, for AD-BUG, (>=AD15)
--    SIGNAL ADBUG_PWR_VAL_S13:       STD_LOGIC_VECTOR (13 DOWNTO 0);           --Temp. Signals, for AD-BUG, (>=AD15)
--    SIGNAL ADBUG_CURR_VAL_S13:      STD_LOGIC_VECTOR (13 DOWNTO 0);           --Temp. Signals, for AD-BUG, (>=AD15)

--    SIGNAL ADC_ERROR_REG:           STD_LOGIC;

--    SIGNAL CLK_1ms_LAST_1:          STD_LOGIC;
--    SIGNAL CLK_1ms_LAST_2:          STD_LOGIC;
--    SIGNAL CLK_1ms_LAST_3:          STD_LOGIC;
--    SIGNAL CLK_1ms_POS_STRB:        STD_LOGIC;
--    SIGNAL CLK_1ms_NEG_STRB:        STD_LOGIC;

    SIGNAL DSPI_CLK_DEB:              STD_LOGIC;
    SIGNAL i_DSPI_CLK_DEB_CNT:        INTEGER RANGE 0 TO 7;

    SIGNAL DSPI_N_CS_ADC_DEB:         STD_LOGIC;
    SIGNAL DSPI_CS_ADC_DEB:           STD_LOGIC;
    SIGNAL i_DSPI_CS_ADC_DEB_CNT:     INTEGER RANGE 0 TO 7;
--    SIGNAL DSPI_CS_ADC_DEB_LAST_1:    STD_LOGIC;
--    SIGNAL DSPI_CS_ADC_DEB_LAST_2:    STD_LOGIC;
--    SIGNAL DSPI_CS_ADC_DEB_LAST_3:    STD_LOGIC;
--    SIGNAL DSPI_CS_ADC_DEB_POS_STRB:  STD_LOGIC;

    SIGNAL DSPI_MISO_ADC_DEB:         STD_LOGIC;
    SIGNAL i_DSPI_RXD_ADC_DEB_CNT:    INTEGER RANGE 0 TO 7;

    SIGNAL DSPI_MOSI_ADC_DEB:         STD_LOGIC;
    SIGNAL i_DSPI_TXD_ADC_DEB_CNT:    INTEGER RANGE 0 TO 7;

begin

-----


--READ only, (D)SPI ADC                                                         -- ??? + Impl. Frequ-High Error @ CLK ???
--AD7682, Sequence Mode, RAC
--Baudrate = ~ 24MBit/s -> Tclk =  41.6ns --> Debounce with 10ns
--CPOL=0, CPHA=0
--MSBit/MSByte first
--Bit length = 16 Bit


-- Debounce I_DSPI_CLK
    p_dspi_clk_deb: PROCESS (X_RST, CLK_M, I_DSPI_CLK)
    BEGIN
        IF (X_RST='0') THEN
            DSPI_CLK_DEB <= '0';                                    -- IDLE
            i_DSPI_CLK_DEB_CNT <= 0;
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (I_DSPI_CLK = DSPI_CLK_DEB) THEN
                i_DSPI_CLK_DEB_CNT <= 0;
            ELSE
                i_DSPI_CLK_DEB_CNT <= i_DSPI_CLK_DEB_CNT+1;
            END IF;
            IF(i_DSPI_CLK_DEB_CNT >= 10) THEN                       -- 50ns     -- ???
                DSPI_CLK_DEB <= I_DSPI_CLK;
            END IF;
        END IF;
    END PROCESS p_dspi_clk_deb;


-- Debounce DSPI_N_CS_ADC / I_DSPI_CS_ADC
    p_dspi_cs_adc_deb: PROCESS (X_RST, CLK_M, I_DSPI_CS_ADC)
    BEGIN
        IF (X_RST='0') THEN
            DSPI_N_CS_ADC_DEB <= '1';                               -- IDLE
            DSPI_CS_ADC_DEB <= '0';
            i_DSPI_CS_ADC_DEB_CNT <= 0;
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (I_DSPI_CS_ADC = DSPI_N_CS_ADC_DEB) THEN
                i_DSPI_CS_ADC_DEB_CNT <= 0;
            ELSE
                i_DSPI_CS_ADC_DEB_CNT <= i_DSPI_CS_ADC_DEB_CNT+1;
            END IF;
            IF ((i_DSPI_CS_ADC_DEB_CNT >= 2) AND                    -- 10ns     -- ???
                (DSPI_N_CS_ADC_DEB /= I_DSPI_CS_ADC)) THEN
                DSPI_N_CS_ADC_DEB <= I_DSPI_CS_ADC;
                DSPI_CS_ADC_DEB <= NOT I_DSPI_CS_ADC;               -- Invert!
            END IF;
        END IF;
    END PROCESS p_dspi_cs_adc_deb;


-- Debounce I_DSPI_CS_RXD (MISO)
    p_dspi_rxd_adc_deb: PROCESS (X_RST, CLK_M, I_DSPI_RXD)
    BEGIN
        IF (X_RST='0') THEN
            DSPI_MISO_ADC_DEB <= '0';                               -- IDLE
            i_DSPI_RXD_ADC_DEB_CNT <= 0;
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (I_DSPI_RXD = DSPI_MISO_ADC_DEB) THEN
                i_DSPI_RXD_ADC_DEB_CNT <= 0;
            ELSE
                i_DSPI_RXD_ADC_DEB_CNT <= i_DSPI_RXD_ADC_DEB_CNT+1;
            END IF;
            IF(i_DSPI_RXD_ADC_DEB_CNT >= 2) THEN                    -- 10ns     -- ???
                DSPI_MISO_ADC_DEB <= I_DSPI_RXD;
            END IF;
        END IF;
    END PROCESS p_dspi_rxd_adc_deb;


-- Debounce I_DSPI_CS_TXD (MOSI)
    p_dspi_txd_adc_deb: PROCESS (X_RST, CLK_M, I_DSPI_TXD)
    BEGIN
        IF (X_RST='0') THEN
            DSPI_MOSI_ADC_DEB <= '0';                               -- IDLE
            i_DSPI_TXD_ADC_DEB_CNT <= 0;
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (I_DSPI_TXD = DSPI_MOSI_ADC_DEB) THEN
                i_DSPI_TXD_ADC_DEB_CNT <= 0;
            ELSE
                i_DSPI_TXD_ADC_DEB_CNT <= i_DSPI_TXD_ADC_DEB_CNT+1;
            END IF;
            IF(i_DSPI_TXD_ADC_DEB_CNT >= 2) THEN                    -- 10ns     -- ???
                DSPI_MOSI_ADC_DEB <= I_DSPI_TXD;
            END IF;
        END IF;
    END PROCESS p_dspi_txd_adc_deb;



--...



-- Assign Output Values, All Zero...                                            --Implement new ADC !!!
    ADC_ERROR <= '0';
    Ampl_Val_Dspi <= (OTHERS => '0');
    Phase_Val_Dspi <= (OTHERS => '0');
    Curr_Val_Dspi <= (OTHERS => '0');
    Pwr_Val_Dspi <= (OTHERS => '0');
    FSYNC_LVL_VALID <= '0';
    FCURR_LVL_VALID <= '0';


-----

end Structure;

