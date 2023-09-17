<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3a" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="CLK_8" />
        <signal name="CLK_DDS" />
        <signal name="Frequ_Val(23:0)" />
        <signal name="RST" />
        <signal name="F" />
        <signal name="FMOT" />
        <signal name="Phase_Acc_Val(31:0)" />
        <signal name="XLXN_13" />
        <port polarity="Input" name="CLK_8" />
        <port polarity="Input" name="CLK_DDS" />
        <port polarity="Input" name="Frequ_Val(23:0)" />
        <port polarity="Input" name="RST" />
        <port polarity="Input" name="F" />
        <port polarity="Input" name="FMOT" />
        <port polarity="Output" name="Phase_Acc_Val(31:0)" />
        <blockdef name="DDS_FGEN">
            <timestamp>2021-3-9T19:37:55</timestamp>
            <rect width="352" x="64" y="-256" height="256" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <rect width="64" x="416" y="-236" height="24" />
            <line x2="480" y1="-224" y2="-224" x1="416" />
        </blockdef>
        <block symbolname="DDS_FGEN" name="XLXI_4">
            <blockpin signalname="CLK_8" name="CLK_8" />
            <blockpin signalname="CLK_DDS" name="CLK_DDS" />
            <blockpin signalname="Frequ_Val(23:0)" name="Frequ_Val(23:0)" />
            <blockpin signalname="RST" name="RST_INV" />
            <blockpin signalname="Phase_Acc_Val(31:0)" name="Phase_Acc_Val(31:0)" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <branch name="CLK_8">
            <wire x2="1344" y1="1008" y2="1008" x1="1088" />
        </branch>
        <branch name="CLK_DDS">
            <wire x2="1344" y1="1136" y2="1136" x1="1136" />
        </branch>
        <branch name="Frequ_Val(23:0)">
            <wire x2="1344" y1="1072" y2="1072" x1="1200" />
        </branch>
        <iomarker fontsize="28" x="1200" y="1072" name="Frequ_Val(23:0)" orien="R180" />
        <branch name="RST">
            <wire x2="1344" y1="944" y2="944" x1="1056" />
        </branch>
        <iomarker fontsize="28" x="1136" y="1136" name="CLK_DDS" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1008" name="CLK_8" orien="R180" />
        <iomarker fontsize="28" x="1056" y="944" name="RST" orien="R180" />
        <instance x="1344" y="1168" name="XLXI_4" orien="R0">
        </instance>
        <branch name="F">
            <wire x2="1456" y1="608" y2="608" x1="912" />
            <wire x2="1456" y1="608" y2="640" x1="1456" />
            <wire x2="1456" y1="640" y2="656" x1="1456" />
        </branch>
        <branch name="FMOT">
            <wire x2="1120" y1="688" y2="688" x1="976" />
            <wire x2="1248" y1="688" y2="688" x1="1120" />
            <wire x2="1248" y1="688" y2="720" x1="1248" />
        </branch>
        <iomarker fontsize="28" x="976" y="688" name="FMOT" orien="R180" />
        <iomarker fontsize="28" x="912" y="608" name="F" orien="R180" />
        <branch name="Phase_Acc_Val(31:0)">
            <wire x2="1840" y1="944" y2="944" x1="1824" />
            <wire x2="2016" y1="944" y2="944" x1="1840" />
        </branch>
        <iomarker fontsize="28" x="2016" y="944" name="Phase_Acc_Val(31:0)" orien="R0" />
    </sheet>
</drawing>