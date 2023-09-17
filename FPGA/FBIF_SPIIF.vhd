----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:44:59 03/11/2021 
-- Design Name: 
-- Module Name:    FBIF_SPIIF - Structure 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity FBIF_SPIIF is port
(
	  X_RST                    : in    std_logic;
     CLK_M                    : in    std_logic;
     CLK_4                    : in    std_logic;
     CLK_8                    : in    std_logic;
     CLK_1ms                  : in    std_logic;	

     TYPE_BD                  : in    std_logic_vector(7 downto 0);
     HW_VERS_BD               : in    std_logic_vector(7 downto 0);

     SPI_CLK                  : in    std_logic;
     SPI_N_CS                 : in    std_logic;
     SPI_I_MOSI               : in    std_logic;
     SPI_O_MISO               : out   std_logic;

     BTM                      : out   std_logic;
     BTM_GPO                  : out   std_logic_vector(7 downto 0);
     BTM_PWMA                 : out   std_logic;
     BTM_PWMB                 : out   std_logic;

     GPI                      : in    std_logic_vector(7 downto 0);
     GPO                      : out   std_logic_vector(7 downto 0);

     ADC_ERROR                : in    std_logic;
     FPGA_RUN                 : out   std_logic;


     WCYC_P_CtrlHdx           : out   std_logic_vector(15 downto 0);            --Define final Parameternames here...
     WCYC_P_FrequValIn        : out   std_logic_vector(23 downto 0);
     WCYC_P_PhaseValIn        : out   std_logic_vector(15 downto 0);
     WCYC_P_PWMVal            : out   std_logic_vector(15 downto 0);
   

     RCYC_S_StatHdx           : in   std_logic_vector(15 downto 0);             --Define final Parameternames here...
     RCYC_S_FrequValOut       : in   std_logic_vector(23 downto 0);
     RCYC_S_PhaseValOut       : in   std_logic_vector(15 downto 0);

     FBIF_TEST_1              : out   std_logic;                                --TEST
     FBIF_TEST_2              : out   std_logic;                                --TEST

     FPGA_CTRL_TEST           : out   std_logic_vector(15 downto 0);            --TEST
     FPGA_STAT_TEST           : out   std_logic_vector(15 downto 0)             --TEST

   );
end FBIF_SPIIF;

architecture Structure of FBIF_SPIIF is
-- Component Declarations


-- Signal Declarations


-- Everytime at version change, this lines have to change accordingly:
-- FPGA Version String ASCII "2.2.0"
    CONSTANT FPGA_VERSIONx_REG_R:   STD_LOGIC_VECTOR (63 DOWNTO 0) :=
                                    X"322E322E30000000";                        --Change, at each new Version release !

-- Define HW-Version of T1-Board, for which this FPGA Version is running:
-- T1 = DCX, 1283Bd.
-- HW-Version: "0x05"
    CONSTANT HW_VERS_BD_T1_COMPARE: STD_LOGIC_VECTOR (7 DOWNTO 0) :=
                                    "00000101";

-- Define HW-Version of T2-Board, for which this FPGA Version is running:
-- T2 = Next Gen. PACC, 1310Bd.
-- HW-Version: "0x00"
    CONSTANT HW_VERS_BD_T2_COMPARE_1310Bd: STD_LOGIC_VECTOR (7 DOWNTO 0) :=
                                  "00000000";
-- pcb 1012024-Power Controller Bd
--HW-Version: "0x01"
   CONSTANT HW_VERS_BD_T2_COMPARE_024Bd: STD_LOGIC_VECTOR (7 DOWNTO 0) :=
                                  "00000001";

    CONSTANT ci_WDOG_TIMEOUT_MS:    INTEGER RANGE 0 TO 7 := 5;

    SIGNAL FPGA_STAT_RUN_COUNT: INTEGER RANGE 0 TO 300000:=0;
    CONSTANT MAX_FPGA_STAT_RUN_DELAY: INTEGER RANGE 0 TO 300000:= 250000;            --10 Miliseconds for a 25MHz Clock (CLK8)
	 SIGNAL FPGA_RUN_EN:					STD_LOGIC;

    SIGNAL SAMPLE_HW_VERSION:       STD_LOGIC;
    SIGNAL HW_VERS_ERROR:           STD_LOGIC;

    SIGNAL CLK_1ms_LAST_1:          STD_LOGIC;
    SIGNAL CLK_1ms_LAST_2:          STD_LOGIC;
    SIGNAL CLK_1ms_LAST_3:          STD_LOGIC;
    SIGNAL CLK_1ms_POS_STRB:        STD_LOGIC;
    SIGNAL CLK_1ms_NEG_STRB:        STD_LOGIC;
	 


    SIGNAL WDOG_PWM:                STD_LOGIC;
    SIGNAL WDOG_ACK_PWM:            STD_LOGIC;
    SIGNAL i_WDOG_CTR_PWM:          INTEGER RANGE 0 TO 7;
    SIGNAL WDOG_FREQU:              STD_LOGIC;
    SIGNAL WDOG_ACK_FREQU:          STD_LOGIC;
    SIGNAL i_WDOG_CTR_FREQU:        INTEGER RANGE 0 TO 7;
    SIGNAL WDOG_PWM_WCYC_PARA_IDX:  INTEGER RANGE 0 TO 31 :=
                                    10;
    SIGNAL WDOG_FREQU_WCYC_PARA_IDX:INTEGER RANGE 0 TO 31 :=
                                    8;

    SIGNAL DDS_STOP:                STD_LOGIC;
    SIGNAL PWM_VAL_REG_EQ_ZERO:     STD_LOGIC;
    SIGNAL SONIC_SAFE:              STD_LOGIC;
    SIGNAL INITIALIZED:             STD_LOGIC;
    SIGNAL WDOG_ERROR:              STD_LOGIC;
	 
	 SIGNAL PHASE_SIGN:					STD_LOGIC;
	 
	 SIGNAL OL_CURRENT:					STD_LOGIC;
	 SIGNAL OL_VOLTAGE:					STD_LOGIC;
	 SIGNAL OL_POWER:						STD_LOGIC;
	 SIGNAL OL_TEMP:						STD_LOGIC;
	 SIGNAL OL_FUNCTION:					STD_LOGIC;
	 SIGNAL OL_LATCH:  					STD_LOGIC_VECTOR (4 downto 0):="11111";
	 SIGNAL OLD_OL_LATCH:  				STD_LOGIC_VECTOR (4 downto 0):="11111";


    SIGNAL FB_A_INT:                STD_LOGIC_VECTOR (11 DOWNTO 0);
    SIGNAL FB_A_INT_IS_WA:          STD_LOGIC;
    SIGNAL FB_A_INT_IS_DWA:         STD_LOGIC;
    SIGNAL FB_A_INT_IS_HWA:         STD_LOGIC;
    SIGNAL FB_A_INT_IS_LWA:         STD_LOGIC;
    SIGNAL FB_A_INT_WA:             STD_LOGIC_VECTOR (10 DOWNTO 0);
    SIGNAL FB_A_INT_DWA:            STD_LOGIC_VECTOR (9 DOWNTO 0);
    SIGNAL FB_A_INT_LW_CHECK:       STD_LOGIC_VECTOR (11 DOWNTO 0);
    SIGNAL FB_A_INT_LW_CHECK_RD:    STD_LOGIC_VECTOR (11 DOWNTO 0);
    SIGNAL FB_A_INT_LW_CHECK_WR:    STD_LOGIC_VECTOR (11 DOWNTO 0);

    SIGNAL FB_SPI_RD_D_DW:          STD_LOGIC_VECTOR (31 DOWNTO 0);
    ALIAS FB_SPI_RD_D_DW_LW:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    FB_SPI_RD_D_DW (15 DOWNTO 0);
    ALIAS FB_SPI_RD_D_DW_LW_LB:     STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_RD_D_DW (7 DOWNTO 0);
    ALIAS FB_SPI_RD_D_DW_LW_HB:     STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_RD_D_DW (15 DOWNTO 8);
    ALIAS FB_SPI_RD_D_DW_HW:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    FB_SPI_RD_D_DW (31 DOWNTO 16);
    SIGNAL FB_SPI_WR_D_W:           STD_LOGIC_VECTOR (15 DOWNTO 0);
    SIGNAL FB_SPI_WR_D_DW:          STD_LOGIC_VECTOR (31 DOWNTO 0);
    ALIAS FB_SPI_WR_D_DW_LW:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    FB_SPI_WR_D_DW (15 DOWNTO 0);
    ALIAS FB_SPI_WR_D_DW_LW_LB:     STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_WR_D_DW (7 DOWNTO 0);
    ALIAS FB_SPI_WR_D_DW_LW_HB:     STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_WR_D_DW (15 DOWNTO 8);
    ALIAS FB_SPI_WR_D_DW_HW:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    FB_SPI_WR_D_DW (31 DOWNTO 16);

