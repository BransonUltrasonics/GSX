<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3a" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="CLK_25MHZ" />
        <signal name="XLXN_2" />
        <signal name="RST" />
        <signal name="XLXN_6" />
        <signal name="XLXN_7" />
        <signal name="XLXN_9" />
        <signal name="q(2:0)" />
        <signal name="q(0)" />
        <signal name="CLK_DDS" />
        <signal name="q(1)" />
        <signal name="q(2)" />
        <signal name="CLK_8" />
        <signal name="XLXN_39" />
        <signal name="XLXN_44" />
        <signal name="XLXN_53" />
        <signal name="XLXN_57(15:0)" />
        <signal name="XLXN_58(15:0)" />
        <signal name="XLXN_62" />
        <signal name="XLXN_63" />
        <signal name="XLXN_66" />
        <signal name="XLXN_70" />
        <signal name="CLK_1ms" />
        <signal name="XLXN_77" />
        <signal name="XLXN_79(15:0)" />
        <signal name="XLXN_79(8)" />
        <signal name="Reset" />
        <signal name="XLXN_86" />
        <signal name="XLXN_92" />
        <signal name="XLXN_95" />
        <signal name="XLXN_96" />
        <signal name="XLXN_106" />
        <signal name="XLXN_107" />
        <signal name="XLXN_112" />
        <signal name="XLXN_113" />
        <signal name="CLK_FADC" />
        <signal name="CLK_4" />
        <signal name="XLXN_164" />
        <signal name="LCKD1" />
        <signal name="LCKD2" />
        <signal name="LCKD3" />
        <signal name="LCKD4" />
        <signal name="CLK_M" />
        <signal name="Clock_Manager_FXOUT" />
        <signal name="XLXN_172" />
        <signal name="SUD_LOCKED" />
        <signal name="ALIVE" />
        <signal name="S_CLK_M" />
        <signal name="S_CLK_DDS" />
        <signal name="S_CLK_4" />
        <signal name="S_CLK_8" />
        <signal name="XLXN_181" />
        <port polarity="Input" name="CLK_25MHZ" />
        <port polarity="Input" name="RST" />
        <port polarity="Output" name="CLK_DDS" />
        <port polarity="Output" name="CLK_8" />
        <port polarity="Output" name="CLK_1ms" />
        <port polarity="Output" name="CLK_FADC" />
        <port polarity="Output" name="CLK_4" />
        <port polarity="Output" name="CLK_M" />
        <port polarity="Output" name="SUD_LOCKED" />
        <port polarity="Output" name="ALIVE" />
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
        </blockdef>
        <blockdef name="counter3cr">
            <timestamp>2021-3-25T20:36:30</timestamp>
            <rect width="188" x="176" y="152" height="112" />
            <line x2="176" y1="224" y2="224" x1="144" />
            <line x2="176" y1="192" y2="192" x1="144" />
            <line x2="272" y1="288" y2="264" x1="272" />
            <line x2="368" y1="208" y2="208" style="linewidth:W" x1="400" />
        </blockdef>
        <blockdef name="vcc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-64" x1="64" />
            <line x2="64" y1="0" y2="-32" x1="64" />
            <line x2="32" y1="-64" y2="-64" x1="96" />
        </blockdef>
        <blockdef name="ldc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <rect width="256" x="64" y="-320" height="256" />
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
        <blockdef name="and2b1">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-48" y2="-144" x1="64" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
        </blockdef>
        <blockdef name="fdce">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <rect width="256" x="64" y="-320" height="256" />
        </blockdef>
        <blockdef name="cc16ce">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="320" y1="-128" y2="-128" x1="384" />
            <line x2="320" y1="-192" y2="-192" x1="384" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <rect width="64" x="320" y="-268" height="24" />
            <rect width="256" x="64" y="-320" height="256" />
        </blockdef>
        <blockdef name="compm16">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="320" y1="-192" y2="-192" x1="384" />
            <rect width="256" x="64" y="-384" height="320" />
            <rect width="64" x="0" y="-332" height="24" />
            <line x2="64" y1="-320" y2="-320" x1="0" />
            <rect width="64" x="0" y="-140" height="24" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
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
        <blockdef name="constant">
            <timestamp>2006-1-1T10:10:10</timestamp>
            <rect width="112" x="0" y="0" height="64" />
            <line x2="112" y1="32" y2="32" x1="144" />
        </blockdef>
        <blockdef name="ftc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <rect width="256" x="64" y="-320" height="256" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
        </blockdef>
        <blockdef name="and3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
        </blockdef>
        <blockdef name="counter23">
            <timestamp>2021-3-26T21:38:59</timestamp>
            <rect width="256" x="64" y="-192" height="192" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <rect width="64" x="320" y="-172" height="24" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
            <line x2="192" y1="48" y2="0" x1="192" />
            <line x2="0" y1="-48" y2="-48" x1="64" />
            <line x2="0" y1="-80" y2="-80" x1="64" />
            <line x2="384" y1="-80" y2="-80" x1="320" />
        </blockdef>
        <blockdef name="and7">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-448" y2="-64" x1="64" />
            <arc ex="144" ey="-304" sx="144" sy="-208" r="48" cx="144" cy="-256" />
            <line x2="144" y1="-304" y2="-304" x1="64" />
            <line x2="64" y1="-208" y2="-208" x1="144" />
            <line x2="192" y1="-256" y2="-256" x1="256" />
            <line x2="64" y1="-448" y2="-448" x1="0" />
            <line x2="64" y1="-384" y2="-384" x1="0" />
            <line x2="64" y1="-320" y2="-320" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-64" y2="-64" x1="0" />
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
        <blockdef name="CLKMAN_1">
            <timestamp>2021-4-13T14:45:46</timestamp>
            <rect width="336" x="64" y="-256" height="240" />
            <line x2="464" y1="-160" y2="-160" x1="400" />
            <line x2="0" y1="-176" y2="-176" x1="64" />
            <line x2="0" y1="-208" y2="-208" x1="64" />
            <line x2="464" y1="-208" y2="-208" x1="400" />
            <line x2="464" y1="-48" y2="-48" x1="400" />
        </blockdef>
        <blockdef name="CLKMAN_2">
            <timestamp>2021-4-13T14:46:57</timestamp>
            <line x2="496" y1="-160" y2="-160" x1="432" />
            <line x2="0" y1="-112" y2="-112" x1="64" />
            <line x2="0" y1="-144" y2="-144" x1="64" />
            <rect width="368" x="64" y="-192" height="224" />
            <line x2="496" y1="0" y2="0" x1="432" />
            <line x2="496" y1="-96" y2="-96" x1="432" />
        </blockdef>
        <blockdef name="CLKMAN_3">
            <timestamp>2021-4-13T14:48:0</timestamp>
            <rect width="336" x="64" y="-256" height="248" />
            <line x2="464" y1="-160" y2="-160" x1="400" />
            <line x2="0" y1="-176" y2="-176" x1="64" />
            <line x2="0" y1="-208" y2="-208" x1="64" />
            <line x2="464" y1="-208" y2="-208" x1="400" />
            <line x2="464" y1="-48" y2="-48" x1="400" />
        </blockdef>
        <blockdef name="CLKMAN_4">
            <timestamp>2021-4-13T14:48:53</timestamp>
            <rect width="336" x="64" y="-256" height="256" />
            <line x2="464" y1="-160" y2="-160" x1="400" />
            <line x2="0" y1="-208" y2="-208" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="464" y1="-32" y2="-32" x1="400" />
            <line x2="464" y1="-208" y2="-208" x1="400" />
        </blockdef>
        <blockdef name="CLOCK_MANAGER">
            <timestamp>2021-4-13T14:41:59</timestamp>
            <line x2="464" y1="-48" y2="-48" x1="400" />
            <line x2="0" y1="-48" y2="-48" x1="64" />
            <line x2="0" y1="-240" y2="-240" x1="64" />
            <line x2="464" y1="-240" y2="-240" x1="400" />
            <rect width="336" x="64" y="-272" height="280" />
        </blockdef>
        <blockdef name="buf">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="128" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="0" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="-64" x1="128" />
            <line x2="64" y1="-64" y2="0" x1="64" />
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
        <blockdef name="copy_of_and2">
            <timestamp>2021-3-16T19:45:5</timestamp>
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
        <block symbolname="inv" name="XLXI_3">
            <blockpin signalname="RST" name="I" />
            <blockpin signalname="XLXN_2" name="O" />
        </block>
        <block symbolname="counter3cr" name="XLXI_4">
            <blockpin signalname="Clock_Manager_FXOUT" name="clk" />
            <blockpin signalname="XLXN_6" name="ce" />
            <blockpin signalname="Reset" name="sclr" />
            <blockpin signalname="q(2:0)" name="q(2:0)" />
        </block>
        <block symbolname="vcc" name="XLXI_6">
            <blockpin signalname="XLXN_6" name="P" />
        </block>
        <block symbolname="inv" name="XLXI_7">
            <blockpin signalname="XLXN_7" name="I" />
            <blockpin signalname="Reset" name="O" />
        </block>
        <block symbolname="ldc" name="XLXI_9">
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="Clock_Manager_FXOUT" name="D" />
            <blockpin signalname="XLXN_9" name="G" />
            <blockpin signalname="S_CLK_M" name="Q" />
        </block>
        <block symbolname="vcc" name="XLXI_10">
            <blockpin signalname="XLXN_9" name="P" />
        </block>
        <block symbolname="fdc" name="XLXI_16">
            <blockpin signalname="Clock_Manager_FXOUT" name="C" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="q(0)" name="D" />
            <blockpin signalname="S_CLK_DDS" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_20">
            <blockpin signalname="Clock_Manager_FXOUT" name="C" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="q(1)" name="D" />
            <blockpin signalname="S_CLK_4" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_22">
            <blockpin signalname="Clock_Manager_FXOUT" name="C" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="q(2)" name="D" />
            <blockpin signalname="S_CLK_8" name="Q" />
        </block>
        <block symbolname="and2b1" name="XLXI_24">
            <blockpin signalname="XLXN_39" name="I0" />
            <blockpin signalname="q(2)" name="I1" />
            <blockpin signalname="XLXN_44" name="O" />
        </block>
        <block symbolname="fdc" name="XLXI_25">
            <blockpin signalname="Clock_Manager_FXOUT" name="C" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="q(2)" name="D" />
            <blockpin signalname="XLXN_39" name="Q" />
        </block>
        <block symbolname="cc16ce" name="XLXI_28">
            <blockpin signalname="CLK_4" name="C" />
            <blockpin signalname="XLXN_53" name="CE" />
            <blockpin signalname="XLXN_62" name="CLR" />
            <blockpin name="CEO" />
            <blockpin signalname="XLXN_57(15:0)" name="Q(15:0)" />
            <blockpin name="TC" />
        </block>
        <block symbolname="vcc" name="XLXI_30">
            <blockpin signalname="XLXN_53" name="P" />
        </block>
        <block symbolname="compm16" name="XLXI_31">
            <blockpin signalname="XLXN_57(15:0)" name="A(15:0)" />
            <blockpin signalname="XLXN_58(15:0)" name="B(15:0)" />
            <blockpin signalname="XLXN_63" name="GT" />
            <blockpin name="LT" />
        </block>
        <block symbolname="or2" name="XLXI_35">
            <blockpin signalname="XLXN_63" name="I0" />
            <blockpin signalname="Reset" name="I1" />
            <blockpin signalname="XLXN_62" name="O" />
        </block>
        <block symbolname="constant" name="XLXI_36">
            <attr value="61A8" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="XLXN_58(15:0)" name="O" />
        </block>
        <block symbolname="ftc" name="XLXI_37">
            <blockpin signalname="XLXN_63" name="C" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="XLXN_66" name="T" />
            <blockpin signalname="XLXN_70" name="Q" />
        </block>
        <block symbolname="vcc" name="XLXI_38">
            <blockpin signalname="XLXN_66" name="P" />
        </block>
        <block symbolname="cc16ce" name="XLXI_46">
            <blockpin signalname="CLK_1ms" name="C" />
            <blockpin signalname="XLXN_77" name="CE" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin name="CEO" />
            <blockpin signalname="XLXN_79(15:0)" name="Q(15:0)" />
            <blockpin name="TC" />
        </block>
        <block symbolname="vcc" name="XLXI_47">
            <blockpin signalname="XLXN_77" name="P" />
        </block>
        <block symbolname="cc16ce" name="XLXI_50">
            <blockpin signalname="CLK_25MHZ" name="C" />
            <blockpin signalname="XLXN_86" name="CE" />
            <blockpin signalname="XLXN_2" name="CLR" />
            <blockpin name="CEO" />
            <blockpin name="Q(15:0)" />
            <blockpin signalname="XLXN_112" name="TC" />
        </block>
        <block symbolname="inv" name="XLXI_51">
            <blockpin signalname="XLXN_112" name="I" />
            <blockpin signalname="XLXN_86" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_53">
            <blockpin signalname="XLXN_112" name="I0" />
            <blockpin signalname="XLXN_7" name="I1" />
            <blockpin signalname="XLXN_96" name="I2" />
            <blockpin signalname="XLXN_107" name="O" />
        </block>
        <block symbolname="counter23" name="XLXI_54">
            <blockpin name="Q(23:0)" />
            <blockpin signalname="XLXN_95" name="TC" />
            <blockpin signalname="XLXN_92" name="CLR" />
            <blockpin signalname="CLK_25MHZ" name="C" />
            <blockpin signalname="XLXN_107" name="CE" />
            <blockpin name="CEO" />
        </block>
        <block symbolname="or2" name="XLXI_55">
            <blockpin signalname="XLXN_2" name="I0" />
            <blockpin signalname="XLXN_181" name="I1" />
            <blockpin signalname="XLXN_92" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_56">
            <blockpin signalname="XLXN_95" name="I" />
            <blockpin signalname="XLXN_96" name="O" />
        </block>
        <block symbolname="and7" name="XLXI_57">
            <blockpin signalname="XLXN_164" name="I0" />
            <blockpin signalname="LCKD4" name="I1" />
            <blockpin signalname="LCKD3" name="I2" />
            <blockpin signalname="LCKD2" name="I3" />
            <blockpin signalname="LCKD1" name="I4" />
            <blockpin signalname="XLXN_95" name="I5" />
            <blockpin signalname="XLXN_7" name="I6" />
            <blockpin signalname="XLXN_106" name="O" />
        </block>
        <block symbolname="vcc" name="XLXI_58">
            <blockpin signalname="XLXN_164" name="P" />
        </block>
        <block symbolname="vcc" name="XLXI_61">
            <blockpin signalname="XLXN_113" name="P" />
        </block>
        <block symbolname="gnd" name="XLXI_62">
            <blockpin signalname="CLK_FADC" name="G" />
        </block>
        <block symbolname="CLKMAN_1" name="XLXI_81">
            <blockpin signalname="Reset" name="RST_IN" />
            <blockpin signalname="S_CLK_M" name="CLKIN_IN" />
            <blockpin signalname="LCKD1" name="LOCKED_OUT" />
            <blockpin name="CLK90_OUT" />
            <blockpin signalname="CLK_M" name="CLK0_OUT" />
        </block>
        <block symbolname="CLKMAN_2" name="XLXI_87">
            <blockpin signalname="Reset" name="RST_IN" />
            <blockpin signalname="S_CLK_DDS" name="CLKIN_IN" />
            <blockpin signalname="CLK_DDS" name="CLK0_OUT" />
            <blockpin signalname="LCKD2" name="LOCKED_OUT" />
            <blockpin name="CLK90_OUT" />
        </block>
        <block symbolname="CLKMAN_3" name="XLXI_95">
            <blockpin signalname="Reset" name="RST_IN" />
            <blockpin signalname="S_CLK_4" name="CLKIN_IN" />
            <blockpin signalname="LCKD3" name="LOCKED_OUT" />
            <blockpin name="CLK90_OUT" />
            <blockpin signalname="CLK_4" name="CLK0_OUT" />
        </block>
        <block symbolname="CLKMAN_4" name="XLXI_98">
            <blockpin signalname="Reset" name="RST_IN" />
            <blockpin signalname="S_CLK_8" name="CLKIN_IN" />
            <blockpin signalname="LCKD4" name="LOCKED_OUT" />
            <blockpin name="CLK90_OUT" />
            <blockpin signalname="CLK_8" name="CLK0_OUT" />
        </block>
        <block symbolname="CLOCK_MANAGER" name="XLXI_103">
            <blockpin signalname="CLK_25MHZ" name="CLKIN_IN" />
            <blockpin signalname="XLXN_2" name="RST_IN" />
            <blockpin signalname="Clock_Manager_FXOUT" name="CLKFX_OUT" />
            <blockpin signalname="XLXN_7" name="LOCKED_OUT" />
        </block>
        <block symbolname="buf" name="XLXI_105">
            <blockpin signalname="XLXN_172" name="I" />
            <blockpin signalname="SUD_LOCKED" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_106">
            <blockpin signalname="XLXN_172" name="I0" />
            <blockpin signalname="XLXN_79(8)" name="I1" />
            <blockpin signalname="ALIVE" name="O" />
        </block>
        <block symbolname="copy_of_and2" name="XLXI_107">
            <blockpin signalname="XLXN_106" name="I0" />
            <blockpin signalname="XLXN_113" name="I1" />
            <blockpin signalname="XLXN_172" name="O" />
            <blockpin signalname="CLK_8" name="C" />
        </block>
        <block symbolname="inv" name="XLXI_110">
            <blockpin signalname="XLXN_112" name="I" />
            <blockpin signalname="XLXN_181" name="O" />
        </block>
        <block symbolname="fdce" name="XLXI_26">
            <blockpin signalname="Clock_Manager_FXOUT" name="C" />
            <blockpin signalname="XLXN_44" name="CE" />
            <blockpin signalname="Reset" name="CLR" />
            <blockpin signalname="XLXN_70" name="D" />
            <blockpin signalname="CLK_1ms" name="Q" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="7040" height="5440">
        <iomarker fontsize="28" x="720" y="496" name="CLK_25MHZ" orien="R180" />
        <instance x="688" y="880" name="XLXI_3" orien="R0" />
        <branch name="XLXN_2">
            <wire x2="928" y1="848" y2="848" x1="912" />
            <wire x2="1024" y1="848" y2="848" x1="928" />
            <wire x2="1024" y1="848" y2="3536" x1="1024" />
            <wire x2="1680" y1="3536" y2="3536" x1="1024" />
            <wire x2="1024" y1="3536" y2="4048" x1="1024" />
            <wire x2="1920" y1="4048" y2="4048" x1="1024" />
            <wire x2="928" y1="688" y2="848" x1="928" />
            <wire x2="960" y1="688" y2="688" x1="928" />
        </branch>
        <branch name="RST">
            <wire x2="688" y1="848" y2="848" x1="656" />
        </branch>
        <iomarker fontsize="28" x="656" y="848" name="RST" orien="R180" />
        <instance x="1648" y="384" name="XLXI_4" orien="R0">
        </instance>
        <instance x="1664" y="528" name="XLXI_6" orien="R0" />
        <branch name="XLXN_6">
            <wire x2="1728" y1="528" y2="576" x1="1728" />
            <wire x2="1792" y1="576" y2="576" x1="1728" />
        </branch>
        <instance x="1616" y="768" name="XLXI_7" orien="R0" />
        <branch name="XLXN_7">
            <wire x2="1520" y1="1920" y2="1920" x1="1232" />
            <wire x2="1232" y1="1920" y2="2912" x1="1232" />
            <wire x2="2592" y1="2912" y2="2912" x1="1232" />
            <wire x2="2592" y1="2912" y2="3152" x1="2592" />
            <wire x2="2752" y1="3152" y2="3152" x1="2592" />
            <wire x2="2752" y1="3152" y2="3488" x1="2752" />
            <wire x2="2976" y1="3488" y2="3488" x1="2752" />
            <wire x2="2976" y1="3488" y2="3936" x1="2976" />
            <wire x2="1520" y1="688" y2="688" x1="1424" />
            <wire x2="1520" y1="688" y2="736" x1="1520" />
            <wire x2="1616" y1="736" y2="736" x1="1520" />
            <wire x2="1520" y1="736" y2="1920" x1="1520" />
            <wire x2="2752" y1="3488" y2="3488" x1="2656" />
        </branch>
        <instance x="2896" y="688" name="XLXI_9" orien="R0" />
        <instance x="2656" y="416" name="XLXI_10" orien="R0" />
        <branch name="XLXN_9">
            <wire x2="2720" y1="416" y2="560" x1="2720" />
            <wire x2="2896" y1="560" y2="560" x1="2720" />
        </branch>
        <instance x="2880" y="1184" name="XLXI_16" orien="R0" />
        <branch name="q(2:0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2177" y="592" type="branch" />
            <wire x2="2112" y1="592" y2="592" x1="2048" />
            <wire x2="2192" y1="592" y2="592" x1="2112" />
            <wire x2="2288" y1="592" y2="592" x1="2192" />
            <wire x2="2320" y1="592" y2="592" x1="2288" />
        </branch>
        <bustap x2="2288" y1="592" y2="688" x1="2288" />
        <bustap x2="2192" y1="592" y2="688" x1="2192" />
        <bustap x2="2112" y1="592" y2="688" x1="2112" />
        <branch name="q(0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2553" y="928" type="branch" />
            <wire x2="2288" y1="688" y2="928" x1="2288" />
            <wire x2="2880" y1="928" y2="928" x1="2288" />
        </branch>
        <instance x="2864" y="1664" name="XLXI_20" orien="R0" />
        <branch name="q(1)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2482" y="1408" type="branch" />
            <wire x2="2192" y1="688" y2="1408" x1="2192" />
            <wire x2="2864" y1="1408" y2="1408" x1="2192" />
        </branch>
        <instance x="2880" y="2176" name="XLXI_22" orien="R0" />
        <instance x="2224" y="2496" name="XLXI_24" orien="R0" />
        <instance x="1536" y="2688" name="XLXI_25" orien="R0" />
        <branch name="XLXN_39">
            <wire x2="2224" y1="2432" y2="2432" x1="1920" />
        </branch>
        <instance x="2944" y="2736" name="XLXI_28" orien="R0" />
        <instance x="2720" y="2496" name="XLXI_30" orien="R0" />
        <branch name="XLXN_53">
            <wire x2="2784" y1="2496" y2="2544" x1="2784" />
            <wire x2="2944" y1="2544" y2="2544" x1="2784" />
        </branch>
        <instance x="3424" y="2800" name="XLXI_31" orien="R0" />
        <branch name="XLXN_57(15:0)">
            <wire x2="3424" y1="2480" y2="2480" x1="3328" />
        </branch>
        <branch name="XLXN_58(15:0)">
            <wire x2="3376" y1="2752" y2="2752" x1="3248" />
            <wire x2="3248" y1="2752" y2="2816" x1="3248" />
            <wire x2="3376" y1="2672" y2="2752" x1="3376" />
            <wire x2="3424" y1="2672" y2="2672" x1="3376" />
        </branch>
        <instance x="2992" y="2976" name="XLXI_35" orien="R270" />
        <branch name="XLXN_62">
            <wire x2="2944" y1="2704" y2="2704" x1="2896" />
            <wire x2="2896" y1="2704" y2="2720" x1="2896" />
        </branch>
        <instance x="3216" y="2960" name="XLXI_36" orien="R270">
        </instance>
        <branch name="XLXN_63">
            <wire x2="2928" y1="2976" y2="2992" x1="2928" />
            <wire x2="3856" y1="2992" y2="2992" x1="2928" />
            <wire x2="3856" y1="2544" y2="2544" x1="3808" />
            <wire x2="3856" y1="2544" y2="2624" x1="3856" />
            <wire x2="3856" y1="2624" y2="2992" x1="3856" />
            <wire x2="4032" y1="2624" y2="2624" x1="3856" />
        </branch>
        <instance x="4032" y="2752" name="XLXI_37" orien="R0" />
        <instance x="3888" y="2432" name="XLXI_38" orien="R0" />
        <branch name="XLXN_66">
            <wire x2="3952" y1="2432" y2="2496" x1="3952" />
            <wire x2="4032" y1="2496" y2="2496" x1="3952" />
        </branch>
        <iomarker fontsize="28" x="5152" y="1392" name="CLK_4" orien="R0" />
        <iomarker fontsize="28" x="5168" y="1888" name="CLK_8" orien="R0" />
        <iomarker fontsize="28" x="5088" y="432" name="CLK_M" orien="R0" />
        <iomarker fontsize="28" x="5088" y="928" name="CLK_DDS" orien="R0" />
        <instance x="4528" y="3536" name="XLXI_46" orien="R0" />
        <instance x="4384" y="3280" name="XLXI_47" orien="R0" />
        <branch name="XLXN_77">
            <wire x2="4448" y1="3280" y2="3344" x1="4448" />
            <wire x2="4528" y1="3344" y2="3344" x1="4448" />
        </branch>
        <branch name="XLXN_79(15:0)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="5008" y="3280" type="branch" />
            <wire x2="5008" y1="3280" y2="3280" x1="4912" />
            <wire x2="5024" y1="3280" y2="3280" x1="5008" />
            <wire x2="5072" y1="3280" y2="3280" x1="5024" />
        </branch>
        <bustap x2="5024" y1="3280" y2="3376" x1="5024" />
        <branch name="XLXN_79(8)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="5102" y="3424" type="branch" />
            <wire x2="5024" y1="3376" y2="3424" x1="5024" />
            <wire x2="5200" y1="3424" y2="3424" x1="5024" />
        </branch>
        <instance x="1680" y="3568" name="XLXI_50" orien="R0" />
        <instance x="2016" y="3072" name="XLXI_51" orien="R180" />
        <branch name="XLXN_86">
            <wire x2="1664" y1="3104" y2="3376" x1="1664" />
            <wire x2="1680" y1="3376" y2="3376" x1="1664" />
            <wire x2="1792" y1="3104" y2="3104" x1="1664" />
        </branch>
        <text style="fontsize:28;fontname:Arial" x="1776" y="3580">Delay=FFFF</text>
        <instance x="2656" y="3360" name="XLXI_53" orien="R180" />
        <instance x="2192" y="3888" name="XLXI_54" orien="R0">
        </instance>
        <instance x="1920" y="4112" name="XLXI_55" orien="R0" />
        <branch name="XLXN_92">
            <wire x2="2384" y1="4016" y2="4016" x1="2176" />
            <wire x2="2384" y1="3936" y2="4016" x1="2384" />
        </branch>
        <branch name="XLXN_95">
            <wire x2="2768" y1="3856" y2="3856" x1="2576" />
            <wire x2="2768" y1="3856" y2="4000" x1="2768" />
            <wire x2="2976" y1="4000" y2="4000" x1="2768" />
            <wire x2="2768" y1="3840" y2="3856" x1="2768" />
        </branch>
        <branch name="XLXN_96">
            <wire x2="2768" y1="3552" y2="3552" x1="2656" />
            <wire x2="2768" y1="3552" y2="3616" x1="2768" />
        </branch>
        <instance x="2800" y="3840" name="XLXI_56" orien="R270" />
        <instance x="2976" y="4384" name="XLXI_57" orien="R0" />
        <instance x="2640" y="4384" name="XLXI_58" orien="R270" />
        <branch name="XLXN_106">
            <wire x2="3328" y1="4128" y2="4128" x1="3232" />
        </branch>
        <branch name="CLK_8">
            <attrtext style="alignment:SOFT-VRIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3440" y="4304" type="branch" />
            <wire x2="3440" y1="4256" y2="4304" x1="3440" />
        </branch>
        <branch name="XLXN_107">
            <wire x2="2400" y1="3488" y2="3488" x1="2080" />
            <wire x2="2080" y1="3488" y2="3808" x1="2080" />
            <wire x2="2192" y1="3808" y2="3808" x1="2080" />
        </branch>
        <instance x="3248" y="4480" name="XLXI_61" orien="R270" />
        <branch name="XLXN_113">
            <wire x2="3280" y1="4416" y2="4416" x1="3248" />
            <wire x2="3280" y1="4192" y2="4416" x1="3280" />
            <wire x2="3328" y1="4192" y2="4192" x1="3280" />
        </branch>
        <iomarker fontsize="28" x="5328" y="4160" name="SUD_LOCKED" orien="R0" />
        <instance x="5040" y="4608" name="XLXI_62" orien="R0" />
        <iomarker fontsize="28" x="5344" y="4320" name="CLK_FADC" orien="R0" />
        <branch name="CLK_FADC">
            <wire x2="5344" y1="4320" y2="4320" x1="5104" />
            <wire x2="5104" y1="4320" y2="4480" x1="5104" />
        </branch>
        <instance x="3600" y="640" name="XLXI_81" orien="R0">
        </instance>
        <instance x="3632" y="1072" name="XLXI_87" orien="R0">
        </instance>
        <instance x="3680" y="1600" name="XLXI_95" orien="R0">
        </instance>
        <branch name="CLK_4">
            <wire x2="2864" y1="2304" y2="2608" x1="2864" />
            <wire x2="2944" y1="2608" y2="2608" x1="2864" />
            <wire x2="4336" y1="2304" y2="2304" x1="2864" />
            <wire x2="4224" y1="1392" y2="1392" x1="4144" />
            <wire x2="4224" y1="1392" y2="1488" x1="4224" />
            <wire x2="4336" y1="1488" y2="1488" x1="4224" />
            <wire x2="4336" y1="1488" y2="2304" x1="4336" />
            <wire x2="5152" y1="1392" y2="1392" x1="4224" />
        </branch>
        <instance x="3648" y="2096" name="XLXI_98" orien="R0">
        </instance>
        <branch name="CLK_8">
            <wire x2="5168" y1="1888" y2="1888" x1="4112" />
        </branch>
        <branch name="XLXN_164">
            <wire x2="2976" y1="4320" y2="4320" x1="2640" />
        </branch>
        <branch name="LCKD1">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="4094" y="592" type="branch" />
            <wire x2="4128" y1="592" y2="592" x1="4064" />
        </branch>
        <branch name="LCKD1">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2912" y="4064" type="branch" />
            <wire x2="2976" y1="4064" y2="4064" x1="2912" />
        </branch>
        <branch name="LCKD2">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="4159" y="1072" type="branch" />
            <wire x2="4192" y1="1072" y2="1072" x1="4128" />
        </branch>
        <branch name="LCKD2">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2912" y="4128" type="branch" />
            <wire x2="2976" y1="4128" y2="4128" x1="2912" />
        </branch>
        <branch name="LCKD3">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="4182" y="1552" type="branch" />
            <wire x2="4224" y1="1552" y2="1552" x1="4144" />
        </branch>
        <branch name="LCKD3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2912" y="4192" type="branch" />
            <wire x2="2976" y1="4192" y2="4192" x1="2912" />
        </branch>
        <branch name="LCKD4">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="4141" y="2064" type="branch" />
            <wire x2="4176" y1="2064" y2="2064" x1="4112" />
        </branch>
        <branch name="LCKD4">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2912" y="4256" type="branch" />
            <wire x2="2976" y1="4256" y2="4256" x1="2912" />
        </branch>
        <branch name="CLK_M">
            <wire x2="5088" y1="432" y2="432" x1="4064" />
        </branch>
        <branch name="CLK_DDS">
            <wire x2="4448" y1="912" y2="912" x1="4128" />
            <wire x2="4448" y1="912" y2="928" x1="4448" />
            <wire x2="5088" y1="928" y2="928" x1="4448" />
        </branch>
        <instance x="960" y="736" name="XLXI_103" orien="R0">
        </instance>
        <iomarker fontsize="28" x="5680" y="3456" name="ALIVE" orien="R0" />
        <branch name="XLXN_172">
            <wire x2="4992" y1="4160" y2="4160" x1="3584" />
            <wire x2="5040" y1="4160" y2="4160" x1="4992" />
            <wire x2="4992" y1="3488" y2="4160" x1="4992" />
            <wire x2="5200" y1="3488" y2="3488" x1="4992" />
        </branch>
        <instance x="5040" y="4192" name="XLXI_105" orien="R0" />
        <branch name="SUD_LOCKED">
            <wire x2="5328" y1="4160" y2="4160" x1="5264" />
        </branch>
        <instance x="5200" y="3552" name="XLXI_106" orien="R0" />
        <instance x="3328" y="4064" name="XLXI_107" orien="M180">
        </instance>
        <branch name="CLK_25MHZ">
            <wire x2="832" y1="496" y2="496" x1="720" />
            <wire x2="960" y1="496" y2="496" x1="832" />
            <wire x2="832" y1="496" y2="800" x1="832" />
            <wire x2="1200" y1="800" y2="800" x1="832" />
            <wire x2="1200" y1="800" y2="3440" x1="1200" />
            <wire x2="1584" y1="3440" y2="3440" x1="1200" />
            <wire x2="1680" y1="3440" y2="3440" x1="1584" />
            <wire x2="1584" y1="3440" y2="3840" x1="1584" />
            <wire x2="2192" y1="3840" y2="3840" x1="1584" />
        </branch>
        <branch name="q(2)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2420" y="1920" type="branch" />
            <wire x2="1520" y1="2224" y2="2432" x1="1520" />
            <wire x2="1536" y1="2432" y2="2432" x1="1520" />
            <wire x2="2112" y1="2224" y2="2224" x1="1520" />
            <wire x2="2112" y1="2224" y2="2368" x1="2112" />
            <wire x2="2224" y1="2368" y2="2368" x1="2112" />
            <wire x2="2112" y1="688" y2="1920" x1="2112" />
            <wire x2="2112" y1="1920" y2="2224" x1="2112" />
            <wire x2="2880" y1="1920" y2="1920" x1="2112" />
        </branch>
        <branch name="Clock_Manager_FXOUT">
            <wire x2="1616" y1="496" y2="496" x1="1424" />
            <wire x2="1696" y1="496" y2="496" x1="1616" />
            <wire x2="1696" y1="496" y2="608" x1="1696" />
            <wire x2="1792" y1="608" y2="608" x1="1696" />
            <wire x2="1424" y1="2048" y2="2560" x1="1424" />
            <wire x2="1536" y1="2560" y2="2560" x1="1424" />
            <wire x2="2624" y1="2048" y2="2048" x1="1424" />
            <wire x2="2880" y1="2048" y2="2048" x1="2624" />
            <wire x2="2624" y1="2048" y2="2160" x1="2624" />
            <wire x2="3408" y1="2160" y2="2160" x1="2624" />
            <wire x2="3408" y1="2160" y2="2944" x1="3408" />
            <wire x2="4560" y1="2944" y2="2944" x1="3408" />
            <wire x2="1616" y1="368" y2="496" x1="1616" />
            <wire x2="1952" y1="368" y2="368" x1="1616" />
            <wire x2="1952" y1="368" y2="432" x1="1952" />
            <wire x2="2624" y1="432" y2="432" x1="1952" />
            <wire x2="2896" y1="432" y2="432" x1="2624" />
            <wire x2="2624" y1="432" y2="1056" x1="2624" />
            <wire x2="2880" y1="1056" y2="1056" x1="2624" />
            <wire x2="2624" y1="1056" y2="1536" x1="2624" />
            <wire x2="2864" y1="1536" y2="1536" x1="2624" />
            <wire x2="2624" y1="1536" y2="2048" x1="2624" />
        </branch>
        <branch name="ALIVE">
            <wire x2="5680" y1="3456" y2="3456" x1="5456" />
        </branch>
        <branch name="S_CLK_M">
            <wire x2="3600" y1="432" y2="432" x1="3280" />
        </branch>
        <branch name="S_CLK_DDS">
            <wire x2="3632" y1="928" y2="928" x1="3264" />
        </branch>
        <branch name="S_CLK_4">
            <wire x2="3456" y1="1408" y2="1408" x1="3248" />
            <wire x2="3456" y1="1392" y2="1408" x1="3456" />
            <wire x2="3680" y1="1392" y2="1392" x1="3456" />
        </branch>
        <branch name="S_CLK_8">
            <wire x2="3456" y1="1920" y2="1920" x1="3264" />
            <wire x2="3456" y1="1888" y2="1920" x1="3456" />
            <wire x2="3648" y1="1888" y2="1888" x1="3456" />
        </branch>
        <branch name="XLXN_112">
            <wire x2="1488" y1="3648" y2="3984" x1="1488" />
            <wire x2="1616" y1="3984" y2="3984" x1="1488" />
            <wire x2="2192" y1="3648" y2="3648" x1="1488" />
            <wire x2="2176" y1="3104" y2="3104" x1="2016" />
            <wire x2="2176" y1="3104" y2="3264" x1="2176" />
            <wire x2="2176" y1="3264" y2="3440" x1="2176" />
            <wire x2="2192" y1="3440" y2="3440" x1="2176" />
            <wire x2="2192" y1="3440" y2="3648" x1="2192" />
            <wire x2="2656" y1="3264" y2="3264" x1="2176" />
            <wire x2="2656" y1="3264" y2="3424" x1="2656" />
            <wire x2="2176" y1="3440" y2="3440" x1="2064" />
        </branch>
        <branch name="XLXN_181">
            <wire x2="1920" y1="3984" y2="3984" x1="1840" />
        </branch>
        <instance x="1616" y="4016" name="XLXI_110" orien="R0" />
        <branch name="Reset">
            <wire x2="1536" y1="2656" y2="2720" x1="1536" />
            <wire x2="2688" y1="2720" y2="2720" x1="1536" />
            <wire x2="2688" y1="2720" y2="2992" x1="2688" />
            <wire x2="2864" y1="2992" y2="2992" x1="2688" />
            <wire x2="2688" y1="2992" y2="3040" x1="2688" />
            <wire x2="4032" y1="3040" y2="3040" x1="2688" />
            <wire x2="4032" y1="3040" y2="3504" x1="4032" />
            <wire x2="4528" y1="3504" y2="3504" x1="4032" />
            <wire x2="4560" y1="3040" y2="3040" x1="4032" />
            <wire x2="1920" y1="736" y2="736" x1="1840" />
            <wire x2="2688" y1="736" y2="736" x1="1920" />
            <wire x2="2896" y1="736" y2="736" x1="2688" />
            <wire x2="3584" y1="736" y2="736" x1="2896" />
            <wire x2="2688" y1="736" y2="1152" x1="2688" />
            <wire x2="2880" y1="1152" y2="1152" x1="2688" />
            <wire x2="2688" y1="1152" y2="1200" x1="2688" />
            <wire x2="3376" y1="1200" y2="1200" x1="2688" />
            <wire x2="2688" y1="1200" y2="1632" x1="2688" />
            <wire x2="2864" y1="1632" y2="1632" x1="2688" />
            <wire x2="2688" y1="1632" y2="1680" x1="2688" />
            <wire x2="3552" y1="1680" y2="1680" x1="2688" />
            <wire x2="2688" y1="1680" y2="2144" x1="2688" />
            <wire x2="2880" y1="2144" y2="2144" x1="2688" />
            <wire x2="2688" y1="2144" y2="2208" x1="2688" />
            <wire x2="3328" y1="2208" y2="2208" x1="2688" />
            <wire x2="2688" y1="2208" y2="2720" x1="2688" />
            <wire x2="1920" y1="672" y2="736" x1="1920" />
            <wire x2="2864" y1="2976" y2="2992" x1="2864" />
            <wire x2="2896" y1="656" y2="736" x1="2896" />
            <wire x2="3648" y1="1936" y2="1936" x1="3328" />
            <wire x2="3328" y1="1936" y2="2208" x1="3328" />
            <wire x2="3632" y1="960" y2="960" x1="3376" />
            <wire x2="3376" y1="960" y2="1200" x1="3376" />
            <wire x2="3616" y1="1488" y2="1488" x1="3552" />
            <wire x2="3552" y1="1488" y2="1680" x1="3552" />
            <wire x2="3600" y1="464" y2="464" x1="3584" />
            <wire x2="3584" y1="464" y2="736" x1="3584" />
            <wire x2="3680" y1="1424" y2="1424" x1="3616" />
            <wire x2="3616" y1="1424" y2="1488" x1="3616" />
            <wire x2="4032" y1="2720" y2="3040" x1="4032" />
        </branch>
        <instance x="4560" y="3072" name="XLXI_26" orien="R0" />
        <branch name="XLXN_44">
            <wire x2="2480" y1="2384" y2="2400" x1="2480" />
            <wire x2="3392" y1="2384" y2="2384" x1="2480" />
            <wire x2="3392" y1="2384" y2="2880" x1="3392" />
            <wire x2="4560" y1="2880" y2="2880" x1="3392" />
        </branch>
        <branch name="XLXN_70">
            <wire x2="4464" y1="2496" y2="2496" x1="4416" />
            <wire x2="4464" y1="2496" y2="2544" x1="4464" />
            <wire x2="4544" y1="2544" y2="2544" x1="4464" />
            <wire x2="4544" y1="2544" y2="2816" x1="4544" />
            <wire x2="4560" y1="2816" y2="2816" x1="4544" />
        </branch>
        <branch name="CLK_1ms">
            <wire x2="4400" y1="3088" y2="3408" x1="4400" />
            <wire x2="4528" y1="3408" y2="3408" x1="4400" />
            <wire x2="5008" y1="3088" y2="3088" x1="4400" />
            <wire x2="5008" y1="2816" y2="2816" x1="4944" />
            <wire x2="5008" y1="2816" y2="2832" x1="5008" />
            <wire x2="5008" y1="2832" y2="3088" x1="5008" />
            <wire x2="5344" y1="2832" y2="2832" x1="5008" />
        </branch>
        <iomarker fontsize="28" x="5344" y="2832" name="CLK_1ms" orien="R0" />
    </sheet>
</drawing>