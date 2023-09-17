<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3a" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_1" />
        <signal name="S_StatHdx(8)" />
        <signal name="XLXN_3" />
        <signal name="XLXN_4" />
        <signal name="MD" />
        <signal name="PWM_STOP_CF" />
        <signal name="FSYNC_VAL_VALID" />
        <signal name="XLXN_14" />
        <signal name="XLXN_15" />
        <signal name="XLXN_16" />
        <signal name="PHA_VAL_VALID" />
        <signal name="GPO_OL_RESET" />
        <signal name="RST" />
        <signal name="XLXN_23" />
        <signal name="XLXN_28" />
        <signal name="XLXN_29" />
        <signal name="SYNC_DDS_ERR" />
        <signal name="SYNC_DDS_RUN" />
        <signal name="PWM_STOP" />
        <signal name="XLXN_36" />
        <signal name="S_StatHdx(15:0)" />
        <signal name="S_StatHdx(0)" />
        <signal name="S_StatHdx(4)" />
        <signal name="S_StatHdx(5)" />
        <signal name="S_StatHdx(9)" />
        <signal name="S_StatHdx(1)" />
        <signal name="S_StatHdx(2)" />
        <signal name="S_StatHdx(3)" />
        <signal name="S_StatHdx(6)" />
        <signal name="S_StatHdx(10)" />
        <signal name="S_StatHdx(11)" />
        <signal name="S_StatHdx(12)" />
        <signal name="S_StatHdx(13)" />
        <signal name="S_StatHdx(14)" />
        <signal name="S_StatHdx(15)" />
        <signal name="S_StatHdx(7)" />
        <port polarity="Input" name="MD" />
        <port polarity="Input" name="PWM_STOP_CF" />
        <port polarity="Input" name="FSYNC_VAL_VALID" />
        <port polarity="Input" name="PHA_VAL_VALID" />
        <port polarity="Input" name="GPO_OL_RESET" />
        <port polarity="Input" name="RST" />
        <port polarity="Output" name="SYNC_DDS_ERR" />
        <port polarity="Output" name="SYNC_DDS_RUN" />
        <port polarity="Output" name="PWM_STOP" />
        <port polarity="Output" name="S_StatHdx(15:0)" />
        <blockdef name="and3b3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="40" y1="-128" y2="-128" x1="0" />
            <circle r="12" cx="52" cy="-128" />
            <line x2="40" y1="-192" y2="-192" x1="0" />
            <circle r="12" cx="52" cy="-192" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
        </blockdef>
        <blockdef name="and3b2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="40" y1="-128" y2="-128" x1="0" />
            <circle r="12" cx="52" cy="-128" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
        </blockdef>
        <blockdef name="or2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="192" ey="-96" sx="112" sy="-48" r="88" cx="116" cy="-136" />
            <arc ex="48" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <line x2="48" y1="-144" y2="-144" x1="112" />
            <arc ex="112" ey="-144" sx="192" sy="-96" r="88" cx="116" cy="-56" />
            <line x2="48" y1="-48" y2="-48" x1="112" />
        </blockdef>
        <blockdef name="fdc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <rect width="256" x="64" y="-320" height="256" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
        </blockdef>
        <blockdef name="vcc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-64" x1="64" />
            <line x2="64" y1="0" y2="-32" x1="64" />
            <line x2="32" y1="-64" y2="-64" x1="96" />
        </blockdef>
        <blockdef name="or2b1">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="32" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="44" cy="-64" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <line x2="48" y1="-48" y2="-48" x1="112" />
            <arc ex="112" ey="-144" sx="192" sy="-96" r="88" cx="116" cy="-56" />
            <line x2="48" y1="-144" y2="-144" x1="112" />
            <arc ex="48" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <arc ex="192" ey="-96" sx="112" sy="-48" r="88" cx="116" cy="-136" />
        </blockdef>
        <blockdef name="buf">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="128" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="0" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="-64" x1="128" />
            <line x2="64" y1="-64" y2="0" x1="64" />
        </blockdef>
        <blockdef name="fd2c">
            <timestamp>2021-3-17T15:20:43</timestamp>
            <rect width="256" x="64" y="-256" height="220" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="192" y1="-36" y2="0" x1="192" />
        </blockdef>
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
        <block symbolname="and3b3" name="XLXI_1">
            <blockpin signalname="SYNC_DDS_ERR" name="I0" />
            <blockpin signalname="PWM_STOP_CF" name="I1" />
            <blockpin signalname="MD" name="I2" />
            <blockpin signalname="S_StatHdx(0)" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_2">
            <blockpin signalname="SYNC_DDS_ERR" name="I0" />
            <blockpin signalname="PWM_STOP_CF" name="I1" />
            <blockpin signalname="MD" name="I2" />
            <blockpin signalname="SYNC_DDS_RUN" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_3">
            <blockpin signalname="FSYNC_VAL_VALID" name="I0" />
            <blockpin signalname="SYNC_DDS_ERR" name="I1" />
            <blockpin signalname="MD" name="I2" />
            <blockpin signalname="XLXN_4" name="O" />
        </block>
        <block symbolname="or2" name="XLXI_4">
            <blockpin signalname="XLXN_3" name="I0" />
            <blockpin signalname="XLXN_4" name="I1" />
            <blockpin signalname="S_StatHdx(8)" name="O" />
        </block>
        <block symbolname="fdc" name="XLXI_11">
            <blockpin signalname="S_StatHdx(8)" name="C" />
            <blockpin signalname="XLXN_23" name="CLR" />
            <blockpin signalname="XLXN_1" name="D" />
            <blockpin signalname="XLXN_3" name="Q" />
        </block>
        <block symbolname="vcc" name="XLXI_12">
            <blockpin signalname="XLXN_1" name="P" />
        </block>
        <block symbolname="and3b2" name="XLXI_13">
            <blockpin signalname="PHA_VAL_VALID" name="I0" />
            <blockpin signalname="SYNC_DDS_ERR" name="I1" />
            <blockpin signalname="MD" name="I2" />
            <blockpin signalname="XLXN_16" name="O" />
        </block>
        <block symbolname="or2" name="XLXI_14">
            <blockpin signalname="XLXN_15" name="I0" />
            <blockpin signalname="XLXN_16" name="I1" />
            <blockpin signalname="S_StatHdx(9)" name="O" />
        </block>
        <block symbolname="fdc" name="XLXI_15">
            <blockpin signalname="S_StatHdx(9)" name="C" />
            <blockpin signalname="XLXN_23" name="CLR" />
            <blockpin signalname="XLXN_14" name="D" />
            <blockpin signalname="XLXN_15" name="Q" />
        </block>
        <block symbolname="vcc" name="XLXI_16">
            <blockpin signalname="XLXN_14" name="P" />
        </block>
        <block symbolname="or2b1" name="XLXI_17">
            <blockpin signalname="RST" name="I0" />
            <blockpin signalname="GPO_OL_RESET" name="I1" />
            <blockpin signalname="XLXN_23" name="O" />
        </block>
        <block symbolname="or2" name="XLXI_18">
            <blockpin signalname="S_StatHdx(9)" name="I0" />
            <blockpin signalname="S_StatHdx(8)" name="I1" />
            <blockpin signalname="XLXN_28" name="O" />
        </block>
        <block symbolname="fdc" name="XLXI_19">
            <blockpin signalname="XLXN_28" name="C" />
            <blockpin signalname="XLXN_23" name="CLR" />
            <blockpin signalname="XLXN_29" name="D" />
            <blockpin signalname="SYNC_DDS_ERR" name="Q" />
        </block>
        <block symbolname="vcc" name="XLXI_20">
            <blockpin signalname="XLXN_29" name="P" />
        </block>
        <block symbolname="buf" name="XLXI_21">
            <blockpin signalname="SYNC_DDS_ERR" name="I" />
            <blockpin signalname="PWM_STOP" name="O" />
        </block>
        <block symbolname="fd2c" name="XLXI_25">
            <blockpin signalname="XLXN_36" name="D0" />
            <blockpin signalname="MD" name="D1" />
            <blockpin signalname="SYNC_DDS_ERR" name="C" />
            <blockpin signalname="S_StatHdx(4)" name="Q0" />
            <blockpin signalname="S_StatHdx(5)" name="Q1" />
            <blockpin signalname="XLXN_23" name="CLR" />
        </block>
        <block symbolname="inv" name="XLXI_27">
            <blockpin signalname="MD" name="I" />
            <blockpin signalname="XLXN_36" name="O" />
        </block>
        <block symbolname="gnd" name="XLXI_28">
            <blockpin signalname="S_StatHdx(2)" name="G" />
        </block>
        <block symbolname="buf" name="XLXI_29">
            <blockpin signalname="SYNC_DDS_RUN" name="I" />
            <blockpin signalname="S_StatHdx(1)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_31">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(3)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_32">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(6)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_33">
            <blockpin signalname="SYNC_DDS_ERR" name="I" />
            <blockpin signalname="S_StatHdx(7)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_34">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(10)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_35">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(11)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_36">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(12)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_37">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(13)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_38">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(14)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_39">
            <blockpin signalname="S_StatHdx(2)" name="I" />
            <blockpin signalname="S_StatHdx(15)" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="5440" height="3520">
        <instance x="848" y="1200" name="XLXI_3" orien="R0" />
        <instance x="1248" y="1200" name="XLXI_4" orien="R0" />
        <instance x="1632" y="1568" name="XLXI_11" orien="M0" />
        <instance x="1776" y="1296" name="XLXI_12" orien="R0" />
        <branch name="XLXN_1">
            <wire x2="1840" y1="1312" y2="1312" x1="1632" />
            <wire x2="1840" y1="1296" y2="1312" x1="1840" />
        </branch>
        <branch name="XLXN_3">
            <wire x2="1248" y1="1136" y2="1136" x1="1232" />
            <wire x2="1232" y1="1136" y2="1312" x1="1232" />
            <wire x2="1248" y1="1312" y2="1312" x1="1232" />
        </branch>
        <branch name="XLXN_4">
            <wire x2="1248" y1="1072" y2="1072" x1="1104" />
        </branch>
        <instance x="848" y="720" name="XLXI_2" orien="R0" />
        <instance x="832" y="464" name="XLXI_1" orien="R0" />
        <branch name="MD">
            <wire x2="736" y1="528" y2="528" x1="240" />
            <wire x2="736" y1="528" y2="1008" x1="736" />
            <wire x2="736" y1="1008" y2="1856" x1="736" />
            <wire x2="864" y1="1856" y2="1856" x1="736" />
            <wire x2="736" y1="1856" y2="2816" x1="736" />
            <wire x2="1280" y1="2816" y2="2816" x1="736" />
            <wire x2="1360" y1="2816" y2="2816" x1="1280" />
            <wire x2="1280" y1="2816" y2="2880" x1="1280" />
            <wire x2="1824" y1="2880" y2="2880" x1="1280" />
            <wire x2="848" y1="1008" y2="1008" x1="736" />
            <wire x2="848" y1="528" y2="528" x1="736" />
            <wire x2="832" y1="272" y2="272" x1="736" />
            <wire x2="736" y1="272" y2="528" x1="736" />
        </branch>
        <iomarker fontsize="28" x="240" y="528" name="MD" orien="R180" />
        <branch name="PWM_STOP_CF">
            <wire x2="688" y1="592" y2="592" x1="304" />
            <wire x2="848" y1="592" y2="592" x1="688" />
            <wire x2="832" y1="336" y2="336" x1="688" />
            <wire x2="688" y1="336" y2="592" x1="688" />
        </branch>
        <iomarker fontsize="28" x="304" y="592" name="PWM_STOP_CF" orien="R180" />
        <branch name="FSYNC_VAL_VALID">
            <wire x2="848" y1="1136" y2="1136" x1="368" />
        </branch>
        <iomarker fontsize="28" x="368" y="1136" name="FSYNC_VAL_VALID" orien="R180" />
        <instance x="864" y="2048" name="XLXI_13" orien="R0" />
        <instance x="1312" y="2048" name="XLXI_14" orien="R0" />
        <instance x="1616" y="2496" name="XLXI_15" orien="M0" />
        <instance x="1632" y="2176" name="XLXI_16" orien="R0" />
        <branch name="XLXN_14">
            <wire x2="1696" y1="2240" y2="2240" x1="1616" />
            <wire x2="1696" y1="2176" y2="2240" x1="1696" />
        </branch>
        <branch name="XLXN_15">
            <wire x2="1216" y1="1984" y2="2240" x1="1216" />
            <wire x2="1232" y1="2240" y2="2240" x1="1216" />
            <wire x2="1312" y1="1984" y2="1984" x1="1216" />
        </branch>
        <branch name="XLXN_16">
            <wire x2="1312" y1="1920" y2="1920" x1="1120" />
        </branch>
        <branch name="PHA_VAL_VALID">
            <wire x2="864" y1="1984" y2="1984" x1="336" />
        </branch>
        <iomarker fontsize="28" x="336" y="1984" name="PHA_VAL_VALID" orien="R180" />
        <instance x="592" y="2992" name="XLXI_17" orien="M180" />
        <branch name="GPO_OL_RESET">
            <wire x2="592" y1="3120" y2="3120" x1="368" />
        </branch>
        <iomarker fontsize="28" x="368" y="3120" name="GPO_OL_RESET" orien="R180" />
        <branch name="RST">
            <wire x2="464" y1="2848" y2="2848" x1="192" />
            <wire x2="464" y1="2848" y2="3056" x1="464" />
            <wire x2="592" y1="3056" y2="3056" x1="464" />
        </branch>
        <iomarker fontsize="28" x="192" y="2848" name="RST" orien="R180" />
        <branch name="XLXN_23">
            <wire x2="1616" y1="3088" y2="3088" x1="848" />
            <wire x2="2016" y1="3088" y2="3088" x1="1616" />
            <wire x2="1616" y1="2464" y2="2528" x1="1616" />
            <wire x2="1616" y1="2528" y2="3088" x1="1616" />
            <wire x2="1856" y1="2528" y2="2528" x1="1616" />
            <wire x2="1856" y1="1536" y2="1536" x1="1632" />
            <wire x2="1856" y1="1536" y2="1648" x1="1856" />
            <wire x2="3024" y1="1648" y2="1648" x1="1856" />
            <wire x2="1856" y1="1648" y2="2528" x1="1856" />
            <wire x2="2016" y1="3040" y2="3088" x1="2016" />
        </branch>
        <instance x="2448" y="1648" name="XLXI_18" orien="R0" />
        <branch name="S_StatHdx(8)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2496" type="branch" />
            <wire x2="1696" y1="1104" y2="1104" x1="1504" />
            <wire x2="1696" y1="1104" y2="1440" x1="1696" />
            <wire x2="2400" y1="1104" y2="1104" x1="1696" />
            <wire x2="2400" y1="1104" y2="1520" x1="2400" />
            <wire x2="2448" y1="1520" y2="1520" x1="2400" />
            <wire x2="2400" y1="1520" y2="1728" x1="2400" />
            <wire x2="2560" y1="1728" y2="1728" x1="2400" />
            <wire x2="2560" y1="1728" y2="2496" x1="2560" />
            <wire x2="4576" y1="2496" y2="2496" x1="2560" />
            <wire x2="4608" y1="2496" y2="2496" x1="4576" />
            <wire x2="1696" y1="1440" y2="1440" x1="1632" />
        </branch>
        <text style="fontsize:24;fontname:Arial" x="1956" y="1080">1=ERR,FSYNC INVALID</text>
        <text style="fontsize:24;fontname:Arial" x="2044" y="1560">1=ERR,PHASE INVALID</text>
        <instance x="3024" y="1680" name="XLXI_19" orien="R0" />
        <branch name="XLXN_28">
            <wire x2="3024" y1="1552" y2="1552" x1="2704" />
        </branch>
        <instance x="2832" y="1376" name="XLXI_20" orien="R0" />
        <branch name="XLXN_29">
            <wire x2="2896" y1="1376" y2="1424" x1="2896" />
            <wire x2="3024" y1="1424" y2="1424" x1="2896" />
        </branch>
        <iomarker fontsize="28" x="4592" y="592" name="SYNC_DDS_RUN" orien="R0" />
        <iomarker fontsize="28" x="4592" y="768" name="SYNC_DDS_ERR" orien="R0" />
        <iomarker fontsize="28" x="4608" y="848" name="PWM_STOP" orien="R0" />
        <instance x="4288" y="880" name="XLXI_21" orien="R0" />
        <branch name="PWM_STOP">
            <wire x2="4608" y1="848" y2="848" x1="4512" />
        </branch>
        <instance x="1824" y="3040" name="XLXI_25" orien="R0">
        </instance>
        <instance x="1360" y="2848" name="XLXI_27" orien="R0" />
        <branch name="XLXN_36">
            <wire x2="1824" y1="2816" y2="2816" x1="1584" />
        </branch>
        <branch name="S_StatHdx(15:0)">
            <wire x2="4704" y1="2032" y2="2048" x1="4704" />
            <wire x2="4704" y1="2048" y2="2096" x1="4704" />
            <wire x2="4704" y1="2096" y2="2144" x1="4704" />
            <wire x2="4704" y1="2144" y2="2192" x1="4704" />
            <wire x2="4704" y1="2192" y2="2240" x1="4704" />
            <wire x2="4704" y1="2240" y2="2288" x1="4704" />
            <wire x2="4704" y1="2288" y2="2336" x1="4704" />
            <wire x2="4704" y1="2336" y2="2384" x1="4704" />
            <wire x2="4704" y1="2384" y2="2496" x1="4704" />
            <wire x2="4704" y1="2496" y2="2544" x1="4704" />
            <wire x2="4704" y1="2544" y2="2592" x1="4704" />
            <wire x2="4704" y1="2592" y2="2640" x1="4704" />
            <wire x2="4704" y1="2640" y2="2688" x1="4704" />
            <wire x2="4704" y1="2688" y2="2736" x1="4704" />
            <wire x2="4704" y1="2736" y2="2784" x1="4704" />
            <wire x2="4704" y1="2784" y2="2832" x1="4704" />
            <wire x2="4704" y1="2832" y2="3024" x1="4704" />
            <wire x2="5120" y1="3024" y2="3024" x1="4704" />
        </branch>
        <iomarker fontsize="28" x="5120" y="3024" name="S_StatHdx(15:0)" orien="R0" />
        <bustap x2="4608" y1="2048" y2="2048" x1="4704" />
        <bustap x2="4608" y1="2096" y2="2096" x1="4704" />
        <bustap x2="4608" y1="2144" y2="2144" x1="4704" />
        <bustap x2="4608" y1="2192" y2="2192" x1="4704" />
        <bustap x2="4608" y1="2240" y2="2240" x1="4704" />
        <bustap x2="4608" y1="2288" y2="2288" x1="4704" />
        <bustap x2="4608" y1="2336" y2="2336" x1="4704" />
        <bustap x2="4608" y1="2384" y2="2384" x1="4704" />
        <branch name="S_StatHdx(0)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2048" type="branch" />
            <wire x2="3888" y1="336" y2="336" x1="1088" />
            <wire x2="3888" y1="336" y2="2048" x1="3888" />
            <wire x2="4576" y1="2048" y2="2048" x1="3888" />
            <wire x2="4608" y1="2048" y2="2048" x1="4576" />
        </branch>
        <branch name="S_StatHdx(4)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2240" type="branch" />
            <wire x2="2400" y1="2816" y2="2816" x1="2208" />
            <wire x2="2400" y1="2240" y2="2816" x1="2400" />
            <wire x2="4576" y1="2240" y2="2240" x1="2400" />
            <wire x2="4608" y1="2240" y2="2240" x1="4576" />
        </branch>
        <branch name="S_StatHdx(5)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2288" type="branch" />
            <wire x2="2448" y1="2880" y2="2880" x1="2208" />
            <wire x2="2448" y1="2288" y2="2880" x1="2448" />
            <wire x2="4576" y1="2288" y2="2288" x1="2448" />
            <wire x2="4608" y1="2288" y2="2288" x1="4576" />
        </branch>
        <bustap x2="4608" y1="2496" y2="2496" x1="4704" />
        <bustap x2="4608" y1="2544" y2="2544" x1="4704" />
        <bustap x2="4608" y1="2592" y2="2592" x1="4704" />
        <bustap x2="4608" y1="2640" y2="2640" x1="4704" />
        <bustap x2="4608" y1="2688" y2="2688" x1="4704" />
        <bustap x2="4608" y1="2736" y2="2736" x1="4704" />
        <bustap x2="4608" y1="2784" y2="2784" x1="4704" />
        <bustap x2="4608" y1="2832" y2="2832" x1="4704" />
        <branch name="S_StatHdx(9)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2544" type="branch" />
            <wire x2="1744" y1="1952" y2="1952" x1="1568" />
            <wire x2="1744" y1="1952" y2="2368" x1="1744" />
            <wire x2="2016" y1="1952" y2="1952" x1="1744" />
            <wire x2="1744" y1="2368" y2="2368" x1="1616" />
            <wire x2="2016" y1="1584" y2="1952" x1="2016" />
            <wire x2="2320" y1="1584" y2="1584" x1="2016" />
            <wire x2="2448" y1="1584" y2="1584" x1="2320" />
            <wire x2="2320" y1="1584" y2="1792" x1="2320" />
            <wire x2="2528" y1="1792" y2="1792" x1="2320" />
            <wire x2="2528" y1="1792" y2="2544" x1="2528" />
            <wire x2="4576" y1="2544" y2="2544" x1="2528" />
            <wire x2="4608" y1="2544" y2="2544" x1="4576" />
        </branch>
        <branch name="SYNC_DDS_RUN">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="3984" y="2096" type="branch" />
            <wire x2="3936" y1="592" y2="592" x1="1104" />
            <wire x2="4592" y1="592" y2="592" x1="3936" />
            <wire x2="3936" y1="592" y2="2096" x1="3936" />
            <wire x2="3984" y1="2096" y2="2096" x1="3936" />
            <wire x2="4080" y1="2096" y2="2096" x1="3984" />
            <wire x2="4160" y1="2096" y2="2096" x1="4080" />
        </branch>
        <instance x="4160" y="2128" name="XLXI_29" orien="R0" />
        <branch name="S_StatHdx(1)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2096" type="branch" />
            <wire x2="4576" y1="2096" y2="2096" x1="4384" />
            <wire x2="4608" y1="2096" y2="2096" x1="4576" />
        </branch>
        <text style="fontsize:28;fontname:Arial" x="3960" y="2028">STD_DDS_RUN</text>
        <branch name="S_StatHdx(3)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2192" type="branch" />
            <wire x2="4576" y1="2192" y2="2192" x1="4400" />
            <wire x2="4608" y1="2192" y2="2192" x1="4576" />
        </branch>
        <branch name="S_StatHdx(6)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2336" type="branch" />
            <wire x2="4576" y1="2336" y2="2336" x1="4384" />
            <wire x2="4608" y1="2336" y2="2336" x1="4576" />
        </branch>
        <branch name="S_StatHdx(10)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2592" type="branch" />
            <wire x2="4576" y1="2592" y2="2592" x1="4384" />
            <wire x2="4608" y1="2592" y2="2592" x1="4576" />
        </branch>
        <branch name="S_StatHdx(11)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2640" type="branch" />
            <wire x2="4464" y1="2656" y2="2656" x1="4384" />
            <wire x2="4576" y1="2640" y2="2640" x1="4464" />
            <wire x2="4608" y1="2640" y2="2640" x1="4576" />
            <wire x2="4464" y1="2640" y2="2656" x1="4464" />
        </branch>
        <branch name="S_StatHdx(12)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2688" type="branch" />
            <wire x2="4464" y1="2720" y2="2720" x1="4384" />
            <wire x2="4576" y1="2688" y2="2688" x1="4464" />
            <wire x2="4608" y1="2688" y2="2688" x1="4576" />
            <wire x2="4464" y1="2688" y2="2720" x1="4464" />
        </branch>
        <branch name="S_StatHdx(13)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2736" type="branch" />
            <wire x2="4416" y1="2784" y2="2784" x1="4384" />
            <wire x2="4416" y1="2736" y2="2784" x1="4416" />
            <wire x2="4576" y1="2736" y2="2736" x1="4416" />
            <wire x2="4608" y1="2736" y2="2736" x1="4576" />
        </branch>
        <branch name="S_StatHdx(14)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2784" type="branch" />
            <wire x2="4448" y1="2848" y2="2848" x1="4384" />
            <wire x2="4448" y1="2768" y2="2848" x1="4448" />
            <wire x2="4464" y1="2768" y2="2768" x1="4448" />
            <wire x2="4464" y1="2768" y2="2784" x1="4464" />
            <wire x2="4576" y1="2784" y2="2784" x1="4464" />
            <wire x2="4608" y1="2784" y2="2784" x1="4576" />
        </branch>
        <branch name="S_StatHdx(15)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2832" type="branch" />
            <wire x2="4464" y1="2912" y2="2912" x1="4384" />
            <wire x2="4576" y1="2832" y2="2832" x1="4464" />
            <wire x2="4608" y1="2832" y2="2832" x1="4576" />
            <wire x2="4464" y1="2832" y2="2912" x1="4464" />
        </branch>
        <branch name="SYNC_DDS_ERR">
            <wire x2="832" y1="400" y2="400" x1="784" />
            <wire x2="784" y1="400" y2="656" x1="784" />
            <wire x2="848" y1="656" y2="656" x1="784" />
            <wire x2="784" y1="656" y2="768" x1="784" />
            <wire x2="4592" y1="768" y2="768" x1="784" />
            <wire x2="784" y1="768" y2="848" x1="784" />
            <wire x2="4288" y1="848" y2="848" x1="784" />
            <wire x2="784" y1="848" y2="912" x1="784" />
            <wire x2="784" y1="912" y2="1072" x1="784" />
            <wire x2="848" y1="1072" y2="1072" x1="784" />
            <wire x2="784" y1="1072" y2="1920" x1="784" />
            <wire x2="864" y1="1920" y2="1920" x1="784" />
            <wire x2="3536" y1="912" y2="912" x1="784" />
            <wire x2="3536" y1="912" y2="1424" x1="3536" />
            <wire x2="3600" y1="1424" y2="1424" x1="3536" />
            <wire x2="3600" y1="1424" y2="2400" x1="3600" />
            <wire x2="3600" y1="2400" y2="3152" x1="3600" />
            <wire x2="4160" y1="2400" y2="2400" x1="3600" />
            <wire x2="1824" y1="2944" y2="2944" x1="1776" />
            <wire x2="1776" y1="2944" y2="3152" x1="1776" />
            <wire x2="3600" y1="3152" y2="3152" x1="1776" />
            <wire x2="3536" y1="1424" y2="1424" x1="3408" />
        </branch>
        <branch name="S_StatHdx(7)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2384" type="branch" />
            <wire x2="4448" y1="2400" y2="2400" x1="4384" />
            <wire x2="4576" y1="2384" y2="2384" x1="4448" />
            <wire x2="4608" y1="2384" y2="2384" x1="4576" />
            <wire x2="4448" y1="2384" y2="2400" x1="4448" />
        </branch>
        <instance x="4048" y="3184" name="XLXI_28" orien="R0" />
        <instance x="4176" y="2224" name="XLXI_31" orien="R0" />
        <instance x="4160" y="2368" name="XLXI_32" orien="R0" />
        <instance x="4160" y="2432" name="XLXI_33" orien="R0" />
        <instance x="4160" y="2624" name="XLXI_34" orien="R0" />
        <instance x="4160" y="2688" name="XLXI_35" orien="R0" />
        <instance x="4160" y="2752" name="XLXI_36" orien="R0" />
        <instance x="4160" y="2816" name="XLXI_37" orien="R0" />
        <instance x="4160" y="2880" name="XLXI_38" orien="R0" />
        <instance x="4160" y="2944" name="XLXI_39" orien="R0" />
        <text style="fontsize:28;fontname:Arial" x="3776" y="2224">(STD_DDS_ERR)</text>
        <text style="fontsize:28;fontname:Arial" x="3780" y="2276">SYNC_DDS_ERR</text>
        <text style="fontsize:28;fontname:Arial" x="3800" y="2484">ERR_1</text>
        <branch name="S_StatHdx(2)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="4576" y="2144" type="branch" />
            <wire x2="4576" y1="2144" y2="2144" x1="4112" />
            <wire x2="4608" y1="2144" y2="2144" x1="4576" />
            <wire x2="4112" y1="2144" y2="2192" x1="4112" />
            <wire x2="4176" y1="2192" y2="2192" x1="4112" />
            <wire x2="4112" y1="2192" y2="2336" x1="4112" />
            <wire x2="4160" y1="2336" y2="2336" x1="4112" />
            <wire x2="4112" y1="2336" y2="2592" x1="4112" />
            <wire x2="4160" y1="2592" y2="2592" x1="4112" />
            <wire x2="4112" y1="2592" y2="2656" x1="4112" />
            <wire x2="4160" y1="2656" y2="2656" x1="4112" />
            <wire x2="4112" y1="2656" y2="2720" x1="4112" />
            <wire x2="4160" y1="2720" y2="2720" x1="4112" />
            <wire x2="4112" y1="2720" y2="2784" x1="4112" />
            <wire x2="4160" y1="2784" y2="2784" x1="4112" />
            <wire x2="4112" y1="2784" y2="2848" x1="4112" />
            <wire x2="4160" y1="2848" y2="2848" x1="4112" />
            <wire x2="4112" y1="2848" y2="2912" x1="4112" />
            <wire x2="4112" y1="2912" y2="3056" x1="4112" />
            <wire x2="4160" y1="2912" y2="2912" x1="4112" />
        </branch>
        <text style="fontsize:28;fontname:Arial" x="3800" y="2528">ERR_2</text>
        <text style="fontsize:28;fontname:Arial" x="3800" y="2384">ERR_X</text>
    </sheet>
</drawing>