--+SPI                                                                          SPI

    SIGNAL SPI_CLK_DEB:             STD_LOGIC;
    SIGNAL i_SPI_CLK_DEB_CNT:       INTEGER RANGE 0 TO 3;
    SIGNAL i_SPI_CLK_F_H_ERR_CNT:   INTEGER RANGE 0 TO 15;
    SIGNAL SPI_CLK_F_H_ERR_CNT_RST: STD_LOGIC;
    SIGNAL SPI_CLK_F_H_ERR:         STD_LOGIC;

    SIGNAL SPI_N_CS_DEB:            STD_LOGIC;
    SIGNAL SPI_CS_DEB:              STD_LOGIC;
    SIGNAL i_SPI_CS_DEB_CNT:        INTEGER RANGE 0 TO 3;

    SIGNAL SPI_MOSI_RD_START:       STD_LOGIC;
    SIGNAL SPI_MOSI_RD_START_LAST1: STD_LOGIC;
    SIGNAL SPI_MOSI_RD_START_LAST2: STD_LOGIC;

    SIGNAL SPI_CS_DEB_LAST1:        STD_LOGIC;
    SIGNAL SPI_CS_DEB_LAST2:        STD_LOGIC;
    SIGNAL SPI_CS_DEB_LAST3:        STD_LOGIC;

    SIGNAL SPI_START_CLEAR:         STD_LOGIC;
    SIGNAL SPI_START:               STD_LOGIC;
    SIGNAL SPI_START_LAST1:         STD_LOGIC;
    SIGNAL SPI_START_LAST2:         STD_LOGIC;
    SIGNAL SPI_OK:                  STD_LOGIC;
    SIGNAL SPI_OK_LAST1:            STD_LOGIC;
    SIGNAL SPI_OK_LAST2:            STD_LOGIC;
    SIGNAL SPI_ERROR_CLK_F_H:       STD_LOGIC;
    SIGNAL SPI_ERROR_CLK_F_H_LAST1: STD_LOGIC;
    SIGNAL SPI_ERROR_CLK_F_H_LAST2: STD_LOGIC;
    SIGNAL SPI_ERROR_BIT_ORUN:      STD_LOGIC;
    SIGNAL SPI_ERROR_BIT_ORUN_LAST1:STD_LOGIC;
    SIGNAL SPI_ERROR_BIT_ORUN_LAST2:STD_LOGIC;

    SIGNAL SPI_ERROR_BUR:           STD_LOGIC;
    SIGNAL SPI_ERROR_BOR:           STD_LOGIC;
    SIGNAL SPI_ERROR_L_WPTY:        STD_LOGIC;
    SIGNAL SPI_ERROR_L_WA:          STD_LOGIC;
    SIGNAL SPI_ERROR_L_RA:          STD_LOGIC;

    CONSTANT i_SPI_BIT_IDX_MAX:     INTEGER RANGE 0 TO 63 := 63;
    SIGNAL i_SPI_BIT_IDX_CNT:       INTEGER RANGE 0 TO 63 := 63;

    SIGNAL SPI_MOSI_DIN:            STD_LOGIC_VECTOR (63 DOWNTO 0);
    ALIAS SPI_MOSI_DIN_RD_ADR:      STD_LOGIC_VECTOR (11 DOWNTO 0) IS
                                    SPI_MOSI_DIN (63 DOWNTO 52);
    SIGNAL SPI_MOSI_REG_RD_ADR:     STD_LOGIC_VECTOR (11 DOWNTO 0);
    SIGNAL SPI_MOSI_REG:            STD_LOGIC_VECTOR (63 DOWNTO 0);
    ALIAS SPI_MOSI_REG_WR_ADR:      STD_LOGIC_VECTOR (11 DOWNTO 0) IS
                                    SPI_MOSI_REG (51 DOWNTO 40);
    ALIAS SPI_MOSI_REG_WPTY:        STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (39 DOWNTO 32);
    ALIAS SPI_MOSI_REG_WR_DATA:     STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    SPI_MOSI_REG (31 DOWNTO 0);
    ALIAS SPI_MISO_REG_WR_DATA_LW:  STD_LOGIC_VECTOR (15 DOWNTO 0) IS
												SPI_MOSI_REG(15 DOWNTO 0);
                                    --SPI_MOSI_REG_WR_DATA (15 DOWNTO 0);
    ALIAS SPI_MISO_REG_WR_DATA_HW:  STD_LOGIC_VECTOR (15 DOWNTO 0) IS
												SPI_MOSI_REG(31 DOWNTO 16);
                                    --SPI_MOSI_REG_WR_DATA (31 DOWNTO 16);

    ALIAS SPI_MOSI_REG_WPTY_BY7:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (63 DOWNTO 56);
    ALIAS SPI_MOSI_REG_WPTY_BY6:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (55 DOWNTO 48);
    ALIAS SPI_MOSI_REG_WPTY_BY5:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (47 DOWNTO 40);
    ALIAS SPI_MOSI_REG_WPTY_BY4_PT: STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (39 DOWNTO 32);
    ALIAS SPI_MOSI_REG_WPTY_BY3:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (31 DOWNTO 24);
    ALIAS SPI_MOSI_REG_WPTY_BY2:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (23 DOWNTO 16);
    ALIAS SPI_MOSI_REG_WPTY_BY1:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (15 DOWNTO 8);
    ALIAS SPI_MOSI_REG_WPTY_BY0:    STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MOSI_REG (7 DOWNTO 0);

    SIGNAL SPI_NEW_MOSI_VALID:      STD_LOGIC;                                  --SM WRITE...
    SIGNAL SPI_HDL_WPTY:            STD_LOGIC;
    SIGNAL SPI_HDL_WR_CHK_DMY_ADDR: STD_LOGIC;
    SIGNAL SPI_HDL_WR_START:        STD_LOGIC;
    SIGNAL SPI_HDL_WR:              STD_LOGIC;
    SIGNAL SPI_HDL_WR_SM_SR:        STD_LOGIC_VECTOR (16 DOWNTO 0);
    --... to/from Write engine
    SIGNAL SPI_WR_RD_ADR:           STD_LOGIC_VECTOR (11 DOWNTO 0);
    SIGNAL SPI_WR_D_W:              STD_LOGIC_VECTOR (15 DOWNTO 0);
    SIGNAL SPI_WRITE_SYNC:          STD_LOGIC;

    SIGNAL SPI_MISO_DOUT:           STD_LOGIC_VECTOR (63 DOWNTO 0);
    ALIAS SPI_MISO_DOUT_LD_ZERO:    STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    SPI_MISO_DOUT (63 DOWNTO 48);
    ALIAS SPI_MISO_DOUT_ERR_7_RA:   STD_LOGIC IS SPI_MISO_DOUT (47);
    ALIAS SPI_MISO_DOUT_ERR_6_WA:   STD_LOGIC IS SPI_MISO_DOUT (46);
    ALIAS SPI_MISO_DOUT_ERR_5:      STD_LOGIC IS SPI_MISO_DOUT (45);
    ALIAS SPI_MISO_DOUT_ERR_4_WPTY: STD_LOGIC IS SPI_MISO_DOUT (44);
    ALIAS SPI_MISO_DOUT_ERR_3:      STD_LOGIC IS SPI_MISO_DOUT (43);
    ALIAS SPI_MISO_DOUT_ERR_2:      STD_LOGIC IS SPI_MISO_DOUT (42);
    ALIAS SPI_MISO_DOUT_ERR_1_BOR:  STD_LOGIC IS SPI_MISO_DOUT (41);
    ALIAS SPI_MISO_DOUT_ERR_0_BUR:  STD_LOGIC IS SPI_MISO_DOUT (40);
    ALIAS SPI_MISO_REG_RPTY:        STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (39 DOWNTO 32);
    ALIAS SPI_MISO_REG_RD_DATA:     STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    SPI_MISO_DOUT (31 DOWNTO 0);
    ALIAS SPI_MISO_REG_RD_DATA_LW:  STD_LOGIC_VECTOR (15 DOWNTO 0) IS
												SPI_MISO_DOUT(15 DOWNTO 0);
    ALIAS SPI_MISO_REG_RD_DATA_HW:  STD_LOGIC_VECTOR (15 DOWNTO 0) IS
												SPI_MISO_DOUT(31 DOWNTO 16);

    ALIAS SPI_MISO_DOUT_RPTY_BY7:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (63 DOWNTO 56);
    ALIAS SPI_MISO_DOUT_RPTY_BY6:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (55 DOWNTO 48);
    ALIAS SPI_MISO_DOUT_RPTY_BY5:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (47 DOWNTO 40);
    ALIAS SPI_MISO_DOUT_RPTY_BY4_PT: STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (39 DOWNTO 32);
    ALIAS SPI_MISO_DOUT_RPTY_BY3:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (31 DOWNTO 24);
    ALIAS SPI_MISO_DOUT_RPTY_BY2:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (23 DOWNTO 16);
    ALIAS SPI_MISO_DOUT_RPTY_BY1:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (15 DOWNTO 8);
    ALIAS SPI_MISO_DOUT_RPTY_BY0:   STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    SPI_MISO_DOUT (7 DOWNTO 0);

    SIGNAL SPI_NEW_MOSI_RD_VALID:   STD_LOGIC;                                  --SM READ...
    SIGNAL SPI_HDL_RD_CHK_DMY_ADDR: STD_LOGIC;
    SIGNAL SPI_HDL_RD_START:        STD_LOGIC;
    SIGNAL SPI_HDL_RD:              STD_LOGIC;
    SIGNAL SPI_HDL_RD_SM_SR:        STD_LOGIC_VECTOR (12 DOWNTO 0);
    SIGNAL SPI_HDL_RPTY:            STD_LOGIC;
    --... to/from Read engine
    SIGNAL SPI_RD_D_W:              STD_LOGIC_VECTOR (15 DOWNTO 0);
    SIGNAL SPI_READ_SYNC:           STD_LOGIC;

    CONSTANT SPI_DMY_ADDR_NO_DW_MM: STD_LOGIC_VECTOR (11 DOWNTO 0) := X"FFC"; 
    CONSTANT SPI_DMY_ADDR_DW_MM:    STD_LOGIC_VECTOR (11 DOWNTO 0) := X"FFF"; 

