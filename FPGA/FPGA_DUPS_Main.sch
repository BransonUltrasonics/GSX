<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3a" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="X_RST" />
        <signal name="CLK_M" />
        <signal name="CLK_4" />
        <signal name="CLK_8" />
        <signal name="CLK_1ms" />
        <signal name="CLK_DDS" />
        <signal name="HW_VERS_BD(7:0)" />
        <signal name="SPI_CLK" />
        <signal name="SPI_N_CS" />
        <signal name="SPI_I_MOSI" />
        <signal name="SPI_O_MISO" />
        <signal name="GPI(7:0)" />
        <signal name="GPI(1:0)" />
        <signal name="GPI(2)" />
        <signal name="GPI(3)" />
        <signal name="GPI(4)" />
        <signal name="GPI(5)" />
        <signal name="GPI(6)" />
        <signal name="GPO(7:0)" />
        <signal name="GPI(7)" />
        <signal name="GPIO(0)" />
        <signal name="GPIO(1)" />
        <signal name="GPIO(2)" />
        <signal name="GPIO(3)" />
        <signal name="GPIO(4)" />
        <signal name="GPO(1)" />
        <signal name="GPIO(6:0)" />
        <signal name="GPIO(5)" />
        <signal name="GPIO(6)" />
        <signal name="N_FMOT_to_FMOT" />
        <signal name="N_PWMB_to_PWMA" />
        <signal name="N_PWMA_to_PWMB" />
        <signal name="F" />
        <signal name="XLXN_168" />
        <signal name="FPGA_RUN" />
        <signal name="s_DSPI_ADC(4:0)" />
        <signal name="s_DSPI_ADC(1)" />
        <signal name="s_DSPI_ADC(2)" />
        <signal name="s_DSPI_ADC(3)" />
        <signal name="s_DSPI_ADC(4)" />
        <signal name="XLXN_204" />
        <signal name="GPO(0)" />
        <signal name="XLXN_221" />
        <signal name="XLXN_226" />
        <signal name="BTM" />
        <signal name="XLXN_232" />
        <signal name="ALIVE" />
        <signal name="GPLED1_5" />
        <signal name="XLXN_236" />
        <signal name="XLXN_241" />
        <signal name="XLXN_242" />
        <signal name="SYNC_DDS_RUN" />
        <signal name="SYNC_DDS_ERR" />
        <signal name="GPLED2_6" />
        <signal name="GPLED3_7" />
        <signal name="XLXN_259" />
        <signal name="XLXN_260" />
        <signal name="XLXN_267" />
        <signal name="XLXN_268" />
        <signal name="FBIF_TEST_1" />
        <signal name="FBIF_TEST_2" />
        <signal name="SUM31" />
        <signal name="DIFF31" />
        <signal name="XLXN_275" />
        <signal name="XLXN_276(15:0)" />
        <signal name="XLXN_277(23:0)" />
        <signal name="XLXN_278" />
        <signal name="XLXN_279" />
        <signal name="XLXN_281" />
        <signal name="XLXN_282" />
        <signal name="XLXN_283" />
        <signal name="XLXN_284" />
        <signal name="s_DSPI_ADC(0)" />
        <signal name="XLXN_294" />
        <signal name="XLXN_271" />
        <signal name="XLXN_298" />
        <signal name="XLXN_299(15:0)" />
        <signal name="XLXN_300" />
        <port polarity="Input" name="X_RST" />
        <port polarity="Input" name="CLK_M" />
        <port polarity="Input" name="CLK_4" />
        <port polarity="Input" name="CLK_8" />
        <port polarity="Input" name="CLK_1ms" />
        <port polarity="Input" name="CLK_DDS" />
        <port polarity="Input" name="HW_VERS_BD(7:0)" />
        <port polarity="Input" name="SPI_CLK" />
        <port polarity="Input" name="SPI_N_CS" />
        <port polarity="Input" name="SPI_I_MOSI" />
        <port polarity="Output" name="SPI_O_MISO" />
        <port polarity="BiDirectional" name="GPIO(6:0)" />
        <port polarity="Output" name="N_FMOT_to_FMOT" />
        <port polarity="Output" name="N_PWMB_to_PWMA" />
        <port polarity="Output" name="N_PWMA_to_PWMB" />
        <port polarity="Output" name="F" />
        <port polarity="Input" name="s_DSPI_ADC(4:0)" />
        <port polarity="Input" name="ALIVE" />
        <port polarity="Output" name="GPLED1_5" />
        <port polarity="Output" name="GPLED2_6" />
        <port polarity="Output" name="GPLED3_7" />
        <port polarity="Output" name="FBIF_TEST_1" />
        <port polarity="Output" name="FBIF_TEST_2" />
        <port polarity="Output" name="SUM31" />
        <port polarity="Output" name="DIFF31" />
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
        </blockdef>
        <blockdef name="gnd">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-96" x1="64" />
            <line x2="52" y1="-48" y2="-48" x1="76" />
            <line x2="60" y1="-32" y2="-32" x1="68" />
            <line x2="40" y1="-64" y2="-64" x1="88" />
            <line x2="64" y1="-64" y2="-80" x1="64" />
            <line x2="64" y1="-128" y2="-96" x1="64" />
        </blockdef>
        <blockdef name="buf">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="128" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="0" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="-64" x1="128" />
            <line x2="64" y1="-64" y2="0" x1="64" />
        </blockdef>
        <blockdef name="ERRSTAT">
            <timestamp>2021-3-17T17:14:5</timestamp>
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-192" y2="-192" x1="64" />
            <line x2="0" y1="-272" y2="-272" x1="64" />
            <line x2="112" y1="-384" y2="-432" x1="112" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="0" y1="-64" y2="-64" x1="64" />
            <line x2="608" y1="-48" y2="-48" x1="544" />
            <rect width="64" x="544" y="-60" height="24" />
            <rect width="480" x="64" y="-384" height="384" />
            <line x2="608" y1="-176" y2="-176" x1="544" />
            <line x2="608" y1="-144" y2="-144" x1="544" />
            <line x2="608" y1="-112" y2="-112" x1="544" />
        </blockdef>
        <blockdef name="m2_1">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="96" y1="-64" y2="-192" x1="96" />
            <line x2="96" y1="-96" y2="-64" x1="256" />
            <line x2="256" y1="-160" y2="-96" x1="256" />
            <line x2="256" y1="-192" y2="-160" x1="96" />
            <line x2="96" y1="-32" y2="-32" x1="176" />
            <line x2="176" y1="-80" y2="-32" x1="176" />
            <line x2="96" y1="-32" y2="-32" x1="0" />
            <line x2="256" y1="-128" y2="-128" x1="320" />
            <line x2="96" y1="-96" y2="-96" x1="0" />
            <line x2="96" y1="-160" y2="-160" x1="0" />
        </blockdef>
        <blockdef name="and2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-144" x1="64" />
        </blockdef>
        <blockdef name="uP_SPI_IF">
            <timestamp>2021-7-14T14:40:54</timestamp>
            <rect width="544" x="64" y="-1120" height="1080" />
            <line x2="0" y1="-704" y2="-704" x1="64" />
            <line x2="112" y1="-1120" y2="-1168" x1="112" />
            <line x2="144" y1="-1120" y2="-1168" x1="144" />
            <line x2="176" y1="-1120" y2="-1168" x1="176" />
            <line x2="208" y1="-1120" y2="-1168" x1="208" />
            <line x2="240" y1="-1168" y2="-1120" x1="240" />
            <line x2="272" y1="-1168" y2="-1120" x1="272" />
            <line x2="68" y1="-944" y2="-944" x1="4" />
            <line x2="64" y1="-912" y2="-912" x1="0" />
            <line x2="0" y1="-816" y2="-816" x1="64" />
            <rect width="64" x="0" y="-828" height="24" />
            <line x2="0" y1="-672" y2="-672" x1="64" />
            <line x2="0" y1="-640" y2="-640" x1="64" />
            <line x2="64" y1="-608" y2="-608" x1="0" />
            <line x2="0" y1="-528" y2="-528" x1="64" />
            <rect width="64" x="0" y="-540" height="24" />
            <line x2="0" y1="-496" y2="-496" x1="64" />
            <rect width="64" x="0" y="-508" height="24" />
            <line x2="0" y1="-464" y2="-464" x1="64" />
            <rect width="64" x="0" y="-476" height="24" />
            <line x2="0" y1="-384" y2="-384" x1="64" />
            <rect width="64" x="0" y="-396" height="24" />
            <line x2="64" y1="-336" y2="-336" x1="0" />
            <line x2="64" y1="-304" y2="-304" x1="0" />
            <line x2="64" y1="-272" y2="-272" x1="0" />
            <line x2="64" y1="-224" y2="-224" x1="0" />
            <rect width="64" x="0" y="-236" height="24" />
            <line x2="64" y1="-176" y2="-176" x1="0" />
            <rect width="64" x="0" y="-188" height="24" />
            <line x2="304" y1="-40" y2="0" x1="304" />
            <line x2="672" y1="-352" y2="-352" x1="608" />
            <rect width="64" x="608" y="-364" height="24" />
            <line x2="672" y1="-288" y2="-288" x1="608" />
            <rect width="64" x="608" y="-300" height="24" />
        </blockdef>
        <blockdef name="copy_of_and2">
            <timestamp>2021-3-16T18:45:5</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-144" x1="64" />
            <line x2="112" y1="-192" y2="-144" x1="112" />
            <line x2="112" y1="-144" y2="-128" x1="104" />
            <line x2="120" y1="-128" y2="-144" x1="112" />
        </blockdef>
        <blockdef name="or2clk">
            <timestamp>2021-3-30T22:11:31</timestamp>
            <arc ex="48" ey="-192" sx="52" sy="-96" r="101" cx="-41" cy="-141" />
            <arc ex="208" ey="-144" sx="112" sy="-96" r="120" cx="110" cy="-218" />
            <line x2="116" y1="-192" y2="-192" x1="48" />
            <line x2="56" y1="-176" y2="-176" x1="0" />
            <line x2="208" y1="-144" y2="-144" x1="272" />
            <arc ex="116" ey="-192" sx="208" sy="-144" r="130" cx="105" cy="-62" />
            <line x2="60" y1="-112" y2="-112" x1="0" />
            <line x2="116" y1="-96" y2="-96" x1="52" />
            <line x2="104" y1="-112" y2="-96" x1="112" />
            <line x2="120" y1="-112" y2="-96" x1="112" />
            <line x2="112" y1="-96" y2="-64" x1="112" />
        </blockdef>
        <blockdef name="PWM_DDS">
            <timestamp>2021-7-17T13:49:54</timestamp>
            <rect width="384" x="64" y="-320" height="320" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="512" y1="-64" y2="-64" x1="448" />
            <line x2="512" y1="-128" y2="-128" x1="448" />
            <line x2="512" y1="-192" y2="-192" x1="448" />
            <line x2="512" y1="-256" y2="-256" x1="448" />
            <rect width="64" x="0" y="-76" height="24" />
            <line x2="0" y1="-64" y2="-64" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-256" y2="-256" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
        </blockdef>
        <block symbolname="gnd" name="XLXI_23(1:0)">
            <blockpin signalname="GPI(1:0)" name="G" />
        </block>
        <block symbolname="buf" name="XLXI_28">
            <blockpin signalname="s_DSPI_ADC(4)" name="I" />
            <blockpin signalname="GPI(2)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_29">
            <blockpin signalname="GPIO(0)" name="I" />
            <blockpin signalname="GPI(3)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_30">
            <blockpin signalname="GPIO(1)" name="I" />
            <blockpin signalname="GPI(4)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_31">
            <blockpin signalname="GPIO(2)" name="I" />
            <blockpin signalname="GPI(5)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_32">
            <blockpin signalname="GPIO(3)" name="I" />
            <blockpin signalname="GPI(6)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_35">
            <blockpin signalname="GPIO(4)" name="I" />
            <blockpin signalname="GPI(7)" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_36">
            <blockpin signalname="GPO(1)" name="I" />
            <blockpin signalname="XLXN_204" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_37">
            <blockpin signalname="GPO(0)" name="I" />
            <blockpin signalname="GPIO(5)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_38">
            <blockpin signalname="XLXN_226" name="I" />
            <blockpin signalname="GPIO(6)" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_6">
            <blockpin signalname="XLXN_284" name="I" />
            <blockpin signalname="N_FMOT_to_FMOT" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_5">
            <blockpin signalname="XLXN_283" name="I" />
            <blockpin signalname="N_PWMB_to_PWMA" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_4">
            <blockpin signalname="XLXN_281" name="I" />
            <blockpin signalname="N_PWMA_to_PWMB" name="O" />
        </block>
        <block symbolname="ERRSTAT" name="XLXI_63">
            <blockpin name="FSYNC_VAL_VALID" />
            <blockpin name="PHA_VAL_VALID" />
            <blockpin signalname="XLXN_232" name="MD" />
            <blockpin signalname="FPGA_RUN" name="RST" />
            <blockpin signalname="XLXN_204" name="PWM_STOP_CF" />
            <blockpin signalname="GPO(0)" name="GPO_OL_RESET" />
            <blockpin name="S_StatHdx(15:0)" />
            <blockpin signalname="SYNC_DDS_RUN" name="SYNC_DDS_RUN" />
            <blockpin signalname="SYNC_DDS_ERR" name="SYNC_DDS_ERR" />
            <blockpin signalname="XLXN_221" name="PWM_STOP" />
        </block>
        <block symbolname="gnd" name="XLXI_66">
            <blockpin signalname="XLXN_232" name="G" />
        </block>
        <block symbolname="buf" name="XLXI_75">
            <blockpin signalname="ALIVE" name="I" />
            <blockpin signalname="GPLED1_5" name="O" />
        </block>
        <block symbolname="m2_1" name="XLXI_71">
            <blockpin signalname="XLXN_242" name="D0" />
            <blockpin signalname="XLXN_267" name="D1" />
            <blockpin signalname="XLXN_241" name="S0" />
            <blockpin signalname="XLXN_259" name="O" />
        </block>
        <block symbolname="m2_1" name="XLXI_72">
            <blockpin signalname="X_RST" name="D0" />
            <blockpin signalname="XLXN_268" name="D1" />
            <blockpin signalname="XLXN_241" name="S0" />
            <blockpin signalname="XLXN_260" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_79">
            <blockpin signalname="ALIVE" name="I" />
            <blockpin signalname="XLXN_236" name="O" />
        </block>
        <block symbolname="m2_1" name="XLXI_80">
            <blockpin signalname="XLXN_259" name="D0" />
            <blockpin signalname="XLXN_236" name="D1" />
            <blockpin signalname="BTM" name="S0" />
            <blockpin signalname="GPLED2_6" name="O" />
        </block>
        <block symbolname="m2_1" name="XLXI_81">
            <blockpin signalname="XLXN_260" name="D0" />
            <blockpin signalname="XLXN_236" name="D1" />
            <blockpin signalname="BTM" name="S0" />
            <blockpin signalname="GPLED3_7" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_82">
            <blockpin signalname="FPGA_RUN" name="I" />
            <blockpin signalname="XLXN_242" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_83">
            <blockpin signalname="XLXN_236" name="I0" />
            <blockpin signalname="SYNC_DDS_ERR" name="I1" />
            <blockpin signalname="XLXN_267" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_84">
            <blockpin signalname="XLXN_236" name="I0" />
            <blockpin signalname="SYNC_DDS_RUN" name="I1" />
            <blockpin signalname="XLXN_268" name="O" />
        </block>
        <block symbolname="gnd" name="XLXI_85">
            <blockpin signalname="XLXN_241" name="G" />
        </block>
        <block symbolname="copy_of_and2" name="XLXI_90">
            <blockpin signalname="X_RST" name="I0" />
            <blockpin signalname="XLXN_168" name="I1" />
            <blockpin signalname="FPGA_RUN" name="O" />
            <blockpin signalname="CLK_8" name="C" />
        </block>
        <block symbolname="or2clk" name="XLXI_95">
            <blockpin signalname="XLXN_221" name="I1" />
            <blockpin signalname="XLXN_271" name="O" />
            <blockpin signalname="XLXN_204" name="I0" />
            <blockpin signalname="CLK_8" name="C" />
        </block>
        <block symbolname="uP_SPI_IF" name="XLXI_104">
            <blockpin name="ADC_ERROR" />
            <blockpin signalname="CLK_1ms" name="CLK_1ms" />
            <blockpin signalname="CLK_4" name="CLK_4" />
            <blockpin signalname="CLK_8" name="CLK_8" />
            <blockpin signalname="CLK_M" name="CLK_M" />
            <blockpin signalname="GPI(7:0)" name="GPI(7:0)" />
            <blockpin signalname="HW_VERS_BD(7:0)" name="HW_VERS_BD(7:0)" />
            <blockpin name="RCYC_STAT_FrequValOut(23:0)" />
            <blockpin name="RCYC_S_PhaseValOut(15:0)" />
            <blockpin name="RCYC_S_StatHdx(15:0)" />
            <blockpin signalname="SPI_CLK" name="SPI_CLK" />
            <blockpin signalname="SPI_I_MOSI" name="SPI_I_MOSI" />
            <blockpin signalname="SPI_N_CS" name="SPI_N_CS" />
            <blockpin signalname="X_RST" name="X_RST" />
            <blockpin name="BTM_GPO(7:0)" />
            <blockpin signalname="BTM" name="BTM" />
            <blockpin name="BTM_PWMA" />
            <blockpin name="BTM_PWMB" />
            <blockpin signalname="FBIF_TEST_1" name="FBIF_TEST_1" />
            <blockpin signalname="FBIF_TEST_2" name="FBIF_TEST_2" />
            <blockpin signalname="XLXN_168" name="FPGA_RUN" />
            <blockpin signalname="XLXN_277(23:0)" name="FrequVal(23:0)" />
            <blockpin signalname="GPO(7:0)" name="GPO(7:0)" />
            <blockpin signalname="XLXN_276(15:0)" name="Pwm_Val(15:0)" />
            <blockpin signalname="SPI_O_MISO" name="SPI_O_MISO" />
        </block>
        <block symbolname="buf" name="XLXI_134">
            <blockpin signalname="XLXN_275" name="I" />
            <blockpin signalname="SUM31" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_135">
            <blockpin signalname="XLXN_275" name="I" />
            <blockpin signalname="DIFF31" name="O" />
        </block>
        <block symbolname="gnd" name="XLXI_136">
            <blockpin signalname="XLXN_275" name="G" />
        </block>
        <block symbolname="PWM_DDS" name="XLXI_137">
            <blockpin signalname="CLK_DDS" name="CLK_DDS" />
            <blockpin signalname="XLXN_284" name="Motion" />
            <blockpin signalname="XLXN_283" name="PhaseA" />
            <blockpin signalname="XLXN_281" name="PhaseB" />
            <blockpin signalname="F" name="Quad" />
            <blockpin signalname="XLXN_276(15:0)" name="Pwm_Val(15:0)" />
            <blockpin signalname="XLXN_277(23:0)" name="Freq_Val(23:0)" />
            <blockpin signalname="FPGA_RUN" name="X_RST" />
            <blockpin signalname="CLK_8" name="CLK_8" />
            <blockpin signalname="GPO(1)" name="SONICS_ON" />
        </block>
        <block symbolname="m2_1" name="XLXI_65">
            <blockpin signalname="XLXN_271" name="D0" />
            <blockpin signalname="XLXN_204" name="D1" />
            <blockpin signalname="BTM" name="S0" />
            <blockpin signalname="XLXN_226" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="5440" height="3520">
        <branch name="X_RST">
            <wire x2="1024" y1="528" y2="528" x1="240" />
            <wire x2="1296" y1="528" y2="528" x1="1024" />
            <wire x2="1296" y1="528" y2="624" x1="1296" />
            <wire x2="1136" y1="96" y2="96" x1="1024" />
            <wire x2="1024" y1="96" y2="528" x1="1024" />
        </branch>
        <branch name="CLK_M">
            <wire x2="1360" y1="480" y2="480" x1="240" />
            <wire x2="1360" y1="480" y2="624" x1="1360" />
        </branch>
        <branch name="CLK_4">
            <wire x2="1392" y1="432" y2="432" x1="240" />
            <wire x2="1392" y1="432" y2="624" x1="1392" />
        </branch>
        <iomarker fontsize="28" x="240" y="480" name="CLK_M" orien="R180" />
        <iomarker fontsize="28" x="240" y="528" name="X_RST" orien="R180" />
        <iomarker fontsize="28" x="240" y="432" name="CLK_4" orien="R180" />
        <iomarker fontsize="28" x="240" y="368" name="CLK_8" orien="R180" />
        <branch name="CLK_1ms">
            <wire x2="1456" y1="304" y2="304" x1="272" />
            <wire x2="1456" y1="304" y2="624" x1="1456" />
        </branch>
        <iomarker fontsize="28" x="272" y="304" name="CLK_1ms" orien="R180" />
        <iomarker fontsize="28" x="272" y="240" name="CLK_DDS" orien="R180" />
        <branch name="HW_VERS_BD(7:0)">
            <wire x2="352" y1="960" y2="960" x1="336" />
            <wire x2="352" y1="960" y2="976" x1="352" />
            <wire x2="1184" y1="976" y2="976" x1="352" />
        </branch>
        <iomarker fontsize="28" x="336" y="960" name="HW_VERS_BD(7:0)" orien="R180" />
        <branch name="SPI_N_CS">
            <wire x2="1184" y1="1120" y2="1120" x1="224" />
        </branch>
        <iomarker fontsize="28" x="208" y="1072" name="SPI_CLK" orien="R180" />
        <iomarker fontsize="28" x="224" y="1120" name="SPI_N_CS" orien="R180" />
        <branch name="SPI_I_MOSI">
            <wire x2="1184" y1="1168" y2="1168" x1="240" />
            <wire x2="1184" y1="1152" y2="1168" x1="1184" />
        </branch>
        <iomarker fontsize="28" x="240" y="1168" name="SPI_I_MOSI" orien="R180" />
        <branch name="SPI_O_MISO">
            <wire x2="704" y1="1216" y2="1216" x1="224" />
            <wire x2="704" y1="1184" y2="1216" x1="704" />
            <wire x2="1184" y1="1184" y2="1184" x1="704" />
        </branch>
        <iomarker fontsize="28" x="224" y="1216" name="SPI_O_MISO" orien="R180" />
        <bustap x2="736" y1="1984" y2="1984" x1="832" />
        <instance x="416" y="1920" name="XLXI_23(1:0)" orien="R90" />
        <branch name="GPI(1:0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="630" y="1984" type="branch" />
            <wire x2="736" y1="1984" y2="1984" x1="544" />
        </branch>
        <branch name="GPI(7:0)">
            <wire x2="896" y1="1984" y2="1984" x1="832" />
            <wire x2="896" y1="1984" y2="2064" x1="896" />
            <wire x2="896" y1="2064" y2="2128" x1="896" />
            <wire x2="896" y1="2128" y2="2192" x1="896" />
            <wire x2="896" y1="2192" y2="2256" x1="896" />
            <wire x2="896" y1="2256" y2="2304" x1="896" />
            <wire x2="896" y1="2304" y2="2368" x1="896" />
            <wire x2="896" y1="2368" y2="2384" x1="896" />
            <wire x2="1184" y1="1408" y2="1408" x1="896" />
            <wire x2="896" y1="1408" y2="1984" x1="896" />
        </branch>
        <instance x="544" y="2096" name="XLXI_28" orien="R0" />
        <instance x="544" y="2160" name="XLXI_29" orien="R0" />
        <instance x="544" y="2224" name="XLXI_30" orien="R0" />
        <instance x="544" y="2288" name="XLXI_31" orien="R0" />
        <instance x="544" y="2352" name="XLXI_32" orien="R0" />
        <bustap x2="800" y1="2064" y2="2064" x1="896" />
        <bustap x2="800" y1="2128" y2="2128" x1="896" />
        <bustap x2="800" y1="2192" y2="2192" x1="896" />
        <bustap x2="800" y1="2256" y2="2256" x1="896" />
        <bustap x2="800" y1="2304" y2="2304" x1="896" />
        <branch name="GPI(2)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="785" y="2064" type="branch" />
            <wire x2="800" y1="2064" y2="2064" x1="768" />
        </branch>
        <branch name="GPI(3)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="785" y="2128" type="branch" />
            <wire x2="800" y1="2128" y2="2128" x1="768" />
        </branch>
        <branch name="GPI(4)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="784" y="2192" type="branch" />
            <wire x2="784" y1="2192" y2="2192" x1="768" />
            <wire x2="800" y1="2192" y2="2192" x1="784" />
        </branch>
        <branch name="GPI(5)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="784" y="2256" type="branch" />
            <wire x2="784" y1="2256" y2="2256" x1="768" />
            <wire x2="800" y1="2256" y2="2256" x1="784" />
        </branch>
        <branch name="GPI(6)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="784" y="2304" type="branch" />
            <wire x2="784" y1="2320" y2="2320" x1="768" />
            <wire x2="784" y1="2304" y2="2320" x1="784" />
            <wire x2="800" y1="2304" y2="2304" x1="784" />
        </branch>
        <bustap x2="800" y1="2368" y2="2368" x1="896" />
        <instance x="544" y="2416" name="XLXI_35" orien="R0" />
        <branch name="GPI(7)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="784" y="2368" type="branch" />
            <wire x2="784" y1="2384" y2="2384" x1="768" />
            <wire x2="784" y1="2368" y2="2384" x1="784" />
            <wire x2="800" y1="2368" y2="2368" x1="784" />
        </branch>
        <branch name="GPIO(0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="473" y="2128" type="branch" />
            <wire x2="544" y1="2128" y2="2128" x1="400" />
        </branch>
        <branch name="GPIO(1)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="448" y="2160" type="branch" />
            <wire x2="448" y1="2160" y2="2160" x1="400" />
            <wire x2="544" y1="2160" y2="2160" x1="448" />
            <wire x2="544" y1="2160" y2="2192" x1="544" />
        </branch>
        <branch name="GPIO(2)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="416" y="2208" type="branch" />
            <wire x2="400" y1="2192" y2="2208" x1="400" />
            <wire x2="416" y1="2208" y2="2208" x1="400" />
            <wire x2="544" y1="2208" y2="2208" x1="416" />
            <wire x2="544" y1="2208" y2="2256" x1="544" />
        </branch>
        <branch name="GPIO(3)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="496" y="2272" type="branch" />
            <wire x2="464" y1="2224" y2="2224" x1="400" />
            <wire x2="464" y1="2224" y2="2272" x1="464" />
            <wire x2="496" y1="2272" y2="2272" x1="464" />
            <wire x2="544" y1="2272" y2="2272" x1="496" />
            <wire x2="544" y1="2272" y2="2320" x1="544" />
        </branch>
        <bustap x2="1104" y1="1936" y2="1936" x1="1008" />
        <bustap x2="1104" y1="1984" y2="1984" x1="1008" />
        <branch name="GPO(1)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:24;fontname:Arial" attrname="Name" x="1168" y="1984" type="branch" />
            <wire x2="1168" y1="1984" y2="1984" x1="1104" />
            <wire x2="1328" y1="1984" y2="1984" x1="1168" />
        </branch>
        <instance x="1328" y="2016" name="XLXI_36" orien="R0" />
        <branch name="GPIO(4)">
            <attrtext style="alignment:SOFT-TCENTER;fontsize:20;fontname:Arial" attrname="Name" x="528" y="2384" type="branch" />
            <wire x2="448" y1="2256" y2="2256" x1="400" />
            <wire x2="448" y1="2256" y2="2304" x1="448" />
            <wire x2="528" y1="2304" y2="2304" x1="448" />
            <wire x2="528" y1="2304" y2="2384" x1="528" />
            <wire x2="544" y1="2384" y2="2384" x1="528" />
        </branch>
        <iomarker fontsize="28" x="224" y="2112" name="GPIO(6:0)" orien="R180" />
        <bustap x2="400" y1="2128" y2="2128" x1="304" />
        <bustap x2="400" y1="2160" y2="2160" x1="304" />
        <bustap x2="400" y1="2192" y2="2192" x1="304" />
        <bustap x2="400" y1="2224" y2="2224" x1="304" />
        <bustap x2="400" y1="2256" y2="2256" x1="304" />
        <bustap x2="400" y1="2288" y2="2288" x1="304" />
        <bustap x2="400" y1="2320" y2="2320" x1="304" />
        <branch name="GPIO(6:0)">
            <wire x2="304" y1="2112" y2="2112" x1="224" />
            <wire x2="304" y1="2112" y2="2128" x1="304" />
            <wire x2="304" y1="2128" y2="2160" x1="304" />
            <wire x2="304" y1="2160" y2="2192" x1="304" />
            <wire x2="304" y1="2192" y2="2224" x1="304" />
            <wire x2="304" y1="2224" y2="2256" x1="304" />
            <wire x2="304" y1="2256" y2="2288" x1="304" />
            <wire x2="304" y1="2288" y2="2320" x1="304" />
            <wire x2="304" y1="2320" y2="2336" x1="304" />
        </branch>
        <instance x="768" y="2448" name="XLXI_37" orien="R180" />
        <instance x="768" y="2544" name="XLXI_38" orien="R180" />
        <branch name="GPIO(5)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="496" y="2480" type="branch" />
            <wire x2="432" y1="2288" y2="2288" x1="400" />
            <wire x2="432" y1="2288" y2="2480" x1="432" />
            <wire x2="496" y1="2480" y2="2480" x1="432" />
            <wire x2="544" y1="2480" y2="2480" x1="496" />
        </branch>
        <text x="780" y="2460">OL_RESET</text>
        <branch name="GPIO(6)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="466" y="2576" type="branch" />
            <wire x2="416" y1="2320" y2="2320" x1="400" />
            <wire x2="416" y1="2320" y2="2576" x1="416" />
            <wire x2="544" y1="2576" y2="2576" x1="416" />
        </branch>
        <branch name="N_FMOT_to_FMOT">
            <wire x2="3744" y1="1584" y2="1584" x1="3264" />
        </branch>
        <branch name="N_PWMB_to_PWMA">
            <wire x2="3264" y1="1472" y2="1472" x1="3248" />
            <wire x2="3728" y1="1456" y2="1456" x1="3264" />
            <wire x2="3264" y1="1456" y2="1472" x1="3264" />
        </branch>
        <branch name="N_PWMA_to_PWMB">
            <wire x2="3232" y1="1376" y2="1376" x1="3216" />
            <wire x2="3728" y1="1360" y2="1360" x1="3232" />
            <wire x2="3232" y1="1360" y2="1376" x1="3232" />
        </branch>
        <instance x="3040" y="1616" name="XLXI_6" orien="R0" />
        <instance x="3024" y="1504" name="XLXI_5" orien="R0" />
        <instance x="2992" y="1408" name="XLXI_4" orien="R0" />
        <iomarker fontsize="28" x="3728" y="1296" name="F" orien="R0" />
        <iomarker fontsize="28" x="3744" y="1584" name="N_FMOT_to_FMOT" orien="R0" />
        <iomarker fontsize="28" x="3728" y="1456" name="N_PWMB_to_PWMA" orien="R0" />
        <iomarker fontsize="28" x="3728" y="1360" name="N_PWMA_to_PWMB" orien="R0" />
        <branch name="XLXN_168">
            <wire x2="1088" y1="160" y2="576" x1="1088" />
            <wire x2="1328" y1="576" y2="576" x1="1088" />
            <wire x2="1328" y1="576" y2="624" x1="1328" />
            <wire x2="1136" y1="160" y2="160" x1="1088" />
        </branch>
        <iomarker fontsize="28" x="320" y="2720" name="s_DSPI_ADC(4:0)" orien="R180" />
        <branch name="s_DSPI_ADC(4:0)">
            <wire x2="688" y1="2720" y2="2720" x1="320" />
            <wire x2="688" y1="2720" y2="2736" x1="688" />
            <wire x2="688" y1="2736" y2="2784" x1="688" />
            <wire x2="688" y1="2784" y2="2832" x1="688" />
            <wire x2="688" y1="2832" y2="2880" x1="688" />
            <wire x2="688" y1="2880" y2="2928" x1="688" />
            <wire x2="688" y1="2928" y2="2976" x1="688" />
        </branch>
        <bustap x2="784" y1="2736" y2="2736" x1="688" />
        <bustap x2="784" y1="2784" y2="2784" x1="688" />
        <bustap x2="784" y1="2832" y2="2832" x1="688" />
        <bustap x2="784" y1="2880" y2="2880" x1="688" />
        <bustap x2="784" y1="2928" y2="2928" x1="688" />
        <instance x="3312" y="896" name="XLXI_63" orien="R0">
        </instance>
        <text style="fontsize:24;fontname:Arial" x="1320" y="1920">OL_RESET</text>
        <text style="fontsize:20;fontname:Arial" x="772" y="2560">DDS_STOP</text>
        <instance x="3120" y="768" name="XLXI_66" orien="R0" />
        <branch name="XLXN_232">
            <wire x2="3312" y1="624" y2="624" x1="3184" />
            <wire x2="3184" y1="624" y2="640" x1="3184" />
        </branch>
        <branch name="ALIVE">
            <wire x2="4800" y1="2176" y2="2176" x1="4688" />
            <wire x2="4960" y1="2176" y2="2176" x1="4800" />
            <wire x2="4800" y1="2176" y2="2304" x1="4800" />
            <wire x2="4800" y1="2304" y2="2304" x1="4736" />
        </branch>
        <iomarker fontsize="28" x="4688" y="2176" name="ALIVE" orien="R180" />
        <instance x="4960" y="2208" name="XLXI_75" orien="R0" />
        <branch name="GPLED1_5">
            <wire x2="5216" y1="2176" y2="2176" x1="5184" />
        </branch>
        <iomarker fontsize="28" x="5216" y="2176" name="GPLED1_5" orien="R0" />
        <instance x="4032" y="2544" name="XLXI_71" orien="R0" />
        <instance x="4032" y="2752" name="XLXI_72" orien="R0" />
        <instance x="4736" y="2272" name="XLXI_79" orien="R180" />
        <instance x="4656" y="2896" name="XLXI_80" orien="R0" />
        <instance x="4672" y="3168" name="XLXI_81" orien="R0" />
        <branch name="BTM">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="4304" y="3136" type="branch" />
            <wire x2="4560" y1="3136" y2="3136" x1="4304" />
            <wire x2="4672" y1="3136" y2="3136" x1="4560" />
            <wire x2="4656" y1="2864" y2="2864" x1="4560" />
            <wire x2="4560" y1="2864" y2="3136" x1="4560" />
        </branch>
        <branch name="XLXN_242">
            <wire x2="4032" y1="2384" y2="2384" x1="3920" />
        </branch>
        <branch name="FPGA_RUN">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3632" y="2384" type="branch" />
            <wire x2="3696" y1="2384" y2="2384" x1="3632" />
        </branch>
        <instance x="3648" y="2640" name="XLXI_83" orien="R0" />
        <instance x="3648" y="2752" name="XLXI_84" orien="R0" />
        <branch name="SYNC_DDS_RUN">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="4096" y="720" type="branch" />
            <wire x2="4096" y1="720" y2="720" x1="3920" />
            <wire x2="4272" y1="720" y2="720" x1="4096" />
        </branch>
        <branch name="SYNC_DDS_ERR">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="4176" y="752" type="branch" />
            <wire x2="4176" y1="752" y2="752" x1="3920" />
            <wire x2="4272" y1="752" y2="752" x1="4176" />
        </branch>
        <branch name="SYNC_DDS_ERR">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3312" y="2512" type="branch" />
            <wire x2="3648" y1="2512" y2="2512" x1="3312" />
        </branch>
        <branch name="SYNC_DDS_RUN">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3312" y="2624" type="branch" />
            <wire x2="3648" y1="2624" y2="2624" x1="3312" />
        </branch>
        <instance x="3776" y="2928" name="XLXI_85" orien="R0" />
        <branch name="GPLED2_6">
            <wire x2="5184" y1="2768" y2="2768" x1="4976" />
        </branch>
        <iomarker fontsize="28" x="5184" y="2768" name="GPLED2_6" orien="R0" />
        <branch name="GPLED3_7">
            <wire x2="5184" y1="3040" y2="3040" x1="4992" />
        </branch>
        <iomarker fontsize="28" x="5184" y="3040" name="GPLED3_7" orien="R0" />
        <text style="fontsize:24;fontname:Arial" x="3856" y="2828">&gt;(STD_DDS)</text>
        <instance x="1136" y="32" name="XLXI_90" orien="M180">
        </instance>
        <branch name="CLK_8">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2864" y="1920" type="branch" />
            <wire x2="3008" y1="1920" y2="1920" x1="2864" />
            <wire x2="3008" y1="1920" y2="1936" x1="3008" />
            <wire x2="3088" y1="1936" y2="1936" x1="3008" />
            <wire x2="3088" y1="1936" y2="1968" x1="3088" />
        </branch>
        <branch name="XLXN_204">
            <wire x2="1664" y1="1984" y2="1984" x1="1552" />
            <wire x2="1664" y1="1984" y2="2128" x1="1664" />
            <wire x2="2736" y1="2128" y2="2128" x1="1664" />
            <wire x2="2736" y1="2128" y2="2160" x1="2736" />
            <wire x2="3280" y1="2160" y2="2160" x1="2736" />
            <wire x2="1664" y1="2128" y2="2128" x1="1600" />
            <wire x2="3280" y1="2016" y2="2016" x1="3200" />
            <wire x2="3280" y1="2016" y2="2160" x1="3280" />
            <wire x2="3312" y1="848" y2="848" x1="3280" />
            <wire x2="3312" y1="848" y2="864" x1="3312" />
            <wire x2="3280" y1="848" y2="2016" x1="3280" />
        </branch>
        <branch name="XLXN_221">
            <wire x2="3392" y1="2080" y2="2080" x1="3200" />
            <wire x2="4112" y1="1088" y2="1088" x1="3392" />
            <wire x2="3392" y1="1088" y2="2080" x1="3392" />
            <wire x2="4112" y1="784" y2="784" x1="3920" />
            <wire x2="4112" y1="784" y2="1088" x1="4112" />
        </branch>
        <instance x="3200" y="1904" name="XLXI_95" orien="R180">
        </instance>
        <branch name="XLXN_236">
            <wire x2="3440" y1="2272" y2="2576" x1="3440" />
            <wire x2="3648" y1="2576" y2="2576" x1="3440" />
            <wire x2="3440" y1="2576" y2="2688" x1="3440" />
            <wire x2="3648" y1="2688" y2="2688" x1="3440" />
            <wire x2="4496" y1="2272" y2="2272" x1="3440" />
            <wire x2="4496" y1="2272" y2="2304" x1="4496" />
            <wire x2="4512" y1="2304" y2="2304" x1="4496" />
            <wire x2="4496" y1="2304" y2="2304" x1="4448" />
            <wire x2="4448" y1="2304" y2="2800" x1="4448" />
            <wire x2="4656" y1="2800" y2="2800" x1="4448" />
            <wire x2="4448" y1="2800" y2="3072" x1="4448" />
            <wire x2="4672" y1="3072" y2="3072" x1="4448" />
        </branch>
        <branch name="XLXN_259">
            <wire x2="4496" y1="2416" y2="2416" x1="4352" />
            <wire x2="4496" y1="2416" y2="2736" x1="4496" />
            <wire x2="4656" y1="2736" y2="2736" x1="4496" />
        </branch>
        <branch name="XLXN_260">
            <wire x2="4480" y1="2624" y2="2624" x1="4352" />
            <wire x2="4480" y1="2624" y2="3008" x1="4480" />
            <wire x2="4672" y1="3008" y2="3008" x1="4480" />
        </branch>
        <branch name="XLXN_241">
            <wire x2="3840" y1="2720" y2="2800" x1="3840" />
            <wire x2="3952" y1="2720" y2="2720" x1="3840" />
            <wire x2="4032" y1="2720" y2="2720" x1="3952" />
            <wire x2="4032" y1="2512" y2="2512" x1="3952" />
            <wire x2="3952" y1="2512" y2="2720" x1="3952" />
        </branch>
        <instance x="3696" y="2416" name="XLXI_82" orien="R0" />
        <branch name="X_RST">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3632" y="2448" type="branch" />
            <wire x2="3936" y1="2448" y2="2448" x1="3632" />
            <wire x2="3936" y1="2448" y2="2592" x1="3936" />
            <wire x2="4032" y1="2592" y2="2592" x1="3936" />
        </branch>
        <branch name="XLXN_267">
            <wire x2="3968" y1="2544" y2="2544" x1="3904" />
            <wire x2="3968" y1="2448" y2="2544" x1="3968" />
            <wire x2="4032" y1="2448" y2="2448" x1="3968" />
        </branch>
        <branch name="XLXN_268">
            <wire x2="4032" y1="2656" y2="2656" x1="3904" />
        </branch>
        <branch name="GPO(7:0)">
            <wire x2="1184" y1="1616" y2="1616" x1="1008" />
            <wire x2="1008" y1="1616" y2="1936" x1="1008" />
            <wire x2="1008" y1="1936" y2="1984" x1="1008" />
            <wire x2="1008" y1="1984" y2="2064" x1="1008" />
        </branch>
        <branch name="BTM">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="1256" y="1872" type="branch" />
            <wire x2="1024" y1="1456" y2="1872" x1="1024" />
            <wire x2="1600" y1="1872" y2="1872" x1="1024" />
            <wire x2="1600" y1="1872" y2="2064" x1="1600" />
            <wire x2="1184" y1="1456" y2="1456" x1="1024" />
        </branch>
        <instance x="1184" y="1792" name="XLXI_104" orien="R0">
        </instance>
        <text x="3196" y="604">STD MODE</text>
        <iomarker fontsize="28" x="256" y="800" name="FBIF_TEST_1" orien="R180" />
        <iomarker fontsize="28" x="256" y="880" name="FBIF_TEST_2" orien="R180" />
        <branch name="FBIF_TEST_1">
            <wire x2="448" y1="800" y2="800" x1="256" />
            <wire x2="448" y1="800" y2="848" x1="448" />
            <wire x2="1184" y1="848" y2="848" x1="448" />
        </branch>
        <branch name="FBIF_TEST_2">
            <wire x2="1184" y1="880" y2="880" x1="256" />
        </branch>
        <branch name="SPI_CLK">
            <wire x2="704" y1="1072" y2="1072" x1="208" />
            <wire x2="704" y1="1072" y2="1088" x1="704" />
            <wire x2="1184" y1="1088" y2="1088" x1="704" />
        </branch>
        <instance x="480" y="1264" name="XLXI_134" orien="R180" />
        <instance x="512" y="1456" name="XLXI_135" orien="R180" />
        <branch name="SUM31">
            <wire x2="256" y1="1296" y2="1296" x1="224" />
        </branch>
        <iomarker fontsize="28" x="224" y="1296" name="SUM31" orien="R180" />
        <branch name="DIFF31">
            <wire x2="288" y1="1488" y2="1488" x1="256" />
        </branch>
        <iomarker fontsize="28" x="256" y="1488" name="DIFF31" orien="R180" />
        <branch name="CLK_DDS">
            <wire x2="1920" y1="240" y2="240" x1="272" />
            <wire x2="1920" y1="240" y2="1328" x1="1920" />
            <wire x2="2320" y1="1328" y2="1328" x1="1920" />
        </branch>
        <branch name="CLK_8">
            <wire x2="640" y1="368" y2="368" x1="240" />
            <wire x2="640" y1="368" y2="416" x1="640" />
            <wire x2="1424" y1="416" y2="416" x1="640" />
            <wire x2="1424" y1="416" y2="624" x1="1424" />
            <wire x2="1872" y1="416" y2="416" x1="1424" />
            <wire x2="1872" y1="416" y2="1360" x1="1872" />
            <wire x2="2320" y1="1360" y2="1360" x1="1872" />
            <wire x2="1248" y1="368" y2="368" x1="640" />
            <wire x2="1248" y1="224" y2="368" x1="1248" />
        </branch>
        <instance x="560" y="1648" name="XLXI_136" orien="R0" />
        <branch name="XLXN_275">
            <wire x2="624" y1="1296" y2="1296" x1="480" />
            <wire x2="624" y1="1296" y2="1488" x1="624" />
            <wire x2="624" y1="1488" y2="1520" x1="624" />
            <wire x2="624" y1="1488" y2="1488" x1="512" />
        </branch>
        <branch name="XLXN_277(23:0)">
            <wire x2="2096" y1="1440" y2="1440" x1="1856" />
            <wire x2="2096" y1="1440" y2="1520" x1="2096" />
            <wire x2="2320" y1="1520" y2="1520" x1="2096" />
        </branch>
        <branch name="XLXN_281">
            <wire x2="2976" y1="1424" y2="1424" x1="2832" />
            <wire x2="2992" y1="1376" y2="1376" x1="2976" />
            <wire x2="2976" y1="1376" y2="1424" x1="2976" />
        </branch>
        <branch name="XLXN_283">
            <wire x2="3008" y1="1488" y2="1488" x1="2832" />
            <wire x2="3024" y1="1472" y2="1472" x1="3008" />
            <wire x2="3008" y1="1472" y2="1488" x1="3008" />
        </branch>
        <branch name="XLXN_284">
            <wire x2="3024" y1="1552" y2="1552" x1="2832" />
            <wire x2="3024" y1="1552" y2="1584" x1="3024" />
            <wire x2="3040" y1="1584" y2="1584" x1="3024" />
        </branch>
        <branch name="s_DSPI_ADC(4)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="960" y="2976" type="branch" />
            <wire x2="368" y1="1888" y2="2064" x1="368" />
            <wire x2="544" y1="2064" y2="2064" x1="368" />
            <wire x2="976" y1="1888" y2="1888" x1="368" />
            <wire x2="976" y1="1888" y2="2528" x1="976" />
            <wire x2="1472" y1="2528" y2="2528" x1="976" />
            <wire x2="1472" y1="2528" y2="2976" x1="1472" />
            <wire x2="816" y1="2928" y2="2928" x1="784" />
            <wire x2="816" y1="2928" y2="2976" x1="816" />
            <wire x2="960" y1="2976" y2="2976" x1="816" />
            <wire x2="1472" y1="2976" y2="2976" x1="960" />
        </branch>
        <branch name="s_DSPI_ADC(3)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="960" y="2912" type="branch" />
            <wire x2="816" y1="2880" y2="2880" x1="784" />
            <wire x2="816" y1="2880" y2="2912" x1="816" />
            <wire x2="960" y1="2912" y2="2912" x1="816" />
            <wire x2="1168" y1="2912" y2="2912" x1="960" />
        </branch>
        <branch name="s_DSPI_ADC(2)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="960" y="2848" type="branch" />
            <wire x2="816" y1="2832" y2="2832" x1="784" />
            <wire x2="816" y1="2832" y2="2848" x1="816" />
            <wire x2="960" y1="2848" y2="2848" x1="816" />
            <wire x2="1152" y1="2848" y2="2848" x1="960" />
        </branch>
        <branch name="s_DSPI_ADC(1)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="960" y="2784" type="branch" />
            <wire x2="960" y1="2784" y2="2784" x1="784" />
            <wire x2="1152" y1="2784" y2="2784" x1="960" />
        </branch>
        <branch name="s_DSPI_ADC(0)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="960" y="2704" type="branch" />
            <wire x2="816" y1="2736" y2="2736" x1="784" />
            <wire x2="816" y1="2704" y2="2736" x1="816" />
            <wire x2="960" y1="2704" y2="2704" x1="816" />
            <wire x2="1152" y1="2704" y2="2704" x1="960" />
        </branch>
        <branch name="XLXN_226">
            <wire x2="1168" y1="2576" y2="2576" x1="768" />
            <wire x2="1168" y1="2160" y2="2576" x1="1168" />
            <wire x2="1280" y1="2160" y2="2160" x1="1168" />
        </branch>
        <instance x="1600" y="2032" name="XLXI_65" orien="R180" />
        <branch name="FPGA_RUN">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="1461" y="128" type="branch" />
            <wire x2="1461" y1="128" y2="128" x1="1392" />
            <wire x2="2208" y1="128" y2="128" x1="1461" />
            <wire x2="3424" y1="128" y2="128" x1="2208" />
            <wire x2="3424" y1="128" y2="464" x1="3424" />
            <wire x2="2208" y1="128" y2="1392" x1="2208" />
            <wire x2="2320" y1="1392" y2="1392" x1="2208" />
        </branch>
        <branch name="F">
            <wire x2="2848" y1="1360" y2="1360" x1="2832" />
            <wire x2="2848" y1="1296" y2="1360" x1="2848" />
            <wire x2="3728" y1="1296" y2="1296" x1="2848" />
        </branch>
        <branch name="XLXN_276(15:0)">
            <wire x2="1872" y1="1504" y2="1504" x1="1856" />
            <wire x2="1872" y1="1504" y2="1552" x1="1872" />
            <wire x2="2320" y1="1552" y2="1552" x1="1872" />
        </branch>
        <branch name="GPO(1)">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2240" y="1456" type="branch" />
            <wire x2="2320" y1="1456" y2="1456" x1="2240" />
        </branch>
        <branch name="GPO(0)">
            <wire x2="1120" y1="2480" y2="2480" x1="768" />
            <wire x2="1120" y1="1936" y2="1936" x1="1104" />
            <wire x2="1120" y1="1936" y2="2480" x1="1120" />
            <wire x2="1552" y1="1936" y2="1936" x1="1120" />
            <wire x2="1552" y1="1840" y2="1936" x1="1552" />
            <wire x2="3312" y1="1840" y2="1840" x1="1552" />
            <wire x2="3312" y1="832" y2="832" x1="3264" />
            <wire x2="3264" y1="832" y2="928" x1="3264" />
            <wire x2="3312" y1="928" y2="928" x1="3264" />
            <wire x2="3312" y1="928" y2="1840" x1="3312" />
        </branch>
        <branch name="XLXN_271">
            <wire x2="2672" y1="2192" y2="2192" x1="1600" />
            <wire x2="2864" y1="2016" y2="2016" x1="2672" />
            <wire x2="2864" y1="2016" y2="2032" x1="2864" />
            <wire x2="2912" y1="2032" y2="2032" x1="2864" />
            <wire x2="2912" y1="2032" y2="2048" x1="2912" />
            <wire x2="2928" y1="2048" y2="2048" x1="2912" />
            <wire x2="2672" y1="2016" y2="2192" x1="2672" />
        </branch>
        <instance x="2320" y="1616" name="XLXI_137" orien="R0">
        </instance>
    </sheet>
</drawing>