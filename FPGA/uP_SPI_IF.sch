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
        <signal name="CLK_8" />
        <signal name="CLK_4" />
        <signal name="CLK_1ms" />
        <signal name="FBIF_TEST_1" />
        <signal name="FBIF_TEST_2" />
        <signal name="HW_VERS_BD(7:0)" />
        <signal name="XLXN_30(7:0)" />
        <signal name="SPI_CLK" />
        <signal name="SPI_N_CS" />
        <signal name="SPI_O_MISO" />
        <signal name="SPI_I_MOSI" />
        <signal name="RCYC_S_StatHdx(15:0)" />
        <signal name="RCYC_STAT_FrequValOut(23:0)" />
        <signal name="RCYC_S_PhaseValOut(15:0)" />
        <signal name="GPI(7:0)" />
        <signal name="GPO(7:0)" />
        <signal name="BTM" />
        <signal name="BTM_PWMA" />
        <signal name="BTM_GPO(7:0)" />
        <signal name="BTM_PWMB" />
        <signal name="ADC_ERROR" />
        <signal name="FrequVal(23:0)" />
        <signal name="Phase_Val_Adc(15:0)" />
        <signal name="Pwm_Val(15:0)" />
        <signal name="FPGA_RUN" />
        <port polarity="Input" name="X_RST" />
        <port polarity="Input" name="CLK_M" />
        <port polarity="Input" name="CLK_8" />
        <port polarity="Input" name="CLK_4" />
        <port polarity="Input" name="CLK_1ms" />
        <port polarity="Output" name="FBIF_TEST_1" />
        <port polarity="Output" name="FBIF_TEST_2" />
        <port polarity="Input" name="HW_VERS_BD(7:0)" />
        <port polarity="Input" name="SPI_CLK" />
        <port polarity="Input" name="SPI_N_CS" />
        <port polarity="Output" name="SPI_O_MISO" />
        <port polarity="Input" name="SPI_I_MOSI" />
        <port polarity="Input" name="RCYC_S_StatHdx(15:0)" />
        <port polarity="Input" name="RCYC_STAT_FrequValOut(23:0)" />
        <port polarity="Input" name="RCYC_S_PhaseValOut(15:0)" />
        <port polarity="Input" name="GPI(7:0)" />
        <port polarity="Output" name="GPO(7:0)" />
        <port polarity="Output" name="BTM" />
        <port polarity="Output" name="BTM_PWMA" />
        <port polarity="Output" name="BTM_GPO(7:0)" />
        <port polarity="Output" name="BTM_PWMB" />
        <port polarity="Input" name="ADC_ERROR" />
        <port polarity="Output" name="FrequVal(23:0)" />
        <port polarity="Output" name="Phase_Val_Adc(15:0)" />
        <port polarity="Output" name="Pwm_Val(15:0)" />
        <port polarity="Output" name="FPGA_RUN" />
        <blockdef name="FBIF_SPIIF">
            <timestamp>2021-7-14T14:39:5</timestamp>
            <line x2="144" y1="-3264" y2="-3312" x1="144" />
            <line x2="272" y1="-3264" y2="-3264" x1="208" />
            <line x2="192" y1="-3264" y2="-3312" x1="192" />
            <line x2="240" y1="-3312" y2="-3264" x1="240" />
            <line x2="288" y1="-3312" y2="-3264" x1="288" />
            <line x2="336" y1="-3312" y2="-3264" x1="336" />
            <line x2="384" y1="-3264" y2="-3312" x1="384" />
            <line x2="64" y1="-3088" y2="-3088" x1="0" />
            <line x2="64" y1="-3056" y2="-3056" x1="0" />
            <rect width="64" x="0" y="-3036" height="24" />
            <line x2="64" y1="-3024" y2="-3024" x1="0" />
            <rect width="768" x="64" y="-3264" height="3204" />
            <rect width="64" x="0" y="-3004" height="24" />
            <line x2="64" y1="-2992" y2="-2992" x1="0" />
            <rect width="64" x="832" y="-1724" height="24" />
            <line x2="896" y1="-1712" y2="-1712" x1="832" />
            <rect width="64" x="832" y="-1596" height="24" />
            <line x2="896" y1="-1584" y2="-1584" x1="832" />
            <rect width="64" x="832" y="-1564" height="24" />
            <line x2="896" y1="-1552" y2="-1552" x1="832" />
            <rect width="64" x="832" y="-1532" height="24" />
            <line x2="896" y1="-1520" y2="-1520" x1="832" />
            <line x2="0" y1="-2064" y2="-2064" x1="64" />
            <line x2="0" y1="-2032" y2="-2032" x1="64" />
            <rect width="64" x="0" y="-2364" height="24" />
            <line x2="0" y1="-2352" y2="-2352" x1="64" />
            <rect width="64" x="0" y="-2412" height="24" />
            <line x2="0" y1="-2400" y2="-2400" x1="64" />
            <line x2="64" y1="-2000" y2="-2000" x1="0" />
            <line x2="0" y1="-1968" y2="-1968" x1="64" />
            <rect width="64" x="0" y="-1724" height="24" />
            <line x2="0" y1="-1712" y2="-1712" x1="64" />
            <rect width="64" x="0" y="-1596" height="24" />
            <line x2="0" y1="-1584" y2="-1584" x1="64" />
            <rect width="64" x="0" y="-1564" height="24" />
            <line x2="0" y1="-1552" y2="-1552" x1="64" />
            <rect width="64" x="0" y="-684" height="24" />
            <line x2="0" y1="-672" y2="-672" x1="64" />
            <rect width="64" x="0" y="-636" height="24" />
            <line x2="64" y1="-624" y2="-624" x1="0" />
            <line x2="64" y1="-544" y2="-544" x1="0" />
            <line x2="64" y1="-512" y2="-512" x1="0" />
            <line x2="64" y1="-480" y2="-480" x1="0" />
            <rect width="64" x="0" y="-460" height="24" />
            <line x2="64" y1="-448" y2="-448" x1="0" />
            <line x2="320" y1="0" y2="-60" x1="320" />
        </blockdef>
        <blockdef name="constant">
            <timestamp>2006-1-1T10:10:10</timestamp>
            <rect width="112" x="0" y="0" height="64" />
            <line x2="112" y1="32" y2="32" x1="144" />
        </blockdef>
        <block symbolname="constant" name="XLXI_15">
            <attr value="02" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="XLXN_30(7:0)" name="O" />
        </block>
        <block symbolname="FBIF_SPIIF" name="XLXI_22">
            <blockpin signalname="ADC_ERROR" name="ADC_ERROR" />
            <blockpin signalname="CLK_1ms" name="CLK_1ms" />
            <blockpin signalname="CLK_4" name="CLK_4" />
            <blockpin signalname="CLK_8" name="CLK_8" />
            <blockpin signalname="CLK_M" name="CLK_M" />
            <blockpin signalname="GPI(7:0)" name="GPI(7:0)" />
            <blockpin signalname="HW_VERS_BD(7:0)" name="HW_VERS_BD(7:0)" />
            <blockpin signalname="RCYC_STAT_FrequValOut(23:0)" name="RCYC_S_FrequValOut(23:0)" />
            <blockpin signalname="RCYC_S_PhaseValOut(15:0)" name="RCYC_S_PhaseValOut(15:0)" />
            <blockpin signalname="RCYC_S_StatHdx(15:0)" name="RCYC_S_StatHdx(15:0)" />
            <blockpin signalname="SPI_CLK" name="SPI_CLK" />
            <blockpin signalname="SPI_I_MOSI" name="SPI_I_MOSI" />
            <blockpin signalname="SPI_N_CS" name="SPI_N_CS" />
            <blockpin signalname="XLXN_30(7:0)" name="TYPE_BD(7:0)" />
            <blockpin signalname="X_RST" name="X_RST" />
            <blockpin signalname="BTM" name="BTM" />
            <blockpin signalname="BTM_GPO(7:0)" name="BTM_GPO(7:0)" />
            <blockpin signalname="BTM_PWMA" name="BTM_PWMA" />
            <blockpin signalname="BTM_PWMB" name="BTM_PWMB" />
            <blockpin signalname="FBIF_TEST_1" name="FBIF_TEST_1" />
            <blockpin signalname="FBIF_TEST_2" name="FBIF_TEST_2" />
            <blockpin name="FPGA_CTRL_TEST(15:0)" />
            <blockpin signalname="FPGA_RUN" name="FPGA_RUN" />
            <blockpin name="FPGA_STAT_TEST(15:0)" />
            <blockpin signalname="GPO(7:0)" name="GPO(7:0)" />
            <blockpin signalname="SPI_O_MISO" name="SPI_O_MISO" />
            <blockpin name="WCYC_P_CtrlHdx(15:0)" />
            <blockpin signalname="FrequVal(23:0)" name="WCYC_P_FrequValIn(23:0)" />
            <blockpin signalname="Phase_Val_Adc(15:0)" name="WCYC_P_PhaseValIn(15:0)" />
            <blockpin signalname="Pwm_Val(15:0)" name="WCYC_P_PWMVal(15:0)" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="7040" height="5440">
        <branch name="X_RST">
            <wire x2="1232" y1="128" y2="128" x1="640" />
            <wire x2="1232" y1="128" y2="416" x1="1232" />
        </branch>
        <branch name="CLK_M">
            <wire x2="1328" y1="192" y2="192" x1="640" />
            <wire x2="1328" y1="192" y2="416" x1="1328" />
        </branch>
        <branch name="CLK_4">
            <wire x2="1376" y1="256" y2="256" x1="640" />
            <wire x2="1376" y1="256" y2="416" x1="1376" />
        </branch>
        <branch name="CLK_8">
            <wire x2="1424" y1="320" y2="320" x1="640" />
            <wire x2="1424" y1="320" y2="416" x1="1424" />
        </branch>
        <iomarker fontsize="28" x="640" y="128" name="X_RST" orien="R180" />
        <iomarker fontsize="28" x="640" y="192" name="CLK_M" orien="R180" />
        <iomarker fontsize="28" x="640" y="256" name="CLK_4" orien="R180" />
        <iomarker fontsize="28" x="640" y="320" name="CLK_8" orien="R180" />
        <branch name="CLK_1ms">
            <wire x2="1472" y1="384" y2="384" x1="672" />
            <wire x2="1472" y1="384" y2="416" x1="1472" />
        </branch>
        <iomarker fontsize="28" x="672" y="384" name="CLK_1ms" orien="R180" />
        <branch name="FBIF_TEST_2">
            <wire x2="1088" y1="672" y2="672" x1="672" />
        </branch>
        <iomarker fontsize="28" x="672" y="640" name="FBIF_TEST_1" orien="R180" />
        <branch name="FBIF_TEST_1">
            <wire x2="1088" y1="640" y2="640" x1="672" />
        </branch>
        <iomarker fontsize="28" x="672" y="672" name="FBIF_TEST_2" orien="R180" />
        <branch name="HW_VERS_BD(7:0)">
            <wire x2="1088" y1="1376" y2="1376" x1="400" />
        </branch>
        <iomarker fontsize="28" x="400" y="1376" name="HW_VERS_BD(7:0)" orien="R180" />
        <branch name="XLXN_30(7:0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="811" y="1328" type="branch" />
            <wire x2="576" y1="1104" y2="1104" x1="496" />
            <wire x2="576" y1="1104" y2="1328" x1="576" />
            <wire x2="1088" y1="1328" y2="1328" x1="576" />
        </branch>
        <branch name="SPI_O_MISO">
            <wire x2="1072" y1="1744" y2="1744" x1="320" />
            <wire x2="1088" y1="1728" y2="1728" x1="1072" />
            <wire x2="1072" y1="1728" y2="1744" x1="1072" />
        </branch>
        <branch name="SPI_I_MOSI">
            <wire x2="1072" y1="1792" y2="1792" x1="304" />
            <wire x2="1088" y1="1760" y2="1760" x1="1072" />
            <wire x2="1072" y1="1760" y2="1792" x1="1072" />
        </branch>
        <branch name="SPI_CLK">
            <wire x2="1072" y1="1648" y2="1648" x1="288" />
            <wire x2="1072" y1="1648" y2="1664" x1="1072" />
            <wire x2="1088" y1="1664" y2="1664" x1="1072" />
        </branch>
        <branch name="SPI_N_CS">
            <wire x2="1088" y1="1696" y2="1696" x1="288" />
        </branch>
        <iomarker fontsize="28" x="288" y="1696" name="SPI_N_CS" orien="R180" />
        <iomarker fontsize="28" x="288" y="1648" name="SPI_CLK" orien="R180" />
        <iomarker fontsize="28" x="320" y="1744" name="SPI_O_MISO" orien="R180" />
        <iomarker fontsize="28" x="304" y="1792" name="SPI_I_MOSI" orien="R180" />
        <branch name="RCYC_S_StatHdx(15:0)">
            <wire x2="1088" y1="2016" y2="2016" x1="384" />
        </branch>
        <iomarker fontsize="28" x="384" y="2016" name="RCYC_S_StatHdx(15:0)" orien="R180" />
        <branch name="RCYC_STAT_FrequValOut(23:0)">
            <wire x2="1088" y1="2144" y2="2144" x1="464" />
        </branch>
        <iomarker fontsize="28" x="464" y="2144" name="RCYC_STAT_FrequValOut(23:0)" orien="R180" />
        <branch name="RCYC_S_PhaseValOut(15:0)">
            <wire x2="1072" y1="2192" y2="2192" x1="448" />
            <wire x2="1088" y1="2176" y2="2176" x1="1072" />
            <wire x2="1072" y1="2176" y2="2192" x1="1072" />
        </branch>
        <iomarker fontsize="28" x="448" y="2192" name="RCYC_S_PhaseValOut(15:0)" orien="R180" />
        <branch name="GPI(7:0)">
            <wire x2="1088" y1="3056" y2="3056" x1="400" />
        </branch>
        <iomarker fontsize="28" x="400" y="3056" name="GPI(7:0)" orien="R180" />
        <branch name="GPO(7:0)">
            <wire x2="1072" y1="3120" y2="3120" x1="400" />
            <wire x2="1088" y1="3104" y2="3104" x1="1072" />
            <wire x2="1072" y1="3104" y2="3120" x1="1072" />
        </branch>
        <iomarker fontsize="28" x="400" y="3120" name="GPO(7:0)" orien="R180" />
        <branch name="BTM_GPO(7:0)">
            <wire x2="1072" y1="3360" y2="3360" x1="416" />
            <wire x2="1088" y1="3280" y2="3280" x1="1072" />
            <wire x2="1072" y1="3280" y2="3360" x1="1072" />
        </branch>
        <branch name="BTM">
            <wire x2="1088" y1="3184" y2="3184" x1="352" />
        </branch>
        <iomarker fontsize="28" x="352" y="3184" name="BTM" orien="R180" />
        <iomarker fontsize="28" x="416" y="3248" name="BTM_PWMA" orien="R180" />
        <branch name="BTM_PWMA">
            <wire x2="496" y1="3248" y2="3248" x1="416" />
            <wire x2="496" y1="3216" y2="3248" x1="496" />
            <wire x2="1088" y1="3216" y2="3216" x1="496" />
        </branch>
        <iomarker fontsize="28" x="400" y="3296" name="BTM_PWMB" orien="R180" />
        <branch name="BTM_PWMB">
            <wire x2="736" y1="3296" y2="3296" x1="400" />
            <wire x2="736" y1="3248" y2="3296" x1="736" />
            <wire x2="1088" y1="3248" y2="3248" x1="736" />
        </branch>
        <iomarker fontsize="28" x="416" y="3360" name="BTM_GPO(7:0)" orien="R180" />
        <branch name="ADC_ERROR">
            <wire x2="1408" y1="3744" y2="3744" x1="352" />
            <wire x2="1408" y1="3728" y2="3744" x1="1408" />
        </branch>
        <iomarker fontsize="28" x="352" y="3744" name="ADC_ERROR" orien="R180" />
        <branch name="FrequVal(23:0)">
            <wire x2="2624" y1="2144" y2="2144" x1="1984" />
        </branch>
        <iomarker fontsize="28" x="2624" y="2144" name="FrequVal(23:0)" orien="R0" />
        <branch name="Phase_Val_Adc(15:0)">
            <wire x2="2624" y1="2176" y2="2176" x1="1984" />
        </branch>
        <iomarker fontsize="28" x="2624" y="2176" name="Phase_Val_Adc(15:0)" orien="R0" />
        <iomarker fontsize="28" x="2624" y="2320" name="Pwm_Val(15:0)" orien="R0" />
        <branch name="Pwm_Val(15:0)">
            <wire x2="2608" y1="2208" y2="2208" x1="1984" />
            <wire x2="2608" y1="2208" y2="2320" x1="2608" />
            <wire x2="2624" y1="2320" y2="2320" x1="2608" />
        </branch>
        <branch name="FPGA_RUN">
            <wire x2="1280" y1="128" y2="416" x1="1280" />
            <wire x2="2896" y1="128" y2="128" x1="1280" />
        </branch>
        <iomarker fontsize="28" x="2896" y="128" name="FPGA_RUN" orien="R0" />
        <instance x="352" y="1072" name="XLXI_15" orien="R0">
        </instance>
        <instance x="1088" y="3728" name="XLXI_22" orien="R0">
        </instance>
    </sheet>
</drawing>