--+SPI_END                                                                      SPI-END...

    SIGNAL SPI_READ_HW:             STD_LOGIC;
    SIGNAL READ_LW_VALID:           STD_LOGIC;
   
    SIGNAL READ_SYNC_LAST:          STD_LOGIC;
    SIGNAL READ_TEMP:               STD_LOGIC;
    SIGNAL READ_DATA_TEMP:          STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL READ_LATCHED:            STD_LOGIC;
    SIGNAL READ_DATA_LATCH:         STD_LOGIC_VECTOR (31 DOWNTO 16);
    SIGNAL READ_ADDR_ERROR:         STD_LOGIC;
    SIGNAL READ_NOT_LATCHED:        STD_LOGIC;
    SIGNAL WRITE_LW_VALID:          STD_LOGIC;
    SIGNAL WRITE_SYNC:              STD_LOGIC;
    SIGNAL WRITE_SYNC_LAST:         STD_LOGIC;
    SIGNAL WRITE_TEMP:              STD_LOGIC;
    SIGNAL WRITE_DATA_TEMP:         STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL WRITE_LATCHED:           STD_LOGIC;
    SIGNAL WRITE_ADDR_ERROR:        STD_LOGIC;


    --REGISTER

    SIGNAL TEST_REGISTER_REG_RW:            STD_LOGIC_VECTOR (31 DOWNTO 0);     --RW
    CONSTANT TEST_REGISTER_REG_RST:         STD_LOGIC_VECTOR (31 DOWNTO 0) :=
                                            X"5A5A5A5A";

    SIGNAL TEST_REGISTER_INV_REG_RW:        STD_LOGIC_VECTOR (31 DOWNTO 0);     --RW
    CONSTANT TEST_REGISTER_INV_REG_RST:     STD_LOGIC_VECTOR (31 DOWNTO 0) :=
                                            X"5A5A5A5A";

    --CONSTANT FPGA_VERSION_REG_R:          see at Constants                    --RO

    SIGNAL BOARD_TEST_MODE_REG_RW:          STD_LOGIC_VECTOR (15 DOWNTO 0);     --RW
    CONSTANT BOARD_TEST_MODE_REG_RST:       STD_LOGIC_VECTOR (15 DOWNTO 0) :=
                                            X"0002";

    SIGNAL FPGA_CTRL_REG_RW:                STD_LOGIC_VECTOR (15 DOWNTO 0);     --RW
    CONSTANT FPGA_CTRL_REG_RST:             STD_LOGIC_VECTOR (15 DOWNTO 0) :=
                                            X"0000";

    SIGNAL FPGA_STAT_REG_RO:                STD_LOGIC_VECTOR (15 DOWNTO 0);     --RO
    CONSTANT FPGA_STAT_REG_RST:             STD_LOGIC_VECTOR (15 DOWNTO 0) :=
                                            X"03C0";

    SIGNAL GPO_REG_W:                       STD_LOGIC_VECTOR (7 DOWNTO 0);      --(R?)W
    CONSTANT GPO_REG_RST:                   STD_LOGIC_VECTOR (7 DOWNTO 0) :=
                                            X"01";
    --Array, 32-Bit data                                                        --(R?)W
    CONSTANT WCYC_PARA_AOS:             INTEGER RANGE 0 TO 4095 :=
                                        16#310#;
    CONSTANT WCYC_PARA_ISTART:          INTEGER RANGE 0 TO 7 :=
                                        4;
    --MAX. 32 Elements(SIZE)
    CONSTANT WCYC_PARA_SIZE:            INTEGER RANGE 0 TO 63 :=
                                        28;
    SIGNAL WCYC_PARA_A:                 STD_LOGIC;
    SIGNAL WCYC_PARA_A_IDX:             INTEGER RANGE 0 TO 31;
    CONSTANT WCYC_PARA_SIZE_B:          INTEGER RANGE 0 TO 255 :=
                                        (WCYC_PARA_SIZE)*4;
    TYPE type_WCYC_PARA_W               IS ARRAY
                                        (WCYC_PARA_ISTART TO
                                         WCYC_PARA_ISTART+WCYC_PARA_SIZE-1) OF
                                        STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL WCYC_PARA_REG_W:             type_WCYC_PARA_W;
    CONSTANT WCYC_PARA_RST_X:           STD_LOGIC_VECTOR (31 DOWNTO 0) :=
                                        X"00000000";
    --Index with only 16 Bit Access
    SIGNAL WCYC_PARA_I16:             STD_LOGIC_VECTOR
                                        (WCYC_PARA_ISTART TO
                                         WCYC_PARA_ISTART+
                                         WCYC_PARA_SIZE-1) :=
                                        "1000011000000000000000000000";         --Define
    --Array []                               4   8   12  16  20  24  28

    --Array, 32-Bit data                                                        --RO
    CONSTANT RCYC_STAT_AOS:             INTEGER RANGE 0 TO 4095 :=
                                        16#390#;
    CONSTANT RCYC_STAT_ISTART:          INTEGER RANGE 0 TO 7 :=
                                        4;
    --MAX. 32 Elements(SIZE)
    CONSTANT RCYC_STAT_SIZE:            INTEGER RANGE 0 TO 63 :=
                                        28;
    SIGNAL RCYC_STAT_A:                 STD_LOGIC;
    SIGNAL RCYC_STAT_A_IDX:             INTEGER RANGE 0 TO 31;
    CONSTANT RCYC_STAT_SIZE_B:          INTEGER RANGE 0 TO 255 :=
                                        (RCYC_STAT_SIZE)*4;
    TYPE type_RCYC_STAT_RO              IS ARRAY
                                        (RCYC_STAT_ISTART TO
                                         RCYC_STAT_ISTART+RCYC_STAT_SIZE-1) OF
                                        STD_LOGIC_VECTOR (31 DOWNTO 0);
    SIGNAL RCYC_STAT_REG_RO:            type_RCYC_STAT_RO;
    CONSTANT RCYC_STAT_RST_X:           STD_LOGIC_VECTOR (31 DOWNTO 0) :=
                                        X"00000000";
    --Index with only 16 Bit Access
    SIGNAL RCYC_STAT_I16:             STD_LOGIC_VECTOR
                                        (RCYC_STAT_ISTART TO
                                         RCYC_STAT_ISTART+
                                         RCYC_STAT_SIZE-1) :=
                                        "1000010000000000000000000000";         --Define
    --Array []                           4   8   12  16  20  24  28

    --


    ALIAS WR_D_W:                   STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    FB_SPI_WR_D_W (15 DOWNTO 0);
    ALIAS WR_D_LB:                  STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_WR_D_W (7 DOWNTO 0);
    ALIAS WR_D_HB:                  STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    FB_SPI_WR_D_W (15 DOWNTO 8);
                              
    ALIAS READ_DATA_LATCH_W:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    READ_DATA_LATCH (31 DOWNTO 16);
    ALIAS READ_DATA_LATCH_LB:       STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    READ_DATA_LATCH (23 DOWNTO 16);
    ALIAS READ_DATA_LATCH_HB:       STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    READ_DATA_LATCH (31 DOWNTO 24);

    ALIAS READ_DATA_TEMP_32:        STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    READ_DATA_TEMP (31 DOWNTO 0);
    ALIAS READ_DATA_TEMP_16:        STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    READ_DATA_TEMP (31 DOWNTO 16);
    ALIAS READ_DATA_TEMP_8:         STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    READ_DATA_TEMP (31 DOWNTO 24);
    ALIAS READ_DATA_TEMP_32_HW:     STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (31 DOWNTO 16);
    ALIAS READ_DATA_TEMP_32_LW:     STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (15 DOWNTO 0);

    ALIAS READ_DATA_TEMP_32_A1:     STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    READ_DATA_TEMP (31 DOWNTO 0);
    ALIAS READ_DATA_TEMP_32_HW_A1:  STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (31 DOWNTO 16);
    ALIAS READ_DATA_TEMP_32_LW_A1:  STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (15 DOWNTO 0);

    ALIAS READ_DATA_TEMP_32_A2:     STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    READ_DATA_TEMP (31 DOWNTO 0);
    ALIAS READ_DATA_TEMP_32_HW_A2:  STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (31 DOWNTO 16);
    ALIAS READ_DATA_TEMP_32_LW_A2:  STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    READ_DATA_TEMP (15 DOWNTO 0);

    ALIAS WRITE_DATA_TEMP_32:       STD_LOGIC_VECTOR (31 DOWNTO 0) IS
                                    WRITE_DATA_TEMP (31 DOWNTO 0);
    ALIAS WRITE_DATA_TEMP_16:       STD_LOGIC_VECTOR (15 DOWNTO 0) IS
                                    WRITE_DATA_TEMP (31 DOWNTO 16);
    ALIAS WRITE_DATA_TEMP_8:        STD_LOGIC_VECTOR (7 DOWNTO 0) IS
                                    WRITE_DATA_TEMP (31 DOWNTO 24);
    ALIAS WRITE_DATA_TEMP_32_HW:    STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    WRITE_DATA_TEMP (31 DOWNTO 16);
    ALIAS WRITE_DATA_TEMP_32_LW:    STD_LOGIC_VECTOR (31 DOWNTO 16) IS
                                    WRITE_DATA_TEMP (15 DOWNTO 0);

    ALIAS BOARD_TEST_MODE_OL_RESET: STD_LOGIC IS
                                    BOARD_TEST_MODE_REG_RW(0);
    ALIAS BOARD_TEST_MODE_DDS_STOP: STD_LOGIC IS
                                    BOARD_TEST_MODE_REG_RW(1);
    ALIAS BOARD_TEST_MODE_PWMA:     STD_LOGIC IS
                                    BOARD_TEST_MODE_REG_RW(2);
    ALIAS BOARD_TEST_MODE_PWMB:     STD_LOGIC IS
                                    BOARD_TEST_MODE_REG_RW(3);

    ALIAS FPGA_CTRL_BTM:            STD_LOGIC IS
                                    FPGA_CTRL_REG_RW(0);
    ALIAS FPGA_CTRL_INIT_DONE:      STD_LOGIC IS
                                    FPGA_CTRL_REG_RW(4);
    ALIAS FPGA_CTRL_RUN:            STD_LOGIC IS
                                    FPGA_CTRL_REG_RW(5);

    ALIAS FPGA_STAT_BTM:            STD_LOGIC IS
                                    FPGA_STAT_REG_RO(0);
    ALIAS FPGA_STAT_INIT_DONE:      STD_LOGIC IS
                                    FPGA_STAT_REG_RO(4);
    ALIAS FPGA_STAT_RUN:            STD_LOGIC IS
                                    FPGA_STAT_REG_RO(5);
    ALIAS FPGA_STAT_PWM_ZERO:       STD_LOGIC IS
                                    FPGA_STAT_REG_RO(6);
    ALIAS FPGA_STAT_SONIC_SAFE:     STD_LOGIC IS
                                    FPGA_STAT_REG_RO(7);
    ALIAS FPGA_STAT_FB_ERROR:       STD_LOGIC IS
                                    FPGA_STAT_REG_RO(8);
    ALIAS FPGA_STAT_ADC_ERROR:      STD_LOGIC IS
                                    FPGA_STAT_REG_RO(9);
    ALIAS FPGA_STAT_HW_VERS_ERROR:  STD_LOGIC IS
                                    FPGA_STAT_REG_RO(10);

    ALIAS GPO_OL_RESET:             STD_LOGIC IS
                                    GPO_REG_W(0);
    ALIAS GPO_SONIC_ON:             STD_LOGIC IS
                                    GPO_REG_W(1);
				
begin

--TESTs

--READ
--FBIF_TEST_2 <= READ_TEMP AND (NOT READ_NOT_LATCHED);
--FBIF_TEST_1 <= READ_LATCHED AND (NOT READ_ADDR_ERROR);

--SPI
--FBIF_TEST_1 <= SPI_HDL_WR;
--FBIF_TEST_2 <= SPI_HDL_RD OR SPI_HDL_RPTY;
--FBIF_TEST_2 <= SPI_CLK_F_H_ERR;

--WATCHDOG
--FBIF_TEST_1<=WDOG_ACK_PWM;
--FBIF_TEST_2<=WDOG_ACK_FREQU;

--NotUsed
--FBIF_TEST_1 <= 0;
--FBIF_TEST_2 <= 0;


--CONTROL
FPGA_CTRL_TEST <= FPGA_CTRL_REG_RW;
--STATUS
FPGA_STAT_TEST <= FPGA_STAT_REG_RO;

-- GPI, GPO Generel Purpose I/O Handling
    -- GPI is sampled during Read!
    GPO <= GPO_REG_W;
	 DDS_STOP <= NOT GPO_SONIC_ON;
	 
	 PHASE_SIGN<=GPI(2);
--Overload signals
	OL_TEMP<=GPI(3);
	OL_POWER<=GPI(4);
	OL_CURRENT<=GPI(6);
   OL_VOLTAGE<=GPI(5);
   OL_FUNCTION<=GPI(7);


-- FPGA BTM,- Control,- Status Handling

-- BTM, Board Testmode
    BTM <= FPGA_STAT_BTM;
    BTM_GPO(0) <= BOARD_TEST_MODE_OL_RESET;
    BTM_GPO(1) <= NOT BOARD_TEST_MODE_DDS_STOP;                     -- SONIC_ON
    BTM_PWMA <= BOARD_TEST_MODE_PWMA;
    BTM_PWMB <= BOARD_TEST_MODE_PWMB;
	 
-- HW_VERS_ERROR
    p_hw_vers_error: PROCESS (X_RST, CLK_8)
    BEGIN
        IF (X_RST='0') THEN
            SAMPLE_HW_VERSION <= '1';
        ELSIF (Rising_Edge(CLK_8)) THEN
            --Sample In after RESET...
            IF (SAMPLE_HW_VERSION = '1') THEN

                --T1-BD
                IF (TYPE_BD = X"01") THEN
                    IF ((HW_VERS_BD(7 DOWNTO 0)) =
                        (HW_VERS_BD_T1_COMPARE(7 DOWNTO 0))) THEN
                        HW_VERS_ERROR <= '0';
                    ELSE
                        HW_VERS_ERROR <= '1';
                    END IF;
                --T2-BD
                ELSIF (TYPE_BD = X"02") THEN
                    IF ((HW_VERS_BD(7 DOWNTO 0)) =
                        (HW_VERS_BD_T2_COMPARE_1310Bd(7 DOWNTO 0))) THEN
                        HW_VERS_ERROR <= '0';
                    ELSIF((HW_VERS_BD(7 DOWNTO 0)) =
                        (HW_VERS_BD_T2_COMPARE_024Bd(7 DOWNTO 0))) THEN
                        HW_VERS_ERROR <= '0';
                    ELSE
                        HW_VERS_ERROR <= '1';
                    END IF;
                --ELSIF, Add new BD-Types here...
                --T?, not defined
                ELSE
                    HW_VERS_ERROR <= '1';
                END IF;
                SAMPLE_HW_VERSION <= '0';
            END IF;
        END IF;
    END PROCESS p_hw_vers_error;

-- RUN DDS,- PWM control
    FPGA_RUN <= FPGA_STAT_RUN AND FPGA_RUN_EN;

-- FBus Access control
    INITIALIZED <= FPGA_CTRL_INIT_DONE AND FPGA_STAT_INIT_DONE;
    SONIC_SAFE <= DDS_STOP AND PWM_VAL_REG_EQ_ZERO;

-- FPGA BTM,- Status handling
    p_fpga_status: PROCESS (X_RST, CLK_8)
    BEGIN
        IF (X_RST='0') THEN
            FPGA_STAT_REG_RO <= FPGA_STAT_REG_RST;                  --INIT
				FPGA_STAT_RUN_COUNT <= 0; --Init of counter for run signal delay
				FPGA_RUN_EN<='0';
        ELSIF (Rising_Edge(CLK_8)) THEN

            IF ((FPGA_CTRL_BTM = '0') AND
                (SONIC_SAFE = '1')) THEN
                --RESET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_BTM <= '0';
            ELSIF ((FPGA_CTRL_BTM = '1') AND
                   (SONIC_SAFE = '1')) THEN
                --SET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_BTM <= '1';
            END IF;

            IF (FPGA_CTRL_INIT_DONE = '1') THEN
                FPGA_STAT_INIT_DONE <= '1';
            ELSIF ((FPGA_CTRL_INIT_DONE = '0') AND
                   (SONIC_SAFE = '1')) THEN
                --RESET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_INIT_DONE <= '0';
            END IF;

            IF ((FPGA_CTRL_RUN = '1') AND
                (FPGA_STAT_INIT_DONE = '1') AND
                (HW_VERS_ERROR = '0') AND                           --HW-VERS.!
                (WDOG_ERROR = '0') AND 
                (ADC_ERROR = '0') AND
                (SONIC_SAFE = '1')) THEN 
                --SET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security AND
                --after FB,- ADC ERROR Reset
                        IF(FPGA_STAT_RUN_COUNT < MAX_FPGA_STAT_RUN_DELAY)THEN     --Waits for 1 second 
                             FPGA_STAT_RUN_COUNT<=FPGA_STAT_RUN_COUNT+1;
                        ELSE
									FPGA_RUN_EN<='1';  
                        END IF;
								FPGA_STAT_RUN <= '1';
            ELSIF((FPGA_STAT_INIT_DONE = '1') AND
                   (SONIC_SAFE = '0')  AND
                   (FPGA_STAT_RUN_COUNT <  MAX_FPGA_STAT_RUN_DELAY))THEN
                    --Avoids any sonics command from MCU before start up delay. Sonics command prior the delay is not a normal operation of the MCU!!
                       FPGA_RUN_EN <= '0';  
            ELSIF (((FPGA_CTRL_RUN = '0') OR
                    (FPGA_STAT_INIT_DONE = '0')) AND
                   (SONIC_SAFE = '1')) THEN
                --RESET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_RUN <= '0';
            ELSIF ((FPGA_STAT_FB_ERROR = '1') OR
                   (FPGA_STAT_ADC_ERROR = '1')) THEN
                --RESET immediatly
               FPGA_STAT_RUN <= '0'; 
            END IF;

            IF (PWM_VAL_REG_EQ_ZERO = '1') THEN
                FPGA_STAT_PWM_ZERO <= '1';
            ELSE
                FPGA_STAT_PWM_ZERO <= '0';
            END IF;

            IF (SONIC_SAFE = '1') THEN
                FPGA_STAT_SONIC_SAFE <= '1';
            ELSE
                FPGA_STAT_SONIC_SAFE <= '0';
            END IF;

            IF (WDOG_ERROR = '1') THEN
                --SET
                FPGA_STAT_FB_ERROR <= '1';
            ELSIF ((WDOG_ERROR = '0') AND
                   (SONIC_SAFE = '1')) THEN
                --RESET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_FB_ERROR <= '0';
            END IF;

            IF (ADC_ERROR = '1') THEN
                --SET
                FPGA_STAT_ADC_ERROR <= '1';
            ELSIF ((ADC_ERROR = '0') AND
                   (SONIC_SAFE = '1')) THEN
                --RESET only at DDS_STOP, PWM=0, (= SONIC_SAFE) Security
                FPGA_STAT_ADC_ERROR <= '0';
            END IF;

            FPGA_STAT_HW_VERS_ERROR <= HW_VERS_ERROR;

        END IF;
    END PROCESS p_fpga_status;

--+SPI                                                                          SPI...

-- SPI-Interface
-- CPOL=1, CPHA=1                                                               ?
-- MSBit/MSByte first
-- Bit length = 64 Bit
-- Baudrate = 12MHz Max.
-- Baudrate = 12MBit/s -> Tclk =  83ns --> Debounce with 10ns
-- CS inactive between 2 Xfers Min. 2 Bittimes / 160 ns

-- Debounce SPI_CLK, Sync In
    p_spi_clk_deb: PROCESS (X_RST, CLK_M, SPI_CLK)
    BEGIN
        IF (X_RST='0') THEN
            SPI_CLK_DEB <= '1';                                  -- IDLE        ?
            i_SPI_CLK_DEB_CNT <= 0;
            i_SPI_CLK_F_H_ERR_CNT <= 0;
            SPI_CLK_F_H_ERR_CNT_RST <= '0';
            SPI_CLK_F_H_ERR <= '0';
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (SPI_CLK = SPI_CLK_DEB) THEN
                i_SPI_CLK_DEB_CNT <= 0;
            ELSE
                i_SPI_CLK_DEB_CNT <= i_SPI_CLK_DEB_CNT+1;
            END IF;
            IF ((i_SPI_CLK_DEB_CNT >= 2) AND                     -- 10ns
                (SPI_CLK_DEB /= SPI_CLK)) THEN
                SPI_CLK_DEB <= SPI_CLK;
                --Detect SPI CLK Freuqency High Error !
                IF (i_SPI_CLK_F_H_ERR_CNT <= 5) THEN --**
                    --5 = Fg 14.2 MHz    **
                    --6 = Fg 12.5 MHz
                    --7 = Fg 11.1 MHz
                    --** FHigh Max = 14.2MHz!
                    IF (SPI_CS_DEB = '1') THEN
                        SPI_CLK_F_H_ERR <= '1';                  --Set
                    END IF;
                END IF;
                --Reset Frequ. High Error CNT
                SPI_CLK_F_H_ERR_CNT_RST <= '1';
            END IF;
           --Handle Frequ. High Error CNT
            IF (SPI_CLK_F_H_ERR_CNT_RST = '1') THEN
                SPI_CLK_F_H_ERR_CNT_RST <= '0';
                --Reset
                i_SPI_CLK_F_H_ERR_CNT <= 0;
            ELSIF (i_SPI_CLK_F_H_ERR_CNT < 15) THEN
                --Increment
                i_SPI_CLK_F_H_ERR_CNT <= i_SPI_CLK_F_H_ERR_CNT+1;
            END IF;
            --Reset Frequ. High Error
            IF (SPI_CS_DEB = '0') THEN
                SPI_CLK_F_H_ERR <= '0';                          --Reset
            END IF;
        END IF;
    END PROCESS p_spi_clk_deb;

-- Debounce SPI_N_CS, Sync In
    p_spi_cs_deb: PROCESS (X_RST, CLK_M, SPI_N_CS)
    BEGIN
        IF (X_RST='0') THEN
            SPI_N_CS_DEB <= '1';                                 -- IDLE
            SPI_CS_DEB <= '0';
            i_SPI_CS_DEB_CNT <= 0;
        ELSIF (Rising_Edge(CLK_M)) THEN
            IF (SPI_N_CS = SPI_N_CS_DEB) THEN
                i_SPI_CS_DEB_CNT <= 0;
            ELSE
                i_SPI_CS_DEB_CNT <= i_SPI_CS_DEB_CNT+1;
            END IF;
            IF ((i_SPI_CS_DEB_CNT >= 2) AND                      -- 10ns
                (SPI_N_CS_DEB /= SPI_N_CS)) THEN
                SPI_N_CS_DEB <= SPI_N_CS;
                SPI_CS_DEB <= NOT SPI_N_CS;                      -- Invert!
            END IF;
        END IF;
    END PROCESS p_spi_cs_deb;

-- MOSI, Recieve, SPI Engine
    p_spi_engine_MOSI: PROCESS (X_RST, SPI_CS_DEB, SPI_CLK_DEB,
                                SPI_START_CLEAR)
    BEGIN
        IF (X_RST = '0') THEN
            SPI_START <= '0';
            SPI_OK <= '0';
            SPI_MOSI_RD_START <= '0';
            SPI_ERROR_CLK_F_H <= '0';
            SPI_ERROR_BIT_ORUN <='0';
            SPI_MOSI_DIN <= (OTHERS => '0');
        ELSIF (SPI_CS_DEB = '0') THEN                            --Restart
            IF (SPI_START_CLEAR = '1') THEN                      --Strobe!
                SPI_START <= '0';
            END IF;
        ELSIF (Rising_Edge(SPI_CLK_DEB)) THEN                    --CS active    ? Rising/Falling ?
            --Edge fo SPI_CLK

            --Reset Indications
            IF (SPI_START = '0') THEN
                SPI_START <= '1';
                SPI_OK <= '0';
                SPI_ERROR_CLK_F_H <= '0';
                SPI_ERROR_BIT_ORUN <='0';
                --Sample first MOSI... Reset BITCNT...
                SPI_MOSI_DIN(i_SPI_BIT_IDX_MAX) <= SPI_I_MOSI;
                i_SPI_BIT_IDX_CNT <= i_SPI_BIT_IDX_MAX-1;

            ELSIF (SPI_CLK_F_H_ERR = '1') THEN
                --SPI-CLK Frequ. High Error, Ignore Hdl. Write
                SPI_ERROR_CLK_F_H <= '1';

            ELSIF (SPI_OK = '0') THEN
                --Sample MOSI...
                SPI_MOSI_DIN(i_SPI_BIT_IDX_CNT) <= SPI_I_MOSI;
                IF (i_SPI_BIT_IDX_CNT > 0) THEN
                    i_SPI_BIT_IDX_CNT <= i_SPI_BIT_IDX_CNT-1;
                ELSE
                   --End!
                   SPI_OK <= '1';
                END IF;

            ELSE -- SPI_OK = '1'
                --ERROR Bit Overrun
                SPI_ERROR_BIT_ORUN <= '1';
                SPI_OK <= '0';
            END IF;

            --Set,- Indicate Start for Read handling...
            --Read Address sampled In...
            IF (i_SPI_BIT_IDX_CNT = (i_SPI_BIT_IDX_MAX-12)) THEN
                SPI_MOSI_RD_START <= '1';
            ELSE
                SPI_MOSI_RD_START <= '0';
            END IF;

        END IF;
    END PROCESS p_spi_engine_MOSI;

-- MISO, Transmit, SPI Engine
    p_spi_engine_MISO: PROCESS (X_RST, SPI_CS_DEB,
                                SPI_CLK_DEB, SPI_ERROR_CLK_F_H, SPI_START,
                                SPI_MISO_DOUT)
    BEGIN
        IF (X_RST = '0') THEN
            SPI_O_MISO <= '0';
        ELSIF (SPI_CS_DEB = '0') THEN                            --IDLE
            SPI_O_MISO <= '0';
        ELSIF (SPI_ERROR_CLK_F_H = '1') THEN
            --SPI-CLK Frequ. High Error, Ignore Hdl. Read
            --MISO <- Zero
            SPI_O_MISO <= '0';
        ELSIF (SPI_START = '0') THEN                             --Restart
            SPI_O_MISO <= SPI_MISO_DOUT(i_SPI_BIT_IDX_MAX);
        ELSIF (Falling_Edge(SPI_CLK_DEB)) THEN                   --CS active    ? Rising/Falling ?
            --Edge fo SPI_CLK

            --Shift MISO...
            SPI_O_MISO <= SPI_MISO_DOUT(i_SPI_BIT_IDX_CNT); 
        END IF;
    END PROCESS p_spi_engine_MISO;

-- SPI-Handling..., Synced with CLK_8 !
    p_spi: PROCESS (X_RST, SPI_CS_DEB, CLK_8)
    BEGIN
        IF (X_RST='0') THEN

            SPI_START_CLEAR <= '0';
            SPI_MOSI_RD_START_LAST1 <= '0';
            SPI_MOSI_RD_START_LAST2 <= '0';
            SPI_CS_DEB_LAST1 <= '0';
            SPI_CS_DEB_LAST2 <= '0';
            SPI_CS_DEB_LAST3 <= '0';
            SPI_START_LAST1 <= '0';
            SPI_START_LAST2 <= '0';
            SPI_OK_LAST1 <= '0';
            SPI_OK_LAST2 <= '0';
            SPI_ERROR_BIT_ORUN_LAST1 <= '0';
            SPI_ERROR_BIT_ORUN_LAST2 <= '0';

            SPI_ERROR_BUR <= '0';
            SPI_ERROR_BOR <= '0';
            SPI_ERROR_L_WPTY <= '0';
            SPI_ERROR_L_WA <= '0';
            SPI_ERROR_L_RA <= '0';

            SPI_NEW_MOSI_VALID <= '0';
            SPI_HDL_WPTY <= '0';
            SPI_HDL_WR_CHK_DMY_ADDR <= '0';
            SPI_HDL_WR_START <= '0';
            SPI_HDL_WR <= '0';
            SPI_HDL_RD_SM_SR <= (OTHERS => '0');

            SPI_WR_RD_ADR <= (OTHERS => '0');
            SPI_WR_D_W <= (OTHERS => '0');
            SPI_WRITE_SYNC <= '0';

            SPI_MOSI_REG_RD_ADR <= (OTHERS => '0');
            SPI_MOSI_REG <= (OTHERS => '0');

            SPI_NEW_MOSI_RD_VALID <= '0';
            SPI_HDL_RD_CHK_DMY_ADDR <= '0';
            SPI_HDL_RD_START <= '0';
            SPI_HDL_RD <= '0';
            SPI_HDL_RD_SM_SR <= (OTHERS => '0');
            SPI_HDL_RPTY <= '0';

            SPI_READ_SYNC <= '0';

            SPI_MISO_DOUT <= (OTHERS => '0'); 
            SPI_MISO_DOUT_LD_ZERO <= (OTHERS => '0');            --Ld. Zero
			

        ELSIF (Rising_Edge(CLK_8)) THEN
            --Sync in to CLK_8

            --SYNC IN !
            SPI_MOSI_RD_START_LAST1 <= SPI_MOSI_RD_START;
            SPI_MOSI_RD_START_LAST2 <= SPI_MOSI_RD_START_LAST1;
            SPI_CS_DEB_LAST1 <= SPI_CS_DEB;
            SPI_CS_DEB_LAST2 <= SPI_CS_DEB_LAST1;
            SPI_CS_DEB_LAST3 <= SPI_CS_DEB_LAST2;
            SPI_START_LAST1 <= SPI_START;
            SPI_START_LAST2 <= SPI_START_LAST1;
            SPI_OK_LAST1 <= SPI_OK;
            SPI_OK_LAST2 <= SPI_OK_LAST1;
            SPI_ERROR_CLK_F_H_LAST1 <= SPI_ERROR_CLK_F_H;
            SPI_ERROR_CLK_F_H_LAST2 <= SPI_ERROR_CLK_F_H_LAST1;
            SPI_ERROR_BIT_ORUN_LAST1 <= SPI_ERROR_BIT_ORUN;
            SPI_ERROR_BIT_ORUN_LAST2 <= SPI_ERROR_BIT_ORUN_LAST1;


            --Handling (READ) after Read Address sampled, Strobe !              READ
            --HDL RD
            IF ((SPI_MOSI_RD_START_LAST1 = '1') AND
                (SPI_MOSI_RD_START_LAST2 = '0')) THEN

                --4 BTimes ~ 8 CLK8(Dx) left to set ERR
                --11 BTimes ~ 22 CLK8(Dx) left to set RD-Value & Calc. Pty.
                --Sync...                                             --...D4

                --Latch
                SPI_MOSI_REG_RD_ADR(11 DOWNTO 0) <=
                    SPI_MOSI_DIN_RD_ADR(11 DOWNTO 0);
						  
                --Leading Zero's alwasy Set...
                --Set ERR
                IF (SPI_ERROR_L_WPTY = '0') THEN
                    SPI_MISO_DOUT_ERR_7_RA <= SPI_ERROR_L_RA;
                    SPI_MISO_DOUT_ERR_6_WA <= SPI_ERROR_L_WA;
                ELSE
                    SPI_MISO_DOUT_ERR_7_RA <= '0';
                    SPI_MISO_DOUT_ERR_6_WA <= '0';
                END IF;
                SPI_MISO_DOUT_ERR_5 <= '0';
                SPI_MISO_DOUT_ERR_4_WPTY <= SPI_ERROR_L_WPTY;
                SPI_MISO_DOUT_ERR_3 <= '0';
                SPI_MISO_DOUT_ERR_2 <= '0';
                SPI_MISO_DOUT_ERR_1_BOR <= SPI_ERROR_BOR;
                SPI_MISO_DOUT_ERR_0_BUR <= SPI_ERROR_BUR;

                --Read...
                SPI_NEW_MOSI_RD_VALID <= '1';
            END IF;

            --New  MOSI Read Address valid...
            IF (SPI_NEW_MOSI_RD_VALID = '1') THEN                     --D5
                SPI_NEW_MOSI_RD_VALID <= '0';
                SPI_HDL_RD_CHK_DMY_ADDR <= '1';
            END IF;

            --HDL RD, Check for Read/Dummy Address
            IF (SPI_HDL_RD_CHK_DMY_ADDR = '1') THEN                   --D6
                SPI_HDL_RD_CHK_DMY_ADDR <= '0';
                IF ((SPI_MOSI_REG_RD_ADR(11 DOWNTO 0) /=
                     SPI_DMY_ADDR_DW_MM(11 DOWNTO 0))) THEN
                    --Read
                    SPI_HDL_RD_START <= '1';
                ELSE
                    --DUMMY Addr. -> Pty.
                    SPI_MISO_REG_RD_DATA(31 DOWNTO 0) <= (OTHERS => '0');
                    SPI_ERROR_L_RA <= '0';
                    SPI_HDL_RPTY <= '1';
                END IF;
            END IF;

            --HDL RD, Start
            IF (SPI_HDL_RD_START = '1') THEN                          --D7
                 SPI_HDL_RD_START <= '0';
                 --Reset Shift Register, SM (Statemachine)
                 SPI_HDL_RD_SM_SR((SPI_HDL_RD_SM_SR'LENGTH-1) DOWNTO 0) <=
                     (OTHERS => '0');
                 SPI_HDL_RD_SM_SR(0) <= '1';
                 SPI_HDL_RD <= '1';
            END IF;

            --HDL RD, ...
            IF (SPI_HDL_RD = '1') THEN                            --D8..D20
                --Shift State
                SPI_HDL_RD_SM_SR((SPI_HDL_RD_SM_SR'LENGTH-1) DOWNTO 0) <=
                    SPI_HDL_RD_SM_SR((SPI_HDL_RD_SM_SR'LENGTH-2) DOWNTO 0) &
                    '0';
--0            Set Address
--1            Check Address --> No DW -> END
--2            Act RSync
--3, 4, 5      Wait
--6            Get Data DW
--             DeAct RSync (Check AE, ERR-> Clear Data DW)
--             END
--7..12        xx not used

                --SM
                IF (SPI_HDL_RD_SM_SR(0) = '1') THEN
                    --Set Address...
                    SPI_WR_RD_ADR(11 DOWNTO 0) <=
                        SPI_MOSI_REG_RD_ADR(11 DOWNTO 0);
                END IF;

                IF (SPI_HDL_RD_SM_SR(1) = '1') THEN
                    --Check Address
                    IF (FB_A_INT_IS_DWA = '0') THEN
                        --Adress ERROR
                        SPI_ERROR_L_RA <= '1';
                        SPI_MISO_REG_RD_DATA(31 DOWNTO 0) <=
                            (OTHERS => '0');
                        --END !, SR-SM End...
                        SPI_HDL_RD <= '0';
                        SPI_HDL_RPTY <= '1';
                    END IF;
                END IF;

                IF (SPI_HDL_RD_SM_SR(2) = '1') THEN
                    --Activate Read
                    SPI_READ_SYNC <= '1';
                END IF;

                --WAIT, (3), (4), (5)

                IF (SPI_HDL_RD_SM_SR(6) = '1') THEN
                    IF (READ_ADDR_ERROR = '1') THEN
                        --Adress ERROR
                        SPI_ERROR_L_RA <= '1';
                        SPI_MISO_REG_RD_DATA(31 DOWNTO 0) <=
                            (OTHERS => '0');
                    ELSE
                        --NO Adress Error
                        --Get Data DW
                        SPI_MISO_REG_RD_DATA(31 DOWNTO 0) <=
                            FB_SPI_RD_D_DW(31 DOWNTO 0);
                        --Reset Error
                        SPI_ERROR_L_RA <= '0';
                    END IF;
                    --Dectivate Read
                    SPI_READ_SYNC <= '0';
                    --END !, SR-SM End...
                    SPI_HDL_RD <= '0';
                    SPI_HDL_RPTY <= '1';
                END IF;

                --NOT USED, (7)..(12)
                --SM-End
            END IF;

            --Handle, Calculate PTY...
            IF (SPI_HDL_RPTY = '1') THEN                              --D21     -- TIMING OK... PO 0015
                 SPI_HDL_RPTY <= '0';
                 --Calculate PTY, Without RPTY itself !
                 SPI_MISO_REG_RPTY(7 DOWNTO 0) <= (
                     (SPI_MISO_DOUT_RPTY_BY7(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY6(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY5(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY3(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY2(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY1(7 DOWNTO 0)) XOR
                     (SPI_MISO_DOUT_RPTY_BY0(7 DOWNTO 0)));

                 --END
            END IF;


            --Handling (WRITE, ERROR) after CS goes inactive, Strobe !          ERROR/WRITE
            IF ((SPI_CS_DEB_LAST2 = '0') AND
                (SPI_CS_DEB_LAST3 = '1')) THEN

               --14 BTimes ~ 28 CLK8(Dx) left to set ERR and write WR-Value
               --Sync...                                              --...D5

                --Check Error/Valid
                IF (SPI_ERROR_CLK_F_H_LAST2 = '1') THEN
                    --ERROR SPI-CLK Frequ. High Error
                    --Do Nothing, Ignore Hdl. Write
                ELSIF (SPI_ERROR_BIT_ORUN_LAST2 = '1') THEN
                    --ERROR OVERRUN
                    SPI_ERROR_BOR <= '1';
                    SPI_ERROR_BUR <= '0';
                ELSIF ((SPI_START_LAST2 = '0') OR
                       (SPI_OK_LAST2 = '0')) THEN
                    --ERROR UNDERRUN (OR SPI-CLK Frequ. High Error)
                    SPI_ERROR_BUR <= '1';
                    SPI_ERROR_BOR <= '0';
                ELSE
                    --OK, Reset Errors, Valid...
                    SPI_ERROR_BUR <= '0';
                    SPI_ERROR_BOR <= '0';
                    SPI_ERROR_L_WPTY <= '0';

                    --Latch...
                    SPI_MOSI_REG(63 DOWNTO 0) <= SPI_MOSI_DIN(63 DOWNTO 0);
						 
                    SPI_NEW_MOSI_VALID <= '1';
                END IF;
                --Reset Start, Strobe!
                SPI_START_CLEAR <= '1';
            ELSE
                SPI_START_CLEAR <= '0';
            END IF;

            --HDL WR
            IF (SPI_NEW_MOSI_VALID = '1') THEN                        --D6
                 SPI_NEW_MOSI_VALID <= '0';
                 SPI_HDL_WPTY <= '1';
            END IF;

            --Handle, Check PTY...
             IF (SPI_HDL_WPTY = '1') THEN                             --D7
                 SPI_HDL_WPTY <= '0';
                 --Check PTY
                 IF (((SPI_MOSI_REG_WPTY_BY7(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY6(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY5(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY4_PT(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY3(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY2(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY1(7 DOWNTO 0)) XOR
                      (SPI_MOSI_REG_WPTY_BY0(7 DOWNTO 0))) /= X"00") THEN
                     --ERROR WPTY
                     SPI_ERROR_L_WPTY <= '1';
                 ELSE
                     --OK
                     SPI_ERROR_L_WPTY <= '0';
                     SPI_HDL_WR_CHK_DMY_ADDR <= '1';
                 END IF;
             END IF;

            --HDL WR, Check for Write/Dummy Address
            IF (SPI_HDL_WR_CHK_DMY_ADDR = '1') THEN                   --D8
                 SPI_HDL_WR_CHK_DMY_ADDR <= '0';
                IF ((SPI_MOSI_REG_WR_ADR(11 DOWNTO 0) /=
                     SPI_DMY_ADDR_DW_MM(11 DOWNTO 0))) THEN
                    --Write
                    SPI_HDL_WR_START <= '1';
                ELSE
                    SPI_ERROR_L_WA <= '0';
                    --END
                END IF;
            END IF;

            --HDL WR, Start
            IF (SPI_HDL_WR_START = '1') THEN                          --D9
                 SPI_HDL_WR_START <= '0';
                 --Reset Shift Register, SM (Statemachine)
                 SPI_HDL_WR_SM_SR((SPI_HDL_WR_SM_SR'LENGTH-1) DOWNTO 0) <=
                     (OTHERS => '0');
                 SPI_HDL_WR_SM_SR(0) <= '1';
                 SPI_HDL_WR <= '1';
            END IF;

            --HDL WR, ...
            IF (SPI_HDL_WR = '1') THEN                            --D10..D27
                 --Shift State
                SPI_HDL_WR_SM_SR((SPI_HDL_WR_SM_SR'LENGTH-1) DOWNTO 0) <=
                    SPI_HDL_WR_SM_SR((SPI_HDL_WR_SM_SR'LENGTH-2) DOWNTO 0) &
                    '0';
--0            Set Address
--1            Check Address --> No DW -> END
--2            Set Data DW, Act WSync
--3, 4, 5      Wait
--6            DeAct WSync (Check AE)
--             END
--7..16        xx not used

               --SM
                IF (SPI_HDL_WR_SM_SR(0) = '1') THEN
                    --Set Address...
                    SPI_WR_RD_ADR(11 DOWNTO 0) <=
                        SPI_MOSI_REG_WR_ADR(11 DOWNTO 0);
								
                END IF;

                IF (SPI_HDL_WR_SM_SR(1) = '1') THEN
                    --Check Address
                    IF (FB_A_INT_IS_DWA = '0') THEN
                        --Adress ERROR
                        SPI_ERROR_L_WA <= '1';
                        --END !, SR-SM End...
                        SPI_HDL_WR <= '0';
                    END IF;
                END IF;

                IF (SPI_HDL_WR_SM_SR(2) = '1') THEN
                    --Set Write Data DW
                    FB_SPI_WR_D_DW(31 DOWNTO 0) <=
                        SPI_MOSI_REG_WR_DATA(31 DOWNTO 0);
                    --Activate Write
                    SPI_WRITE_SYNC <= '1';
                END IF;

                --WAIT, (3), (4), (5)

                IF (SPI_HDL_WR_SM_SR(6) = '1') THEN
                    IF (WRITE_ADDR_ERROR = '1') THEN
                        --Adress ERROR
                        SPI_ERROR_L_WA <= '1';
                    ELSE
                        --Reset Error
                        SPI_ERROR_L_WA <= '0';
                    END IF;
                    --Dectivate Write
                    SPI_WRITE_SYNC <= '0';
                    --END !, SR-SM End...
                    SPI_HDL_WR <= '0';
                END IF;

                --NOT USED, (7)..(16)
                --SM-End
                 --END
            END IF;
        END IF;
    END PROCESS p_spi;
--+SPI_END  
	 SPI_RD_D_W <= READ_DATA_LATCH_W;
    FB_SPI_WR_D_W(15 DOWNTO 0) <= SPI_WR_D_W(15 DOWNTO 0);

-- Internal 10-Bit Addressbus (Mirrored to 12-Bit Addressbus)
    FB_A_INT (11 DOWNTO 0) <= ("00" & SPI_WR_RD_ADR(9 DOWNTO 0));

-- Addressdecoding, Pre-Calculations
    FB_A_INT_IS_WA <= NOT FB_A_INT(0);                   --Is Word Address
    --FB_A_INT_IS_DWA <= (NOT FB_A_INT(0)) AND
                      -- (NOT FB_A_INT(1));                --Is Doublew. Address
	 FB_A_INT_IS_DWA<='1'; --Always Is Double word Address
    FB_A_INT_IS_HWA <= (NOT FB_A_INT(0)) AND
                       (NOT FB_A_INT(1));                --Is Highword Address
    FB_A_INT_IS_LWA <= (NOT FB_A_INT(0)) AND
                       (FB_A_INT(1));                    --Is Lowword Address
    FB_A_INT_WA(10 DOWNTO 0) <= FB_A_INT(11 DOWNTO 1);   --Word Address
    FB_A_INT_DWA(9 DOWNTO 0) <= FB_A_INT(11 DOWNTO 2);   --Doubleword Address
    --Address +2 for DW-LW-Check
    FB_A_INT_LW_CHECK(11 DOWNTO 0) <= STD_LOGIC_VECTOR(
        (UNSIGNED(FB_A_INT(11 DOWNTO 0))) + 2);
    READ_LW_VALID <=
        '1' WHEN (FB_A_INT(11 DOWNTO 0) =
                  FB_A_INT_LW_CHECK_RD(11 DOWNTO 0)) ELSE
        '0';
    WRITE_LW_VALID <=
        '1' WHEN (FB_A_INT(11 DOWNTO 0) =
                  FB_A_INT_LW_CHECK_WR(11 DOWNTO 0)) ELSE
        '0';

    --Arrays, Doubleword Objects, Decoding
    
    AD_WCYC_PARA: PROCESS (FB_A_INT_DWA)
    BEGIN
        IF (((UNSIGNED(FB_A_INT_DWA)) >= WCYC_PARA_AOS / 4) AND
            ((UNSIGNED(FB_A_INT_DWA)) < (WCYC_PARA_AOS +
                                         WCYC_PARA_SIZE_B) / 4)) THEN
             WCYC_PARA_A <= '1';
        ELSE
             WCYC_PARA_A <= '0';
        END IF;
        WCYC_PARA_A_IDX <= (TO_INTEGER(
            (UNSIGNED(FB_A_INT_DWA)) -
            (WCYC_PARA_AOS / 4) +
            WCYC_PARA_ISTART));
    END PROCESS AD_WCYC_PARA;
	 
    AD_RCYC_STAT: PROCESS (FB_A_INT_DWA)
    BEGIN
        IF (((UNSIGNED(FB_A_INT_DWA)) >= RCYC_STAT_AOS / 4) AND
            ((UNSIGNED(FB_A_INT_DWA)) < (RCYC_STAT_AOS +
                                         RCYC_STAT_SIZE_B) / 4)) THEN
             RCYC_STAT_A <= '1';
        ELSE
             RCYC_STAT_A <= '0';
        END IF;
        RCYC_STAT_A_IDX <= (TO_INTEGER(
            (UNSIGNED(FB_A_INT_DWA)) -
            (RCYC_STAT_AOS / 4) +
            RCYC_STAT_ISTART));
    END PROCESS AD_RCYC_STAT;

    p_spi_read: PROCESS (X_RST, SPI_READ_SYNC, CLK_8)
    BEGIN
        IF (X_RST='0') THEN
            READ_TEMP <= '0';
            READ_LATCHED <= '0';
            FB_A_INT_LW_CHECK_RD(11 DOWNTO 0) <= X"FFF";
        ELSIF (Rising_Edge(CLK_8)) THEN
            READ_SYNC_LAST <= SPI_READ_SYNC;                           --Sync.
            IF ((SPI_READ_SYNC = '0') OR (READ_SYNC_LAST = '0')) THEN
                READ_TEMP <= '0';
                READ_LATCHED <= '0';
                READ_ADDR_ERROR <= '0';
                READ_NOT_LATCHED <= '0';
                SPI_READ_HW <= '0';
            ELSIF ((READ_TEMP = '1') AND (READ_LATCHED = '1')) THEN
                READ_TEMP <= '1';                                  --Finished
                READ_LATCHED <= '1';
            ELSE
                READ_TEMP <= '1';
                IF(READ_TEMP = '1') THEN
                    READ_LATCHED <= '1';
                ELSE
                    READ_LATCHED <= '0';
                END IF;

                IF (READ_TEMP = '0') THEN
						-- Always Read 32
						IF (FB_A_INT = X"000") THEN
							FB_SPI_RD_D_DW <=
									TEST_REGISTER_REG_RW;
						-- Always Read 32
						ELSIF ((FB_A_INT = X"004") OR
								(FB_A_INT = X"380")) THEN                     --Mirror
							FB_SPI_RD_D_DW <=
									NOT TEST_REGISTER_INV_REG_RW;                --Invert
						-- Always Read 32
						ELSIF (FB_A_INT = X"010") THEN
							FB_SPI_RD_D_DW <=
									FPGA_VERSIONx_REG_R(63 DOWNTO 32);
						-- Always Read 32
						ELSIF (FB_A_INT = X"014") THEN
							FB_SPI_RD_D_DW <=
									FPGA_VERSIONx_REG_R(31 DOWNTO 0);
						ELSIF (FB_A_INT = X"020") THEN
							FB_SPI_RD_D_DW_HW <= (OTHERS => '0');
							FB_SPI_RD_D_DW_LW <=
									BOARD_TEST_MODE_REG_RW;						
						ELSIF (FB_A_INT = X"032") THEN 
							FB_SPI_RD_D_DW_HW <= (OTHERS => '0');
							FB_SPI_RD_D_DW_LW <=FPGA_CTRL_REG_RW;							               
						ELSIF ((FB_A_INT = X"03A") OR
									(FB_A_INT = X"38E"))THEN  --Mirror
							FB_SPI_RD_D_DW_HW <= (OTHERS => '0');
							FB_SPI_RD_D_DW_LW <= FPGA_STAT_REG_RO;
						ELSIF (FB_A_INT = X"38B") THEN 
                    FB_SPI_RD_D_DW_HW <= (OTHERS => '0');
                    FB_SPI_RD_D_DW_LW_HB <= (OTHERS => '0');
                    FB_SPI_RD_D_DW_LW_LB <= OL_LATCH & PHASE_SIGN & "11";
						 --FB_SPI_RD_D_DW_LW_LB <= GPI(7 DOWNTO 0);
						ELSIF ((RCYC_STAT_A = '1') AND
								(FB_A_INT_IS_DWA = '1')) THEN
							IF ((RCYC_STAT_I16(RCYC_STAT_A_IDX)) = '1') THEN
									--16 Bit Access...
									FB_SPI_RD_D_DW_HW <= (OTHERS => '0');
									FB_SPI_RD_D_DW_LW <= RCYC_STAT_REG_RO(RCYC_STAT_A_IDX)(15 DOWNTO 0);
							ELSE
                        -- Or 32 Bit Access...
                        FB_SPI_RD_D_DW <= RCYC_STAT_REG_RO(RCYC_STAT_A_IDX);
							END IF;
                --Address Error
						ELSE
							FB_SPI_RD_D_DW <= (OTHERS => '0');
							READ_ADDR_ERROR <= '1';
						END IF;
                END IF;
            END IF;
        END IF;
    END PROCESS p_spi_read;


    p_spi_write: PROCESS (X_RST, SPI_WRITE_SYNC, CLK_8, CLK_1ms)
    BEGIN
        IF (X_RST='0') THEN
            WDOG_ERROR <= '1';
            CLK_1ms_LAST_1 <= '0';
            CLK_1ms_LAST_2 <= '0';
            CLK_1ms_LAST_3 <= '0';
            CLK_1ms_POS_STRB <= '0';
            CLK_1ms_NEG_STRB <= '0';
            WDOG_PWM <= '0';
            WDOG_ACK_PWM <= '1';
            i_WDOG_CTR_PWM <= ci_WDOG_TIMEOUT_MS;
            WDOG_FREQU <= '0';
            WDOG_ACK_FREQU <= '1';
            i_WDOG_CTR_FREQU <= ci_WDOG_TIMEOUT_MS;
            WRITE_TEMP <= '0';
            WRITE_LATCHED <= '0';
            FB_A_INT_LW_CHECK_WR(11 DOWNTO 0) <= X"FFF";
           

            TEST_REGISTER_REG_RW <= TEST_REGISTER_REG_RST;
            TEST_REGISTER_INV_REG_RW <= TEST_REGISTER_INV_REG_RST;
            BOARD_TEST_MODE_REG_RW <= BOARD_TEST_MODE_REG_RST;
            FPGA_CTRL_REG_RW <= FPGA_CTRL_REG_RST;
            GPO_REG_W <= GPO_REG_RST;

            FOR i IN WCYC_PARA_ISTART TO
                     (WCYC_PARA_ISTART+WCYC_PARA_SIZE-1) LOOP
                WCYC_PARA_REG_W(i) <= WCYC_PARA_RST_X;
            END LOOP;

        ELSIF (Rising_Edge(CLK_8)) THEN

            --Sync in to CLK_8
            CLK_1ms_LAST_3 <= CLK_1ms_LAST_2;
            CLK_1ms_LAST_2 <= CLK_1ms_LAST_1;
            CLK_1ms_LAST_1 <= CLK_1ms;
            IF ((CLK_1ms_LAST_2 = '1') AND (CLK_1ms_LAST_3 = '0')) THEN
                CLK_1ms_POS_STRB <= '1';                           --Strobe
            ELSIF ((CLK_1ms_LAST_2 = '0') AND (CLK_1ms_LAST_3 = '1')) THEN
                CLK_1ms_NEG_STRB <= '1';                           --Strobe
            END IF;

             --Pos. Edge
            IF (CLK_1ms_POS_STRB = '1') THEN
                CLK_1ms_POS_STRB <= '0';

                --Handle WDOG every 1ms
                IF (WDOG_PWM = WDOG_ACK_PWM) THEN
                    i_WDOG_CTR_PWM <= ci_WDOG_TIMEOUT_MS;
                    WDOG_PWM <= NOT WDOG_PWM;                      --Toggle
                ELSIF (i_WDOG_CTR_PWM > 0) THEN
                    i_WDOG_CTR_PWM <= i_WDOG_CTR_PWM-1;
                ELSE
                    i_WDOG_CTR_PWM <= i_WDOG_CTR_PWM;
                END IF;
                IF (WDOG_FREQU = WDOG_ACK_FREQU) THEN
                    i_WDOG_CTR_FREQU <= ci_WDOG_TIMEOUT_MS;
                    WDOG_FREQU <= NOT WDOG_FREQU;                  --Toggle
                ELSIF (i_WDOG_CTR_FREQU > 0) THEN
                    i_WDOG_CTR_FREQU <= i_WDOG_CTR_FREQU-1;
                ELSE
                    i_WDOG_CTR_FREQU <= i_WDOG_CTR_FREQU;
                END IF;
            END IF;

            --Neg. Edge
            IF (CLK_1ms_NEG_STRB = '1') THEN

                IF ((i_WDOG_CTR_PWM = 0) OR
                    (i_WDOG_CTR_FREQU = 0)) THEN
                    WDOG_ERROR <= '1';
                    CLK_1ms_NEG_STRB <= '0';
                ELSE
                    WDOG_ERROR <= '0';
                    CLK_1ms_NEG_STRB <= '0';
                END IF;
            END IF;

            --WRITE...
            WRITE_SYNC_LAST <= SPI_WRITE_SYNC;                         --Sync.
            IF ((SPI_WRITE_SYNC = '0') OR (WRITE_SYNC_LAST = '0')) THEN
                WRITE_TEMP <= '0';
                WRITE_LATCHED <= '0';
                WRITE_ADDR_ERROR <= '0';
            ELSIF ((WRITE_TEMP = '1') AND (WRITE_LATCHED = '1')) THEN
                WRITE_TEMP <= '1';                                 --Finished
                WRITE_LATCHED <= '1';
            ELSE
                WRITE_TEMP <= '1';
                IF(WRITE_TEMP = '1') THEN
                    WRITE_LATCHED <= '1';
                ELSE
                    WRITE_LATCHED <= '0';
                END IF;
					 
                IF (WRITE_TEMP = '0') THEN
               -- Always Write 32
						IF (FB_A_INT = X"000") THEN
							TEST_REGISTER_REG_RW <=
									FB_SPI_WR_D_DW;

                -- Always Write 32
						ELSIF ((FB_A_INT = X"004") OR
								(FB_A_INT = X"300")) THEN                     --Mirror
									TEST_REGISTER_INV_REG_RW <=
									FB_SPI_WR_D_DW;

                -- Always Write 32, Write 16
						ELSIF (FB_A_INT = X"020") THEN
                    
									BOARD_TEST_MODE_REG_RW <=
									"0000000000001111" AND      -- Bitmask
										FB_SPI_WR_D_DW_LW;

						-- Always Write 32, Write 16
						ELSIF (FB_A_INT = X"032") THEN 
									FPGA_CTRL_REG_RW <=
										"0000000000110001" AND      -- Bitmask
										FB_SPI_WR_D_DW_LW;
								--RESET Board Test Mode Register, Security
								BOARD_TEST_MODE_REG_RW <= BOARD_TEST_MODE_REG_RST;

						-- Always Write 32, Write 8
					 
						ELSIF (FB_A_INT = X"30B") THEN 
							--Preventing DDS STOP to be deactivated before FPGA RUN signal timeout.
							--The change DDS STOP from high to low can cause a sonics impulse response
								GPO_REG_W <=
											("000000" & FPGA_RUN_EN & '1') AND              -- Bitmask
											FB_SPI_WR_D_DW_LW_LB;
                -- Always Write 32, Write 16/32, direct, Array
                -- (16 Bit Access only at Low-Word(LW))
						ELSIF ((WCYC_PARA_A = '1') AND
								(FB_A_INT_IS_DWA = '1')) THEN
							IF ((WCYC_PARA_I16(WCYC_PARA_A_IDX)) = '1') THEN 
									IF (WCYC_PARA_A_IDX =
										WDOG_PWM_WCYC_PARA_IDX) THEN
										WDOG_ACK_PWM <= WDOG_PWM;              --Reset WDOG
									END IF;
										WCYC_PARA_REG_W(WCYC_PARA_A_IDX)(31 DOWNTO 16) <=
										(OTHERS => '0');
										WCYC_PARA_REG_W(WCYC_PARA_A_IDX)(15 DOWNTO 0) <=
										FB_SPI_WR_D_DW_LW;
									 
							ELSE
                        IF (WCYC_PARA_A_IDX =
                            WDOG_FREQU_WCYC_PARA_IDX) THEN
                            WDOG_ACK_FREQU <= WDOG_FREQU;          --Reset WDOG
                        END IF;
                        WCYC_PARA_REG_W(WCYC_PARA_A_IDX) <=
                            FB_SPI_WR_D_DW;
							END IF;
                --Address Error
						ELSE
							WRITE_ADDR_ERROR <= '1';
						END IF;
                END IF;                                                       
            END IF;
        END IF;
    END PROCESS p_spi_write;

-- READ / WRITE Register Assignment
-- (Except GPI, GPO and BTM Handling)

-- READ, Inputs
    p_assign_read: PROCESS (X_RST, CLK_8)

    --VAR!
    VARIABLE ADBUG_RCYC_S_FrequValOut_S42:  STD_LOGIC_VECTOR (41 DOWNTO 0);     --Temp. Signals, for AD-BUG, (>=AD15)

    BEGIN
        IF (X_RST='0') THEN

            FOR i IN RCYC_STAT_ISTART TO
                     (RCYC_STAT_ISTART+RCYC_STAT_SIZE-1) LOOP
                RCYC_STAT_REG_RO(i) <= RCYC_STAT_RST_X;
            END LOOP;

        ELSIF (Rising_Edge(CLK_8)) THEN

            --Assign
--            RCYC_STAT_REG_RO(8) <=
--                "00000000" & RCYC_S_FrequValOut(23 DOWNTO 0);
            --24*18=42-Bit-MUL / Division by Shift, Bitselect
            -- * 244140(18) / 262144 (2^20)
            -- Limited to Out[0.1Hz] up to 250KHz
            -- -> Out 22Bit
            ADBUG_RCYC_S_FrequValOut_S42(41 DOWNTO 0) :=                        --AD>=15 OK, Band-Aid... use VAR to avioid Delay!
                STD_LOGIC_VECTOR(RESIZE((
                    (UNSIGNED(RCYC_S_FrequValOut(23 DOWNTO 0)))
                    *244140)                       --MUL -> 48Bit...
                    ,42)
                    );
            RCYC_STAT_REG_RO(8)(31 DOWNTO 0) <=
                "0000000000" & (
                ADBUG_RCYC_S_FrequValOut_S42
                    (41 DOWNTO 20));
						 --RCYC_STAT_REG_RO(8)(31 DOWNTO 0) <= X"0000" & WCYC_PARA_REG_W(10)(15 DOWNTO 0); --TEST ONLY (PWM feedback attached to Freq Val Out address)
        END IF;
    END PROCESS p_assign_read;

-- WRITE, Outputs

    WCYC_P_CtrlHdx(15 DOWNTO 0) <= WCYC_PARA_REG_W(4)(15 DOWNTO 0);

	 WCYC_P_PWMVal(15 DOWNTO 0) <= WCYC_PARA_REG_W(10)(15 DOWNTO 0) WHEN ( WCYC_PARA_REG_W(10)(15 DOWNTO 0) <= X"F330");             --0xffff = 100%
		
    --LATCHED, may with Calculation
    p_assign_write: PROCESS (X_RST, CLK_8)

    --VAR!
    VARIABLE ADBUG_WCYC_P_FrequValIn_S43:   STD_LOGIC_VECTOR (42 DOWNTO 0);     --Temp. Signals, for AD-BUG, (>=AD15)
    BEGIN
        IF (X_RST='0') THEN

            --INIT (all Zero, Valid after first CLK_8 edge)

            WCYC_P_FrequValIn(23 DOWNTO 0) <= (OTHERS => '0');
            WCYC_P_PhaseValIn(15 DOWNTO 0) <= (OTHERS => '0');

            PWM_VAL_REG_EQ_ZERO <= '0';

        ELSIF (Rising_Edge(CLK_8)) THEN

            --Assign

            --22*21=43-Bit-MUL / Division by Shift, Bitselect
            -- * 1125900 / 262144 (2^18)
            -- Limited to In[0.1Hz] up to 250KHz
            -- --> Ignore Bit42 of Result -> Out 24Bit
            ADBUG_WCYC_P_FrequValIn_S43(42 DOWNTO 0) :=                         --AD>=15 OK, Band-Aid... use VAR to avioid Delay!
                STD_LOGIC_VECTOR(RESIZE((
                    (UNSIGNED(WCYC_PARA_REG_W(8)(21 DOWNTO 0)))
                    *1125900)                      --MUL -> 44Bit...
                    ,43)
                    );
            WCYC_P_FrequValIn(23 DOWNTO 0) <= (
                ADBUG_WCYC_P_FrequValIn_S43
                    (41 DOWNTO 18));
            --Detect PWMVal = 0
            IF (WCYC_PARA_REG_W(10)(15 DOWNTO 0) = X"0000") THEN
                PWM_VAL_REG_EQ_ZERO <= '1';
            ELSE
                PWM_VAL_REG_EQ_ZERO <= '0';
            END IF;

        END IF;
    END PROCESS p_assign_write;

p_OL_data: process(X_RST, CLK_8,OL_FUNCTION , OL_CURRENT , OL_VOLTAGE , OL_POWER , OL_TEMP)		
	begin	
	if(X_RST='0') THEN
		  OL_LATCH<="11111";
		  OLD_OL_LATCH<="11111";
	elsif(Rising_edge(CLK_8)) THEN
	
		if(OL_FUNCTION = '1' AND OL_CURRENT = '1' AND OL_VOLTAGE = '1' and
			OL_POWER = '1' AND OL_TEMP = '1') then
			OL_LATCH <= "11111";
		else
			if(OL_FUNCTION = '0') then
				OL_LATCH <= '0' & OLD_OL_LATCH(3 downto 0);
			end if;		
			if(OL_CURRENT = '0') then	
				OL_LATCH <= "00" & OLD_OL_LATCH(2 downto 0);
			end if;
			if(OL_VOLTAGE = '0') then
				OL_LATCH <= '0' & OLD_OL_LATCH(3) & '0' & OLD_OL_LATCH(1 downto 0);
			end if;
			if(OL_POWER = '0') then	
				OL_LATCH  <= '0' & OLD_OL_LATCH(3 downto 2) & '0' & OLD_OL_LATCH(0);
			end if;
			if(OL_TEMP = '0') then--			
				OL_LATCH <= '0' & OLD_OL_LATCH(3 downto 1) & '0';
			end if;			
		end if;
		OLD_OL_LATCH <= OL_LATCH;
	END IF;
end process;
-----

end Structure;

