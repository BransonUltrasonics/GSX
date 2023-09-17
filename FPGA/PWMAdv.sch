<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3a" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="PWM_STOP" />
        <signal name="XLXN_24" />
        <signal name="XLXN_28" />
        <signal name="XLXN_33" />
        <signal name="XLXN_34" />
        <signal name="XLXN_35" />
        <signal name="XLXN_36" />
        <signal name="CLK_DDS" />
        <signal name="SUM(31)" />
        <signal name="XLXN_48" />
        <signal name="XLXN_49" />
        <signal name="XLXN_55" />
        <signal name="XLXN_56" />
        <signal name="PWMA" />
        <signal name="PWMB" />
        <signal name="PWM_LVL_RST_STOP" />
        <signal name="DIFF(31)" />
        <signal name="XLXN_163" />
        <signal name="RST" />
        <signal name="F" />
        <signal name="XLXN_175" />
        <signal name="XLXN_178" />
        <signal name="XLXN_182" />
        <signal name="XLXN_183" />
        <signal name="FMOT" />
        <signal name="XLXN_193" />
        <signal name="XLXN_194" />
        <signal name="Phase_Acc_Val(31:0)" />
        <signal name="RST_INV" />
        <signal name="Pwm_Val(15:0)" />
        <signal name="SUM(31:0)" />
        <signal name="DIFF(31:0)" />
        <signal name="XLXN_227(31:0)" />
        <signal name="XLXN_228(31:0)" />
        <signal name="XLXN_230" />
        <signal name="XLXN_231" />
        <signal name="XLXN_241" />
        <signal name="XLXN_242" />
        <signal name="XLXN_244" />
        <signal name="XLXN_245" />
        <signal name="XLXN_248" />
        <signal name="XLXN_249" />
        <signal name="XLXN_251" />
        <signal name="XLXN_257" />
        <signal name="XLXN_262" />
        <signal name="XLXN_280" />
        <signal name="XLXN_286" />
        <signal name="XLXN_289" />
        <signal name="XLXN_293" />
        <signal name="XLXN_294" />
        <signal name="XLXN_302" />
        <signal name="XLXN_311" />
        <signal name="Phase_Acc_Val(30)" />
        <signal name="Phase_Acc_Val(31)" />
        <signal name="XLXN_350" />
        <signal name="XLXN_357" />
        <signal name="XLXN_362" />
        <signal name="XLXN_375" />
        <signal name="XLXN_376" />
        <signal name="XLXN_377" />
        <signal name="XLXN_379" />
        <signal name="XLXN_380" />
        <signal name="XLXN_381" />
        <signal name="XLXN_411(31:0)" />
        <signal name="XLXN_411(30:31)" />
        <signal name="XLXN_411(13:0)" />
        <signal name="XLXN_416(15:0)" />
        <signal name="XLXN_418" />
        <signal name="CLK_8" />
        <signal name="PWMAdvTest(15:0)" />
        <signal name="XLXN_421" />
        <signal name="XLXN_411(29:14)" />
        <signal name="XLXN_432" />
        <signal name="XLXN_433" />
        <signal name="XLXN_434(1:0)" />
        <signal name="PWM_Reg_31(1:0)" />
        <signal name="XLXN_434(0)" />
        <signal name="XLXN_434(1)" />
        <signal name="XLXN_435" />
        <signal name="XLXN_436" />
        <port polarity="Input" name="PWM_STOP" />
        <port polarity="Input" name="CLK_DDS" />
        <port polarity="Output" name="PWMA" />
        <port polarity="Output" name="PWMB" />
        <port polarity="Input" name="PWM_LVL_RST_STOP" />
        <port polarity="Output" name="F" />
        <port polarity="Output" name="FMOT" />
        <port polarity="Input" name="Phase_Acc_Val(31:0)" />
        <port polarity="Input" name="RST_INV" />
        <port polarity="Input" name="Pwm_Val(15:0)" />
        <port polarity="Input" name="CLK_8" />
        <port polarity="Output" name="PWMAdvTest(15:0)" />
        <port polarity="Output" name="PWM_Reg_31(1:0)" />
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
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
        <blockdef name="and3b1">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
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
        <blockdef name="xor2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="60" y1="-128" y2="-128" x1="0" />
            <line x2="208" y1="-96" y2="-96" x1="256" />
            <arc ex="44" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <arc ex="64" ey="-144" sx="64" sy="-48" r="56" cx="32" cy="-96" />
            <line x2="64" y1="-144" y2="-144" x1="128" />
            <line x2="64" y1="-48" y2="-48" x1="128" />
            <arc ex="128" ey="-144" sx="208" sy="-96" r="88" cx="132" cy="-56" />
            <arc ex="208" ey="-96" sx="128" sy="-48" r="88" cx="132" cy="-136" />
        </blockdef>
        <blockdef name="fjkc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-320" y2="-320" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <rect width="256" x="64" y="-384" height="320" />
        </blockdef>
        <blockdef name="fd16ce">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <rect width="64" x="320" y="-268" height="24" />
            <rect width="64" x="0" y="-268" height="24" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <rect width="256" x="64" y="-320" height="256" />
        </blockdef>
        <blockdef name="vcc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-64" x1="64" />
            <line x2="64" y1="0" y2="-32" x1="64" />
            <line x2="32" y1="-64" y2="-64" x1="96" />
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
        <blockdef name="comp32">
            <timestamp>2021-3-9T14:16:54</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-64" y2="-64" x1="320" />
        </blockdef>
        <blockdef name="constant">
            <timestamp>2006-1-1T10:10:10</timestamp>
            <rect width="112" x="0" y="0" height="64" />
            <line x2="112" y1="32" y2="32" x1="144" />
        </blockdef>
        <blockdef name="sr4ce">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-320" y2="-320" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="320" y1="-320" y2="-320" x1="384" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="320" y1="-192" y2="-192" x1="384" />
            <line x2="320" y1="-128" y2="-128" x1="384" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <rect width="256" x="64" y="-384" height="320" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
        </blockdef>
        <blockdef name="copy_of_and2b1">
            <timestamp>2021-3-8T21:26:15</timestamp>
            <line x2="64" y1="-48" y2="-144" x1="64" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="112" y1="-176" y2="-144" x1="112" />
            <line x2="112" y1="-144" y2="-132" x1="104" />
            <line x2="120" y1="-132" y2="-144" x1="112" />
        </blockdef>
        <blockdef name="copy_of_or2">
            <timestamp>2021-3-10T17:6:28</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="192" ey="-96" sx="112" sy="-48" r="88" cx="116" cy="-136" />
            <arc ex="48" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <line x2="48" y1="-144" y2="-144" x1="112" />
            <arc ex="112" ey="-144" sx="192" sy="-96" r="88" cx="116" cy="-56" />
            <line x2="48" y1="-48" y2="-48" x1="112" />
            <line x2="112" y1="-176" y2="-144" x1="112" />
            <line x2="112" y1="-144" y2="-136" x1="104" />
            <line x2="120" y1="-136" y2="-144" x1="112" />
        </blockdef>
        <blockdef name="sr4cle">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-320" y2="-320" x1="0" />
            <line x2="320" y1="-448" y2="-448" x1="384" />
            <line x2="320" y1="-512" y2="-512" x1="384" />
            <line x2="320" y1="-576" y2="-576" x1="384" />
            <line x2="320" y1="-640" y2="-640" x1="384" />
            <line x2="64" y1="-448" y2="-448" x1="0" />
            <line x2="64" y1="-512" y2="-512" x1="0" />
            <line x2="64" y1="-576" y2="-576" x1="0" />
            <line x2="64" y1="-640" y2="-640" x1="0" />
            <line x2="64" y1="-704" y2="-704" x1="0" />
            <rect width="256" x="64" y="-768" height="704" />
        </blockdef>
        <blockdef name="nor4">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="48" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="48" y1="-256" y2="-256" x1="0" />
            <line x2="216" y1="-160" y2="-160" x1="256" />
            <circle r="12" cx="204" cy="-160" />
            <line x2="48" y1="-208" y2="-208" x1="112" />
            <arc ex="112" ey="-208" sx="192" sy="-160" r="88" cx="116" cy="-120" />
            <line x2="48" y1="-112" y2="-112" x1="112" />
            <line x2="48" y1="-256" y2="-208" x1="48" />
            <line x2="48" y1="-64" y2="-112" x1="48" />
            <arc ex="48" ey="-208" sx="48" sy="-112" r="56" cx="16" cy="-160" />
            <arc ex="192" ey="-160" sx="112" sy="-112" r="88" cx="116" cy="-200" />
        </blockdef>
        <blockdef name="or3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="48" y1="-64" y2="-64" x1="0" />
            <line x2="72" y1="-128" y2="-128" x1="0" />
            <line x2="48" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <arc ex="192" ey="-128" sx="112" sy="-80" r="88" cx="116" cy="-168" />
            <arc ex="48" ey="-176" sx="48" sy="-80" r="56" cx="16" cy="-128" />
            <line x2="48" y1="-64" y2="-80" x1="48" />
            <line x2="48" y1="-192" y2="-176" x1="48" />
            <line x2="48" y1="-80" y2="-80" x1="112" />
            <arc ex="112" ey="-176" sx="192" sy="-128" r="88" cx="116" cy="-88" />
            <line x2="48" y1="-176" y2="-176" x1="112" />
        </blockdef>
        <blockdef name="copy_of_and3b1">
            <timestamp>2021-3-10T17:46:51</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
            <line x2="128" y1="-224" y2="-176" x1="128" />
            <line x2="128" y1="-176" y2="-168" x1="120" />
            <line x2="136" y1="-168" y2="-176" x1="128" />
        </blockdef>
        <blockdef name="add32">
            <timestamp>2021-4-5T3:42:35</timestamp>
            <line x2="64" y1="-144" y2="-216" x1="64" />
            <line x2="96" y1="-144" y2="-112" x1="64" />
            <line x2="64" y1="-80" y2="-8" x1="64" />
            <line x2="272" y1="-36" y2="-184" x1="272" />
            <line x2="64" y1="-36" y2="-8" x1="272" />
            <line x2="124" y1="-196" y2="-208" x1="112" />
            <line x2="104" y1="-196" y2="-208" x1="112" />
            <line x2="336" y1="-112" y2="-112" x1="272" />
            <rect width="64" x="272" y="-124" height="24" />
            <line x2="64" y1="-112" y2="-80" x1="96" />
            <line x2="112" y1="-208" y2="-240" x1="112" />
            <line x2="272" y1="-216" y2="-184" x1="64" />
            <line x2="0" y1="-176" y2="-176" x1="64" />
            <rect width="64" x="0" y="-188" height="24" />
            <line x2="0" y1="-48" y2="-48" x1="64" />
            <rect width="64" x="0" y="-60" height="24" />
            <line x2="224" y1="-240" y2="-192" x1="224" />
        </blockdef>
        <blockdef name="fjkpc">
            <timestamp>2021-4-2T18:47:15</timestamp>
            <rect width="256" x="64" y="-320" height="320" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
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
        <blockdef name="buf">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="128" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="0" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="-64" x1="128" />
            <line x2="64" y1="-64" y2="0" x1="64" />
        </blockdef>
        <block symbolname="or2" name="XLXI_2">
            <blockpin signalname="RST" name="I0" />
            <blockpin signalname="PWM_STOP" name="I1" />
            <blockpin signalname="XLXN_24" name="O" />
        </block>
        <block symbolname="and2b1" name="XLXI_3">
            <blockpin signalname="PWM_LVL_RST_STOP" name="I0" />
            <blockpin signalname="XLXN_24" name="I1" />
            <blockpin signalname="XLXN_28" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_4">
            <blockpin signalname="PWM_LVL_RST_STOP" name="I0" />
            <blockpin signalname="RST" name="I1" />
            <blockpin signalname="XLXN_357" name="O" />
        </block>
        <block symbolname="and2b1" name="XLXI_5">
            <blockpin signalname="PWM_LVL_RST_STOP" name="I0" />
            <blockpin signalname="RST" name="I1" />
            <blockpin signalname="XLXN_350" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_6">
            <blockpin signalname="FMOT" name="I0" />
            <blockpin signalname="XLXN_24" name="I1" />
            <blockpin signalname="PWM_LVL_RST_STOP" name="I2" />
            <blockpin signalname="XLXN_33" name="O" />
        </block>
        <block symbolname="and3b1" name="XLXI_7">
            <blockpin signalname="XLXN_24" name="I0" />
            <blockpin signalname="FMOT" name="I1" />
            <blockpin signalname="PWM_LVL_RST_STOP" name="I2" />
            <blockpin signalname="XLXN_34" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_8">
            <blockpin signalname="FMOT" name="I0" />
            <blockpin signalname="PWM_LVL_RST_STOP" name="I1" />
            <blockpin signalname="XLXN_28" name="I2" />
            <blockpin signalname="XLXN_35" name="O" />
        </block>
        <block symbolname="and3b3" name="XLXI_9">
            <blockpin signalname="FMOT" name="I0" />
            <blockpin signalname="XLXN_28" name="I1" />
            <blockpin signalname="PWM_LVL_RST_STOP" name="I2" />
            <blockpin signalname="XLXN_36" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_17">
            <blockpin signalname="PWMA" name="I0" />
            <blockpin signalname="FMOT" name="I1" />
            <blockpin signalname="SUM(31)" name="I2" />
            <blockpin signalname="XLXN_48" name="O" />
        </block>
        <block symbolname="and3b1" name="XLXI_18">
            <blockpin signalname="SUM(31)" name="I0" />
            <blockpin signalname="FMOT" name="I1" />
            <blockpin signalname="PWMA" name="I2" />
            <blockpin signalname="XLXN_49" name="O" />
        </block>
        <block symbolname="and3b1" name="XLXI_19">
            <blockpin signalname="PWMB" name="I0" />
            <blockpin signalname="FMOT" name="I1" />
            <blockpin signalname="DIFF(31)" name="I2" />
            <blockpin signalname="XLXN_55" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_23">
            <blockpin signalname="DIFF(31)" name="I0" />
            <blockpin signalname="FMOT" name="I1" />
            <blockpin signalname="PWMB" name="I2" />
            <blockpin signalname="XLXN_56" name="O" />
        </block>
        <block symbolname="xor2" name="XLXI_28">
            <blockpin signalname="Phase_Acc_Val(31)" name="I0" />
            <blockpin signalname="Phase_Acc_Val(30)" name="I1" />
            <blockpin signalname="XLXN_163" name="O" />
        </block>
        <block symbolname="fjkc" name="XLXI_30">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_175" name="J" />
            <blockpin signalname="XLXN_178" name="K" />
            <blockpin signalname="FMOT" name="Q" />
        </block>
        <block symbolname="and3b1" name="XLXI_31">
            <blockpin signalname="FMOT" name="I0" />
            <blockpin signalname="XLXN_183" name="I1" />
            <blockpin signalname="XLXN_182" name="I2" />
            <blockpin signalname="XLXN_175" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_32">
            <blockpin signalname="XLXN_182" name="I0" />
            <blockpin signalname="XLXN_183" name="I1" />
            <blockpin signalname="FMOT" name="I2" />
            <blockpin signalname="XLXN_178" name="O" />
        </block>
        <block symbolname="vcc" name="XLXI_47">
            <blockpin signalname="XLXN_194" name="P" />
        </block>
        <block symbolname="gnd" name="XLXI_48">
            <blockpin signalname="XLXN_193" name="G" />
        </block>
        <block symbolname="inv" name="XLXI_49">
            <blockpin signalname="RST_INV" name="I" />
            <blockpin signalname="RST" name="O" />
        </block>
        <block symbolname="gnd" name="XLXI_51(13:0)">
            <blockpin signalname="XLXN_411(13:0)" name="G" />
        </block>
        <block symbolname="gnd" name="XLXI_52(1:0)">
            <blockpin signalname="XLXN_411(30:31)" name="G" />
        </block>
        <block symbolname="fdc" name="XLXI_57">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_163" name="D" />
            <blockpin signalname="XLXN_183" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_58">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_183" name="D" />
            <blockpin signalname="F" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_60">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="Phase_Acc_Val(31)" name="D" />
            <blockpin signalname="XLXN_182" name="Q" />
        </block>
        <block symbolname="comp32" name="XLXI_61">
            <blockpin signalname="Phase_Acc_Val(31:0)" name="A(31:0)" />
            <blockpin signalname="XLXN_227(31:0)" name="B(31:0)" />
            <blockpin signalname="XLXN_376" name="GT" />
        </block>
        <block symbolname="comp32" name="XLXI_62">
            <blockpin signalname="Phase_Acc_Val(31:0)" name="A(31:0)" />
            <blockpin signalname="XLXN_228(31:0)" name="B(31:0)" />
            <blockpin signalname="XLXN_380" name="GT" />
        </block>
        <block symbolname="constant" name="XLXI_63">
            <attr value="C0000000" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="XLXN_228(31:0)" name="O" />
        </block>
        <block symbolname="constant" name="XLXI_64">
            <attr value="40000000" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="XLXN_227(31:0)" name="O" />
        </block>
        <block symbolname="sr4ce" name="XLXI_65">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="XLXN_231" name="CE" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_375" name="SLI" />
            <blockpin signalname="XLXN_245" name="Q0" />
            <blockpin name="Q1" />
            <blockpin signalname="XLXN_244" name="Q2" />
            <blockpin name="Q3" />
        </block>
        <block symbolname="sr4ce" name="XLXI_66">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="XLXN_230" name="CE" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_379" name="SLI" />
            <blockpin signalname="XLXN_242" name="Q0" />
            <blockpin name="Q1" />
            <blockpin signalname="XLXN_241" name="Q2" />
            <blockpin name="Q3" />
        </block>
        <block symbolname="vcc" name="XLXI_67">
            <blockpin signalname="XLXN_231" name="P" />
        </block>
        <block symbolname="vcc" name="XLXI_68">
            <blockpin signalname="XLXN_230" name="P" />
        </block>
        <block symbolname="copy_of_and2b1" name="XLXI_70">
            <blockpin signalname="XLXN_241" name="I0" />
            <blockpin signalname="XLXN_242" name="I1" />
            <blockpin signalname="XLXN_249" name="O" />
            <blockpin signalname="CLK_DDS" name="c" />
        </block>
        <block symbolname="copy_of_and2b1" name="XLXI_71">
            <blockpin signalname="XLXN_244" name="I0" />
            <blockpin signalname="XLXN_245" name="I1" />
            <blockpin signalname="XLXN_248" name="O" />
            <blockpin signalname="CLK_DDS" name="c" />
        </block>
        <block symbolname="copy_of_or2" name="XLXI_73">
            <blockpin signalname="XLXN_248" name="I0" />
            <blockpin signalname="XLXN_249" name="I1" />
            <blockpin name="O" />
            <blockpin signalname="CLK_DDS" name="C" />
        </block>
        <block symbolname="sr4cle" name="XLXI_74">
            <blockpin signalname="CLK_8" name="C" />
            <blockpin signalname="XLXN_262" name="CE" />
            <blockpin signalname="XLXN_251" name="CLR" />
            <blockpin signalname="XLXN_262" name="D0" />
            <blockpin signalname="XLXN_257" name="D1" />
            <blockpin signalname="XLXN_257" name="D2" />
            <blockpin signalname="XLXN_257" name="D3" />
            <blockpin signalname="XLXN_311" name="L" />
            <blockpin signalname="XLXN_257" name="SLI" />
            <blockpin signalname="XLXN_280" name="Q0" />
            <blockpin signalname="XLXN_286" name="Q1" />
            <blockpin signalname="XLXN_293" name="Q2" />
            <blockpin signalname="XLXN_289" name="Q3" />
        </block>
        <block symbolname="gnd" name="XLXI_75">
            <blockpin signalname="XLXN_251" name="G" />
        </block>
        <block symbolname="vcc" name="XLXI_76">
            <blockpin signalname="XLXN_262" name="P" />
        </block>
        <block symbolname="gnd" name="XLXI_77">
            <blockpin signalname="XLXN_257" name="G" />
        </block>
        <block symbolname="fd16ce" name="XLXI_46">
            <blockpin signalname="CLK_8" name="C" />
            <blockpin signalname="XLXN_280" name="CE" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="Pwm_Val(15:0)" name="D(15:0)" />
            <blockpin signalname="XLXN_416(15:0)" name="Q(15:0)" />
        </block>
        <block symbolname="nor4" name="XLXI_78">
            <blockpin signalname="XLXN_280" name="I0" />
            <blockpin signalname="XLXN_286" name="I1" />
            <blockpin signalname="XLXN_293" name="I2" />
            <blockpin signalname="XLXN_289" name="I3" />
            <blockpin signalname="XLXN_302" name="O" />
        </block>
        <block symbolname="sr4ce" name="XLXI_80">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="XLXN_294" name="CE" />
            <blockpin signalname="XLXN_289" name="CLR" />
            <blockpin signalname="XLXN_293" name="SLI" />
            <blockpin signalname="XLXN_433" name="Q0" />
            <blockpin name="Q1" />
            <blockpin signalname="XLXN_432" name="Q2" />
            <blockpin name="Q3" />
        </block>
        <block symbolname="vcc" name="XLXI_81">
            <blockpin signalname="XLXN_294" name="P" />
        </block>
        <block symbolname="or3" name="XLXI_82">
            <blockpin signalname="XLXN_302" name="I0" />
            <blockpin signalname="XLXN_289" name="I1" />
            <blockpin signalname="RST" name="I2" />
            <blockpin signalname="XLXN_311" name="O" />
        </block>
        <block symbolname="copy_of_and3b1" name="XLXI_86">
            <blockpin signalname="XLXN_432" name="I0" />
            <blockpin signalname="XLXN_421" name="I1" />
            <blockpin signalname="XLXN_433" name="I2" />
            <blockpin signalname="XLXN_418" name="O" />
            <blockpin signalname="CLK_DDS" name="C" />
        </block>
        <block symbolname="add32" name="XLXI_105">
            <blockpin signalname="Phase_Acc_Val(31:0)" name="A(31:0)" />
            <blockpin signalname="XLXN_194" name="ADD" />
            <blockpin signalname="XLXN_411(31:0)" name="B(31:0)" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="SUM(31:0)" name="S(31:0)" />
        </block>
        <block symbolname="add32" name="XLXI_106">
            <blockpin signalname="Phase_Acc_Val(31:0)" name="A(31:0)" />
            <blockpin signalname="XLXN_193" name="ADD" />
            <blockpin signalname="XLXN_411(31:0)" name="B(31:0)" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="DIFF(31:0)" name="S(31:0)" />
        </block>
        <block symbolname="fjkpc" name="XLXI_109">
            <blockpin signalname="XLXN_357" name="PRE" />
            <blockpin signalname="XLXN_33" name="J" />
            <blockpin signalname="XLXN_34" name="K" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin name="Q" />
            <blockpin signalname="XLXN_350" name="CLR" />
        </block>
        <block symbolname="fjkpc" name="XLXI_110">
            <blockpin signalname="XLXN_350" name="PRE" />
            <blockpin signalname="XLXN_35" name="J" />
            <blockpin signalname="XLXN_36" name="K" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="XLXN_362" name="Q" />
            <blockpin signalname="XLXN_357" name="CLR" />
        </block>
        <block symbolname="fjkpc" name="XLXI_111">
            <blockpin signalname="XLXN_436" name="PRE" />
            <blockpin signalname="XLXN_48" name="J" />
            <blockpin signalname="XLXN_49" name="K" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="PWMA" name="Q" />
            <blockpin signalname="XLXN_362" name="CLR" />
        </block>
        <block symbolname="fjkpc" name="XLXI_112">
            <blockpin signalname="XLXN_436" name="PRE" />
            <blockpin signalname="XLXN_55" name="J" />
            <blockpin signalname="XLXN_56" name="K" />
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="PWMB" name="Q" />
            <blockpin signalname="XLXN_362" name="CLR" />
        </block>
        <block symbolname="copy_of_and2" name="XLXI_113">
            <blockpin signalname="XLXN_376" name="I0" />
            <blockpin signalname="XLXN_377" name="I1" />
            <blockpin signalname="XLXN_375" name="O" />
            <blockpin signalname="CLK_DDS" name="C" />
        </block>
        <block symbolname="vcc" name="XLXI_114">
            <blockpin signalname="XLXN_377" name="P" />
        </block>
        <block symbolname="copy_of_and2" name="XLXI_115">
            <blockpin signalname="XLXN_380" name="I0" />
            <blockpin signalname="XLXN_381" name="I1" />
            <blockpin signalname="XLXN_379" name="O" />
            <blockpin signalname="CLK_DDS" name="C" />
        </block>
        <block symbolname="vcc" name="XLXI_116">
            <blockpin signalname="XLXN_381" name="P" />
        </block>
        <block symbolname="fd16ce" name="XLXI_124">
            <blockpin signalname="CLK_DDS" name="C" />
            <blockpin signalname="XLXN_418" name="CE" />
            <blockpin signalname="RST" name="CLR" />
            <blockpin signalname="XLXN_416(15:0)" name="D(15:0)" />
            <blockpin signalname="XLXN_411(29:14)" name="Q(15:0)" />
        </block>
        <block symbolname="copy_of_and2" name="XLXI_126">
            <blockpin signalname="SUM(31)" name="I0" />
            <blockpin signalname="DIFF(31)" name="I1" />
            <blockpin signalname="XLXN_421" name="O" />
            <blockpin signalname="CLK_DDS" name="C" />
        </block>
        <block symbolname="buf" name="XLXI_128(15:0)">
            <blockpin signalname="XLXN_411(29:14)" name="I" />
            <blockpin signalname="PWMAdvTest(15:0)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_129(1:0)">
            <blockpin signalname="XLXN_434(1:0)" name="I" />
            <blockpin signalname="PWM_Reg_31(1:0)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_130">
            <blockpin signalname="SUM(31)" name="I" />
            <blockpin signalname="XLXN_434(0)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_131">
            <blockpin signalname="DIFF(31)" name="I" />
            <blockpin signalname="XLXN_434(1)" name="O" />
        </block>
        <block symbolname="gnd" name="XLXI_132">
            <blockpin signalname="XLXN_436" name="G" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="5440" height="3520">
        <iomarker fontsize="28" x="256" y="240" name="PWM_STOP" orien="R180" />
        <branch name="PWM_STOP">
            <wire x2="1104" y1="240" y2="240" x1="256" />
            <wire x2="1104" y1="192" y2="240" x1="1104" />
            <wire x2="1936" y1="192" y2="192" x1="1104" />
        </branch>
        <branch name="XLXN_48">
            <wire x2="4224" y1="1200" y2="1200" x1="3904" />
            <wire x2="4224" y1="1200" y2="1232" x1="4224" />
            <wire x2="4240" y1="1232" y2="1232" x1="4224" />
        </branch>
        <branch name="XLXN_49">
            <wire x2="3888" y1="1296" y2="1408" x1="3888" />
            <wire x2="4240" y1="1296" y2="1296" x1="3888" />
        </branch>
        <branch name="XLXN_55">
            <wire x2="4240" y1="1664" y2="1664" x1="3840" />
            <wire x2="4240" y1="1664" y2="1712" x1="4240" />
            <wire x2="4256" y1="1712" y2="1712" x1="4240" />
        </branch>
        <branch name="XLXN_56">
            <wire x2="4240" y1="1856" y2="1856" x1="3840" />
            <wire x2="4256" y1="1776" y2="1776" x1="4240" />
            <wire x2="4240" y1="1776" y2="1856" x1="4240" />
        </branch>
        <instance x="3664" y="448" name="XLXI_7" orien="R0" />
        <instance x="3680" y="272" name="XLXI_6" orien="R0" />
        <branch name="XLXN_24">
            <wire x2="2880" y1="224" y2="224" x1="2192" />
            <wire x2="2880" y1="224" y2="304" x1="2880" />
            <wire x2="3568" y1="304" y2="304" x1="2880" />
            <wire x2="3568" y1="304" y2="384" x1="3568" />
            <wire x2="3664" y1="384" y2="384" x1="3568" />
            <wire x2="2992" y1="224" y2="224" x1="2880" />
            <wire x2="3680" y1="144" y2="144" x1="3568" />
            <wire x2="3568" y1="144" y2="304" x1="3568" />
        </branch>
        <instance x="3664" y="752" name="XLXI_8" orien="R0" />
        <branch name="XLXN_28">
            <wire x2="3504" y1="192" y2="192" x1="3248" />
            <wire x2="3504" y1="192" y2="528" x1="3504" />
            <wire x2="3664" y1="528" y2="528" x1="3504" />
            <wire x2="3664" y1="528" y2="560" x1="3664" />
            <wire x2="3504" y1="528" y2="800" x1="3504" />
            <wire x2="3664" y1="800" y2="800" x1="3504" />
        </branch>
        <instance x="3664" y="928" name="XLXI_9" orien="R0" />
        <instance x="3008" y="464" name="XLXI_4" orien="R0" />
        <instance x="2992" y="96" name="XLXI_3" orien="M180" />
        <instance x="1936" y="320" name="XLXI_2" orien="R0" />
        <instance x="3008" y="592" name="XLXI_5" orien="R0" />
        <branch name="PWM_LVL_RST_STOP">
            <wire x2="2928" y1="160" y2="160" x1="400" />
            <wire x2="2992" y1="160" y2="160" x1="2928" />
            <wire x2="2928" y1="160" y2="384" x1="2928" />
            <wire x2="2928" y1="384" y2="400" x1="2928" />
            <wire x2="3008" y1="400" y2="400" x1="2928" />
            <wire x2="2992" y1="384" y2="384" x1="2928" />
            <wire x2="2992" y1="384" y2="528" x1="2992" />
            <wire x2="3008" y1="528" y2="528" x1="2992" />
            <wire x2="2928" y1="80" y2="160" x1="2928" />
            <wire x2="3600" y1="80" y2="80" x1="2928" />
            <wire x2="3680" y1="80" y2="80" x1="3600" />
            <wire x2="3600" y1="80" y2="256" x1="3600" />
            <wire x2="3664" y1="256" y2="256" x1="3600" />
            <wire x2="3600" y1="256" y2="624" x1="3600" />
            <wire x2="3664" y1="624" y2="624" x1="3600" />
            <wire x2="3600" y1="624" y2="736" x1="3600" />
            <wire x2="3664" y1="736" y2="736" x1="3600" />
        </branch>
        <instance x="3632" y="1280" name="XLXI_18" orien="M180" />
        <instance x="3648" y="1328" name="XLXI_17" orien="R0" />
        <branch name="SUM(31)">
            <attrtext style="alignment:SOFT-LEFT" attrname="Name" x="2741" y="1136" type="branch" />
            <wire x2="2464" y1="1136" y2="1216" x1="2464" />
            <wire x2="3008" y1="1136" y2="1136" x1="2464" />
            <wire x2="3616" y1="1136" y2="1136" x1="3008" />
            <wire x2="3648" y1="1136" y2="1136" x1="3616" />
            <wire x2="3616" y1="1136" y2="1344" x1="3616" />
            <wire x2="3632" y1="1344" y2="1344" x1="3616" />
            <wire x2="3008" y1="1136" y2="1808" x1="3008" />
            <wire x2="3008" y1="1808" y2="1808" x1="2992" />
        </branch>
        <instance x="3584" y="1728" name="XLXI_23" orien="M180" />
        <instance x="3584" y="1792" name="XLXI_19" orien="R0" />
        <branch name="DIFF(31)">
            <attrtext style="alignment:SOFT-LEFT" attrname="Name" x="2727" y="1600" type="branch" />
            <wire x2="2432" y1="1600" y2="1680" x1="2432" />
            <wire x2="3152" y1="1600" y2="1600" x1="2432" />
            <wire x2="3536" y1="1600" y2="1600" x1="3152" />
            <wire x2="3536" y1="1600" y2="1792" x1="3536" />
            <wire x2="3584" y1="1792" y2="1792" x1="3536" />
            <wire x2="3584" y1="1600" y2="1600" x1="3536" />
            <wire x2="3152" y1="1600" y2="1872" x1="3152" />
            <wire x2="3152" y1="1872" y2="1872" x1="2992" />
        </branch>
        <iomarker fontsize="28" x="400" y="160" name="PWM_LVL_RST_STOP" orien="R180" />
        <instance x="3120" y="2352" name="XLXI_28" orien="R0" />
        <branch name="XLXN_163">
            <wire x2="3552" y1="2256" y2="2256" x1="3376" />
        </branch>
        <branch name="F">
            <wire x2="4688" y1="2256" y2="2256" x1="4544" />
        </branch>
        <iomarker fontsize="28" x="4688" y="2256" name="F" orien="R0" />
        <instance x="4096" y="2832" name="XLXI_31" orien="R0" />
        <instance x="4112" y="2848" name="XLXI_32" orien="M180" />
        <branch name="XLXN_175">
            <wire x2="4560" y1="2704" y2="2704" x1="4352" />
        </branch>
        <instance x="4560" y="3024" name="XLXI_30" orien="R0" />
        <branch name="XLXN_178">
            <wire x2="4464" y1="2976" y2="2976" x1="4368" />
            <wire x2="4464" y1="2768" y2="2976" x1="4464" />
            <wire x2="4560" y1="2768" y2="2768" x1="4464" />
        </branch>
        <branch name="XLXN_182">
            <wire x2="3952" y1="2624" y2="2640" x1="3952" />
            <wire x2="3968" y1="2640" y2="2640" x1="3952" />
            <wire x2="3968" y1="2640" y2="2768" x1="3968" />
            <wire x2="3968" y1="2768" y2="2912" x1="3968" />
            <wire x2="4112" y1="2912" y2="2912" x1="3968" />
            <wire x2="4096" y1="2624" y2="2624" x1="3952" />
            <wire x2="4096" y1="2624" y2="2640" x1="4096" />
            <wire x2="3968" y1="2768" y2="2768" x1="3952" />
        </branch>
        <branch name="XLXN_183">
            <wire x2="4000" y1="2256" y2="2256" x1="3936" />
            <wire x2="4160" y1="2256" y2="2256" x1="4000" />
            <wire x2="4000" y1="2256" y2="2704" x1="4000" />
            <wire x2="4000" y1="2704" y2="2976" x1="4000" />
            <wire x2="4112" y1="2976" y2="2976" x1="4000" />
            <wire x2="4096" y1="2704" y2="2704" x1="4000" />
        </branch>
        <branch name="FMOT">
            <wire x2="3584" y1="1552" y2="1552" x1="3504" />
            <wire x2="3584" y1="1552" y2="1664" x1="3584" />
            <wire x2="3504" y1="1552" y2="1856" x1="3504" />
            <wire x2="3584" y1="1856" y2="1856" x1="3504" />
            <wire x2="3504" y1="1856" y2="2032" x1="3504" />
            <wire x2="4048" y1="2032" y2="2032" x1="3504" />
            <wire x2="4048" y1="2032" y2="2768" x1="4048" />
            <wire x2="4080" y1="2768" y2="2768" x1="4048" />
            <wire x2="4096" y1="2768" y2="2768" x1="4080" />
            <wire x2="4080" y1="2768" y2="3040" x1="4080" />
            <wire x2="4112" y1="3040" y2="3040" x1="4080" />
            <wire x2="4080" y1="3040" y2="3120" x1="4080" />
            <wire x2="4992" y1="3120" y2="3120" x1="4080" />
            <wire x2="3584" y1="848" y2="1200" x1="3584" />
            <wire x2="3648" y1="1200" y2="1200" x1="3584" />
            <wire x2="3584" y1="1200" y2="1408" x1="3584" />
            <wire x2="3632" y1="1408" y2="1408" x1="3584" />
            <wire x2="3584" y1="1408" y2="1552" x1="3584" />
            <wire x2="3616" y1="848" y2="848" x1="3584" />
            <wire x2="3616" y1="848" y2="864" x1="3616" />
            <wire x2="3664" y1="864" y2="864" x1="3616" />
            <wire x2="3616" y1="208" y2="320" x1="3616" />
            <wire x2="3664" y1="320" y2="320" x1="3616" />
            <wire x2="3616" y1="320" y2="688" x1="3616" />
            <wire x2="3664" y1="688" y2="688" x1="3616" />
            <wire x2="3616" y1="688" y2="848" x1="3616" />
            <wire x2="3680" y1="208" y2="208" x1="3616" />
            <wire x2="4992" y1="2768" y2="2768" x1="4944" />
            <wire x2="5072" y1="2768" y2="2768" x1="4992" />
            <wire x2="4992" y1="2768" y2="3120" x1="4992" />
        </branch>
        <iomarker fontsize="28" x="5072" y="2768" name="FMOT" orien="R0" />
        <instance x="2240" y="1136" name="XLXI_47" orien="R0" />
        <instance x="2352" y="1488" name="XLXI_48" orien="R180" />
        <branch name="XLXN_193">
            <wire x2="2288" y1="1616" y2="1648" x1="2288" />
        </branch>
        <branch name="XLXN_194">
            <wire x2="2304" y1="1136" y2="1184" x1="2304" />
        </branch>
        <iomarker fontsize="28" x="368" y="1072" name="Phase_Acc_Val(31:0)" orien="R180" />
        <instance x="352" y="320" name="XLXI_49" orien="R0" />
        <branch name="RST_INV">
            <wire x2="352" y1="288" y2="288" x1="240" />
        </branch>
        <iomarker fontsize="28" x="240" y="288" name="RST_INV" orien="R180" />
        <branch name="RST">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="784" y="2064" type="branch" />
            <wire x2="800" y1="2064" y2="2064" x1="784" />
            <wire x2="800" y1="2064" y2="2080" x1="800" />
            <wire x2="864" y1="2080" y2="2080" x1="800" />
        </branch>
        <iomarker fontsize="28" x="272" y="1984" name="Pwm_Val(15:0)" orien="R180" />
        <branch name="Pwm_Val(15:0)">
            <wire x2="416" y1="1984" y2="1984" x1="272" />
            <wire x2="864" y1="1856" y2="1856" x1="416" />
            <wire x2="416" y1="1856" y2="1984" x1="416" />
        </branch>
        <instance x="4160" y="2512" name="XLXI_58" orien="R0" />
        <instance x="3568" y="3024" name="XLXI_60" orien="R0" />
        <branch name="SUM(31:0)">
            <attrtext style="alignment:SOFT-LEFT" attrname="Name" x="2496" y="1312" type="branch" />
            <wire x2="2464" y1="1312" y2="1312" x1="2416" />
            <wire x2="2496" y1="1312" y2="1312" x1="2464" />
        </branch>
        <bustap x2="2464" y1="1312" y2="1216" x1="2464" />
        <branch name="DIFF(31:0)">
            <attrtext style="alignment:SOFT-LEFT" attrname="Name" x="2464" y="1776" type="branch" />
            <wire x2="2432" y1="1776" y2="1776" x1="2400" />
            <wire x2="2464" y1="1776" y2="1776" x1="2432" />
        </branch>
        <bustap x2="2432" y1="1776" y2="1680" x1="2432" />
        <branch name="XLXN_228(31:0)">
            <wire x2="2672" y1="3024" y2="3024" x1="2464" />
        </branch>
        <branch name="XLXN_231">
            <wire x2="2064" y1="2320" y2="2320" x1="1984" />
            <wire x2="2064" y1="2304" y2="2320" x1="2064" />
            <wire x2="2080" y1="2304" y2="2304" x1="2064" />
        </branch>
        <branch name="XLXN_230">
            <wire x2="1840" y1="2880" y2="2880" x1="1792" />
        </branch>
        <instance x="1840" y="2816" name="XLXI_68" orien="R90" />
        <instance x="1312" y="2864" name="XLXI_70" orien="R180">
        </instance>
        <branch name="XLXN_249">
            <wire x2="1040" y1="2832" y2="2960" x1="1040" />
            <wire x2="1056" y1="2960" y2="2960" x1="1040" />
            <wire x2="1328" y1="2832" y2="2832" x1="1040" />
            <wire x2="1328" y1="2672" y2="2672" x1="1280" />
            <wire x2="1328" y1="2672" y2="2832" x1="1328" />
        </branch>
        <branch name="XLXN_251">
            <wire x2="400" y1="2208" y2="2208" x1="352" />
            <wire x2="400" y1="2208" y2="2240" x1="400" />
        </branch>
        <instance x="224" y="2144" name="XLXI_75" orien="R90" />
        <instance x="128" y="3056" name="XLXI_77" orien="R0" />
        <instance x="128" y="2624" name="XLXI_76" orien="R0" />
        <iomarker fontsize="28" x="304" y="944" name="CLK_DDS" orien="R180" />
        <iomarker fontsize="28" x="320" y="1536" name="CLK_8" orien="R180" />
        <branch name="CLK_8">
            <wire x2="768" y1="1536" y2="1536" x1="320" />
            <wire x2="768" y1="1536" y2="1984" x1="768" />
            <wire x2="864" y1="1984" y2="1984" x1="768" />
        </branch>
        <branch name="CLK_8">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="368" y="2336" type="branch" />
            <wire x2="400" y1="2336" y2="2336" x1="368" />
        </branch>
        <instance x="400" y="2208" name="XLXI_74" orien="M180" />
        <branch name="XLXN_262">
            <wire x2="192" y1="2624" y2="2656" x1="192" />
            <wire x2="304" y1="2656" y2="2656" x1="192" />
            <wire x2="304" y1="2656" y2="2848" x1="304" />
            <wire x2="400" y1="2848" y2="2848" x1="304" />
            <wire x2="400" y1="2400" y2="2400" x1="304" />
            <wire x2="304" y1="2400" y2="2656" x1="304" />
        </branch>
        <branch name="XLXN_257">
            <wire x2="192" y1="2720" y2="2784" x1="192" />
            <wire x2="192" y1="2784" y2="2912" x1="192" />
            <wire x2="192" y1="2912" y2="2928" x1="192" />
            <wire x2="400" y1="2912" y2="2912" x1="192" />
            <wire x2="400" y1="2784" y2="2784" x1="192" />
            <wire x2="336" y1="2720" y2="2720" x1="192" />
            <wire x2="400" y1="2720" y2="2720" x1="336" />
            <wire x2="400" y1="2656" y2="2656" x1="336" />
            <wire x2="336" y1="2656" y2="2720" x1="336" />
        </branch>
        <instance x="1552" y="2288" name="XLXI_71" orien="R180">
        </instance>
        <branch name="XLXN_248">
            <wire x2="1328" y1="2608" y2="2608" x1="1280" />
            <wire x2="1296" y1="2384" y2="2528" x1="1296" />
            <wire x2="1328" y1="2528" y2="2528" x1="1296" />
            <wire x2="1328" y1="2528" y2="2608" x1="1328" />
        </branch>
        <branch name="XLXN_242">
            <wire x2="1360" y1="2992" y2="2992" x1="1312" />
            <wire x2="1360" y1="2992" y2="3008" x1="1360" />
            <wire x2="1408" y1="3008" y2="3008" x1="1360" />
        </branch>
        <branch name="XLXN_241">
            <wire x2="1360" y1="2928" y2="2928" x1="1312" />
            <wire x2="1360" y1="2880" y2="2928" x1="1360" />
            <wire x2="1408" y1="2880" y2="2880" x1="1360" />
        </branch>
        <instance x="1792" y="2688" name="XLXI_66" orien="R180" />
        <branch name="XLXN_245">
            <wire x2="1584" y1="2416" y2="2416" x1="1552" />
            <wire x2="1584" y1="2416" y2="2448" x1="1584" />
            <wire x2="1600" y1="2448" y2="2448" x1="1584" />
        </branch>
        <branch name="XLXN_244">
            <wire x2="1584" y1="2352" y2="2352" x1="1552" />
            <wire x2="1584" y1="2320" y2="2352" x1="1584" />
            <wire x2="1600" y1="2320" y2="2320" x1="1584" />
        </branch>
        <instance x="1984" y="2128" name="XLXI_65" orien="R180" />
        <instance x="2080" y="2240" name="XLXI_67" orien="R90" />
        <branch name="XLXN_227(31:0)">
            <wire x2="2640" y1="2400" y2="2400" x1="2480" />
        </branch>
        <instance x="2784" y="2432" name="XLXI_64" orien="R180">
        </instance>
        <instance x="2480" y="2368" name="XLXI_61" orien="R180">
        </instance>
        <branch name="XLXN_280">
            <wire x2="848" y1="2848" y2="2848" x1="784" />
            <wire x2="848" y1="2848" y2="2944" x1="848" />
            <wire x2="816" y1="2944" y2="3056" x1="816" />
            <wire x2="848" y1="2944" y2="2944" x1="816" />
            <wire x2="864" y1="1920" y2="1920" x1="848" />
            <wire x2="848" y1="1920" y2="2848" x1="848" />
        </branch>
        <instance x="864" y="2112" name="XLXI_46" orien="R0" />
        <branch name="XLXN_286">
            <wire x2="880" y1="2784" y2="2784" x1="784" />
            <wire x2="880" y1="2784" y2="3056" x1="880" />
        </branch>
        <branch name="XLXN_289">
            <wire x2="336" y1="3312" y2="3392" x1="336" />
            <wire x2="1040" y1="3392" y2="3392" x1="336" />
            <wire x2="800" y1="2656" y2="2656" x1="784" />
            <wire x2="976" y1="2656" y2="2656" x1="800" />
            <wire x2="976" y1="2656" y2="3008" x1="976" />
            <wire x2="1008" y1="3008" y2="3008" x1="976" />
            <wire x2="1008" y1="3008" y2="3056" x1="1008" />
            <wire x2="1040" y1="3008" y2="3008" x1="1008" />
            <wire x2="1040" y1="3008" y2="3392" x1="1040" />
            <wire x2="880" y1="2160" y2="2160" x1="800" />
            <wire x2="800" y1="2160" y2="2656" x1="800" />
        </branch>
        <instance x="1280" y="2544" name="XLXI_73" orien="R180">
        </instance>
        <instance x="880" y="2128" name="XLXI_80" orien="M180" />
        <branch name="XLXN_293">
            <wire x2="880" y1="2720" y2="2720" x1="784" />
            <wire x2="944" y1="2720" y2="2720" x1="880" />
            <wire x2="944" y1="2720" y2="3056" x1="944" />
            <wire x2="880" y1="2448" y2="2720" x1="880" />
        </branch>
        <instance x="704" y="2240" name="XLXI_81" orien="R270" />
        <branch name="XLXN_294">
            <wire x2="784" y1="2176" y2="2176" x1="704" />
            <wire x2="784" y1="2176" y2="2320" x1="784" />
            <wire x2="880" y1="2320" y2="2320" x1="784" />
        </branch>
        <instance x="464" y="3312" name="XLXI_82" orien="R270" />
        <branch name="RST">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="176" y="3312" type="branch" />
            <wire x2="272" y1="3312" y2="3312" x1="176" />
        </branch>
        <instance x="752" y="3056" name="XLXI_78" orien="R90" />
        <branch name="XLXN_302">
            <wire x2="400" y1="3312" y2="3328" x1="400" />
            <wire x2="912" y1="3328" y2="3328" x1="400" />
            <wire x2="912" y1="3312" y2="3328" x1="912" />
        </branch>
        <branch name="XLXN_311">
            <wire x2="336" y1="2992" y2="3056" x1="336" />
            <wire x2="384" y1="2992" y2="2992" x1="336" />
            <wire x2="384" y1="2528" y2="2992" x1="384" />
            <wire x2="400" y1="2528" y2="2528" x1="384" />
        </branch>
        <instance x="1584" y="2192" name="XLXI_86" orien="R270">
        </instance>
        <bustap x2="2624" y1="2000" y2="2000" x1="2528" />
        <bustap x2="2624" y1="2048" y2="2048" x1="2528" />
        <branch name="Phase_Acc_Val(30)">
            <attrtext style="alignment:SOFT-LEFT" attrname="Name" x="2858" y="2000" type="branch" />
            <wire x2="3120" y1="2000" y2="2000" x1="2624" />
            <wire x2="3120" y1="2000" y2="2224" x1="3120" />
        </branch>
        <branch name="Phase_Acc_Val(31)">
            <attrtext style="alignment:SOFT-BCENTER" attrname="Name" x="2848" y="2048" type="branch" />
            <wire x2="2848" y1="2048" y2="2048" x1="2624" />
            <wire x2="3072" y1="2048" y2="2048" x1="2848" />
            <wire x2="3072" y1="2048" y2="2288" x1="3072" />
            <wire x2="3120" y1="2288" y2="2288" x1="3072" />
            <wire x2="3072" y1="2288" y2="2768" x1="3072" />
            <wire x2="3568" y1="2768" y2="2768" x1="3072" />
        </branch>
        <branch name="XLXN_35">
            <wire x2="3936" y1="624" y2="624" x1="3920" />
            <wire x2="3936" y1="624" y2="688" x1="3936" />
            <wire x2="4208" y1="688" y2="688" x1="3936" />
        </branch>
        <branch name="XLXN_36">
            <wire x2="3936" y1="800" y2="800" x1="3920" />
            <wire x2="4208" y1="752" y2="752" x1="3936" />
            <wire x2="3936" y1="752" y2="800" x1="3936" />
        </branch>
        <iomarker fontsize="28" x="5152" y="1168" name="PWMA" orien="R0" />
        <iomarker fontsize="28" x="5152" y="1648" name="PWMB" orien="R0" />
        <branch name="Phase_Acc_Val(31:0)">
            <attrtext style="alignment:SOFT-BCENTER" attrname="Name" x="2400" y="1936" type="branch" />
            <wire x2="2064" y1="1072" y2="1072" x1="368" />
            <wire x2="2064" y1="1072" y2="1248" x1="2064" />
            <wire x2="2080" y1="1248" y2="1248" x1="2064" />
            <wire x2="2048" y1="1248" y2="1712" x1="2048" />
            <wire x2="2064" y1="1712" y2="1712" x1="2048" />
            <wire x2="2048" y1="1712" y2="1936" x1="2048" />
            <wire x2="2400" y1="1936" y2="1936" x1="2048" />
            <wire x2="2528" y1="1936" y2="1936" x1="2400" />
            <wire x2="2528" y1="1936" y2="2000" x1="2528" />
            <wire x2="2528" y1="2000" y2="2048" x1="2528" />
            <wire x2="2528" y1="2048" y2="2464" x1="2528" />
            <wire x2="2528" y1="2464" y2="3088" x1="2528" />
            <wire x2="2064" y1="1248" y2="1248" x1="2048" />
            <wire x2="2528" y1="3088" y2="3088" x1="2464" />
            <wire x2="2528" y1="2464" y2="2464" x1="2480" />
        </branch>
        <instance x="2080" y="1424" name="XLXI_105" orien="R0">
        </instance>
        <instance x="2064" y="1888" name="XLXI_106" orien="R0">
        </instance>
        <branch name="XLXN_34">
            <wire x2="3936" y1="320" y2="320" x1="3920" />
            <wire x2="3936" y1="272" y2="320" x1="3936" />
            <wire x2="4224" y1="272" y2="272" x1="3936" />
        </branch>
        <branch name="XLXN_33">
            <wire x2="3952" y1="144" y2="144" x1="3936" />
            <wire x2="3952" y1="144" y2="208" x1="3952" />
            <wire x2="4224" y1="208" y2="208" x1="3952" />
        </branch>
        <instance x="4224" y="432" name="XLXI_109" orien="R0">
        </instance>
        <instance x="4208" y="912" name="XLXI_110" orien="R0">
        </instance>
        <branch name="XLXN_357">
            <wire x2="3312" y1="368" y2="368" x1="3264" />
            <wire x2="3312" y1="16" y2="368" x1="3312" />
            <wire x2="4016" y1="16" y2="16" x1="3312" />
            <wire x2="4016" y1="16" y2="128" x1="4016" />
            <wire x2="4032" y1="128" y2="128" x1="4016" />
            <wire x2="4032" y1="128" y2="144" x1="4032" />
            <wire x2="4224" y1="144" y2="144" x1="4032" />
            <wire x2="4032" y1="144" y2="992" x1="4032" />
            <wire x2="4144" y1="992" y2="992" x1="4032" />
            <wire x2="4208" y1="880" y2="880" x1="4144" />
            <wire x2="4144" y1="880" y2="992" x1="4144" />
        </branch>
        <branch name="XLXN_350">
            <wire x2="4112" y1="496" y2="496" x1="3264" />
            <wire x2="4176" y1="496" y2="496" x1="4112" />
            <wire x2="4112" y1="496" y2="592" x1="4112" />
            <wire x2="4192" y1="592" y2="592" x1="4112" />
            <wire x2="4192" y1="592" y2="624" x1="4192" />
            <wire x2="4208" y1="624" y2="624" x1="4192" />
            <wire x2="4224" y1="400" y2="400" x1="4176" />
            <wire x2="4176" y1="400" y2="496" x1="4176" />
        </branch>
        <instance x="4240" y="1456" name="XLXI_111" orien="R0">
        </instance>
        <branch name="PWMA">
            <wire x2="3648" y1="1264" y2="1264" x1="3552" />
            <wire x2="3552" y1="1264" y2="1472" x1="3552" />
            <wire x2="3632" y1="1472" y2="1472" x1="3552" />
            <wire x2="3552" y1="1472" y2="1536" x1="3552" />
            <wire x2="4992" y1="1536" y2="1536" x1="3552" />
            <wire x2="4992" y1="1168" y2="1168" x1="4624" />
            <wire x2="5152" y1="1168" y2="1168" x1="4992" />
            <wire x2="4992" y1="1168" y2="1536" x1="4992" />
        </branch>
        <instance x="4256" y="1936" name="XLXI_112" orien="R0">
        </instance>
        <branch name="XLXN_362">
            <wire x2="4656" y1="1088" y2="1088" x1="3920" />
            <wire x2="3920" y1="1088" y2="1424" x1="3920" />
            <wire x2="4240" y1="1424" y2="1424" x1="3920" />
            <wire x2="3920" y1="1424" y2="1904" x1="3920" />
            <wire x2="4256" y1="1904" y2="1904" x1="3920" />
            <wire x2="4880" y1="624" y2="624" x1="4592" />
            <wire x2="4880" y1="624" y2="784" x1="4880" />
            <wire x2="4944" y1="784" y2="784" x1="4880" />
            <wire x2="4944" y1="784" y2="960" x1="4944" />
            <wire x2="4944" y1="960" y2="960" x1="4656" />
            <wire x2="4656" y1="960" y2="1088" x1="4656" />
        </branch>
        <branch name="PWMB">
            <wire x2="3584" y1="1728" y2="1728" x1="3488" />
            <wire x2="3488" y1="1728" y2="1920" x1="3488" />
            <wire x2="3584" y1="1920" y2="1920" x1="3488" />
            <wire x2="3488" y1="1920" y2="2016" x1="3488" />
            <wire x2="5008" y1="2016" y2="2016" x1="3488" />
            <wire x2="5008" y1="1648" y2="1648" x1="4640" />
            <wire x2="5152" y1="1648" y2="1648" x1="5008" />
            <wire x2="5008" y1="1648" y2="2016" x1="5008" />
        </branch>
        <instance x="2336" y="2592" name="XLXI_113" orien="R180">
        </instance>
        <branch name="XLXN_375">
            <wire x2="2032" y1="2448" y2="2448" x1="1984" />
            <wire x2="2032" y1="2448" y2="2688" x1="2032" />
            <wire x2="2080" y1="2688" y2="2688" x1="2032" />
        </branch>
        <branch name="XLXN_376">
            <wire x2="2096" y1="2432" y2="2432" x1="2048" />
            <wire x2="2048" y1="2432" y2="2576" x1="2048" />
            <wire x2="2400" y1="2576" y2="2576" x1="2048" />
            <wire x2="2400" y1="2576" y2="2656" x1="2400" />
            <wire x2="2400" y1="2656" y2="2656" x1="2336" />
        </branch>
        <branch name="CLK_DDS">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="464" y="2128" type="branch" />
            <wire x2="816" y1="2128" y2="2128" x1="464" />
            <wire x2="816" y1="2128" y2="2256" x1="816" />
            <wire x2="880" y1="2256" y2="2256" x1="816" />
            <wire x2="1312" y1="2128" y2="2128" x1="816" />
            <wire x2="1312" y1="2064" y2="2128" x1="1312" />
            <wire x2="1360" y1="2064" y2="2064" x1="1312" />
        </branch>
        <branch name="XLXN_377">
            <wire x2="2368" y1="2720" y2="2720" x1="2336" />
        </branch>
        <branch name="CLK_DDS">
            <attrtext style="alignment:SOFT-VRIGHT;fontsize:28;fontname:Arial" attrname="Name" x="2224" y="2832" type="branch" />
            <wire x2="2224" y1="2784" y2="2832" x1="2224" />
        </branch>
        <instance x="2464" y="2992" name="XLXI_62" orien="R180">
        </instance>
        <instance x="2816" y="3056" name="XLXI_63" orien="R180">
        </instance>
        <instance x="2144" y="3200" name="XLXI_115" orien="R180">
        </instance>
        <branch name="XLXN_379">
            <wire x2="1840" y1="3008" y2="3008" x1="1792" />
            <wire x2="1840" y1="3008" y2="3296" x1="1840" />
            <wire x2="1888" y1="3296" y2="3296" x1="1840" />
        </branch>
        <branch name="XLXN_380">
            <wire x2="2080" y1="3056" y2="3056" x1="2000" />
            <wire x2="2000" y1="3056" y2="3184" x1="2000" />
            <wire x2="2208" y1="3184" y2="3184" x1="2000" />
            <wire x2="2208" y1="3184" y2="3264" x1="2208" />
            <wire x2="2208" y1="3264" y2="3264" x1="2144" />
        </branch>
        <branch name="XLXN_381">
            <wire x2="2208" y1="3328" y2="3328" x1="2144" />
        </branch>
        <branch name="CLK_DDS">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2176" y="3456" type="branch" />
            <wire x2="2032" y1="3392" y2="3456" x1="2032" />
            <wire x2="2176" y1="3456" y2="3456" x1="2032" />
        </branch>
        <instance x="2208" y="3264" name="XLXI_116" orien="R90" />
        <instance x="2368" y="2656" name="XLXI_114" orien="R90" />
        <instance x="1040" y="1584" name="XLXI_124" orien="R0" />
        <instance x="1552" y="1120" name="XLXI_51(13:0)" orien="R90" />
        <instance x="1552" y="1184" name="XLXI_52(1:0)" orien="R90" />
        <bustap x2="1792" y1="1248" y2="1248" x1="1888" />
        <bustap x2="1792" y1="1296" y2="1296" x1="1888" />
        <bustap x2="1792" y1="1344" y2="1344" x1="1888" />
        <branch name="XLXN_411(30:31)">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:20;fontname:Arial" attrname="Name" x="1760" y="1296" type="branch" />
            <wire x2="1728" y1="1248" y2="1248" x1="1680" />
            <wire x2="1728" y1="1248" y2="1296" x1="1728" />
            <wire x2="1760" y1="1296" y2="1296" x1="1728" />
            <wire x2="1792" y1="1296" y2="1296" x1="1760" />
        </branch>
        <branch name="XLXN_411(13:0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="1754" y="1248" type="branch" />
            <wire x2="1744" y1="1184" y2="1184" x1="1680" />
            <wire x2="1744" y1="1184" y2="1248" x1="1744" />
            <wire x2="1792" y1="1248" y2="1248" x1="1744" />
        </branch>
        <branch name="RST">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1008" y="1552" type="branch" />
            <wire x2="1040" y1="1552" y2="1552" x1="1008" />
        </branch>
        <branch name="XLXN_416(15:0)">
            <wire x2="880" y1="1328" y2="1696" x1="880" />
            <wire x2="1264" y1="1696" y2="1696" x1="880" />
            <wire x2="1264" y1="1696" y2="1856" x1="1264" />
            <wire x2="1040" y1="1328" y2="1328" x1="880" />
            <wire x2="1264" y1="1856" y2="1856" x1="1248" />
        </branch>
        <branch name="XLXN_418">
            <wire x2="1040" y1="1392" y2="1392" x1="912" />
            <wire x2="912" y1="1392" y2="1648" x1="912" />
            <wire x2="1456" y1="1648" y2="1648" x1="912" />
            <wire x2="1456" y1="1648" y2="1936" x1="1456" />
        </branch>
        <instance x="3552" y="2512" name="XLXI_57" orien="R0" />
        <branch name="CLK_DDS">
            <attrtext style="alignment:SOFT-LEFT;fontsize:28;fontname:Arial" attrname="Name" x="2032" y="2256" type="branch" />
            <wire x2="1168" y1="2720" y2="2768" x1="1168" />
            <wire x2="1344" y1="2768" y2="2768" x1="1168" />
            <wire x2="1200" y1="3040" y2="3168" x1="1200" />
            <wire x2="1936" y1="3168" y2="3168" x1="1200" />
            <wire x2="1344" y1="2656" y2="2768" x1="1344" />
            <wire x2="1584" y1="2656" y2="2656" x1="1344" />
            <wire x2="1440" y1="2464" y2="2512" x1="1440" />
            <wire x2="1584" y1="2512" y2="2512" x1="1440" />
            <wire x2="1584" y1="2512" y2="2560" x1="1584" />
            <wire x2="2000" y1="2560" y2="2560" x1="1584" />
            <wire x2="2000" y1="2560" y2="2816" x1="2000" />
            <wire x2="2000" y1="2816" y2="2960" x1="2000" />
            <wire x2="1584" y1="2560" y2="2656" x1="1584" />
            <wire x2="2000" y1="2816" y2="2816" x1="1792" />
            <wire x2="1936" y1="2960" y2="3168" x1="1936" />
            <wire x2="2000" y1="2960" y2="2960" x1="1936" />
            <wire x2="2000" y1="2256" y2="2256" x1="1984" />
            <wire x2="2032" y1="2256" y2="2256" x1="2000" />
            <wire x2="2000" y1="2256" y2="2560" x1="2000" />
        </branch>
        <branch name="XLXN_411(31:0)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="1985" y="1376" type="branch" />
            <wire x2="1888" y1="1232" y2="1248" x1="1888" />
            <wire x2="1888" y1="1248" y2="1296" x1="1888" />
            <wire x2="1888" y1="1296" y2="1344" x1="1888" />
            <wire x2="1888" y1="1344" y2="1376" x1="1888" />
            <wire x2="1952" y1="1376" y2="1376" x1="1888" />
            <wire x2="1952" y1="1376" y2="1840" x1="1952" />
            <wire x2="2064" y1="1840" y2="1840" x1="1952" />
            <wire x2="2080" y1="1376" y2="1376" x1="1952" />
        </branch>
        <instance x="2992" y="1744" name="XLXI_126" orien="R180">
        </instance>
        <branch name="XLXN_421">
            <wire x2="1456" y1="2192" y2="2256" x1="1456" />
            <wire x2="1568" y1="2256" y2="2256" x1="1456" />
            <wire x2="2400" y1="2048" y2="2048" x1="1568" />
            <wire x2="2400" y1="2048" y2="2064" x1="2400" />
            <wire x2="2432" y1="2064" y2="2064" x1="2400" />
            <wire x2="1568" y1="2048" y2="2256" x1="1568" />
            <wire x2="2576" y1="1856" y2="1856" x1="2432" />
            <wire x2="2432" y1="1856" y2="2064" x1="2432" />
            <wire x2="2736" y1="1840" y2="1840" x1="2576" />
            <wire x2="2576" y1="1840" y2="1856" x1="2576" />
        </branch>
        <line x2="1448" y1="2176" y2="2176" x1="1452" />
        <branch name="RST">
            <attrtext style="alignment:SOFT-BCENTER;fontsize:28;fontname:Arial" attrname="Name" x="768" y="288" type="branch" />
            <wire x2="768" y1="288" y2="288" x1="576" />
            <wire x2="1216" y1="288" y2="288" x1="768" />
            <wire x2="1216" y1="288" y2="528" x1="1216" />
            <wire x2="2880" y1="528" y2="528" x1="1216" />
            <wire x2="2944" y1="528" y2="528" x1="2880" />
            <wire x2="2880" y1="528" y2="784" x1="2880" />
            <wire x2="3392" y1="784" y2="784" x1="2880" />
            <wire x2="3392" y1="784" y2="2480" x1="3392" />
            <wire x2="3536" y1="2480" y2="2480" x1="3392" />
            <wire x2="3536" y1="2480" y2="2496" x1="3536" />
            <wire x2="4160" y1="2496" y2="2496" x1="3536" />
            <wire x2="3552" y1="2480" y2="2480" x1="3536" />
            <wire x2="3392" y1="2480" y2="2624" x1="3392" />
            <wire x2="3392" y1="2624" y2="2992" x1="3392" />
            <wire x2="3392" y1="2992" y2="3088" x1="3392" />
            <wire x2="4544" y1="3088" y2="3088" x1="3392" />
            <wire x2="3568" y1="2992" y2="2992" x1="3392" />
            <wire x2="1936" y1="256" y2="256" x1="1216" />
            <wire x2="1216" y1="256" y2="288" x1="1216" />
            <wire x2="1792" y1="2624" y2="2720" x1="1792" />
            <wire x2="2912" y1="2624" y2="2624" x1="1792" />
            <wire x2="3392" y1="2624" y2="2624" x1="2912" />
            <wire x2="2912" y1="2160" y2="2160" x1="1984" />
            <wire x2="2912" y1="2160" y2="2624" x1="2912" />
            <wire x2="3008" y1="336" y2="336" x1="2880" />
            <wire x2="2880" y1="336" y2="528" x1="2880" />
            <wire x2="3008" y1="464" y2="464" x1="2944" />
            <wire x2="2944" y1="464" y2="528" x1="2944" />
            <wire x2="4160" y1="2480" y2="2496" x1="4160" />
            <wire x2="4560" y1="2992" y2="2992" x1="4544" />
            <wire x2="4544" y1="2992" y2="3088" x1="4544" />
        </branch>
        <iomarker fontsize="28" x="4288" y="3296" name="PWMAdvTest(15:0)" orien="R0" />
        <branch name="PWMAdvTest(15:0)">
            <wire x2="1664" y1="1776" y2="2096" x1="1664" />
            <wire x2="2992" y1="2096" y2="2096" x1="1664" />
            <wire x2="2992" y1="2096" y2="3120" x1="2992" />
            <wire x2="3552" y1="3120" y2="3120" x1="2992" />
            <wire x2="3552" y1="3120" y2="3216" x1="3552" />
            <wire x2="4128" y1="3216" y2="3216" x1="3552" />
            <wire x2="4128" y1="3216" y2="3296" x1="4128" />
            <wire x2="4288" y1="3296" y2="3296" x1="4128" />
        </branch>
        <branch name="XLXN_411(29:14)">
            <attrtext style="alignment:SOFT-LEFT;fontsize:20;fontname:Arial" attrname="Name" x="1588" y="1328" type="branch" />
            <wire x2="1584" y1="1328" y2="1328" x1="1424" />
            <wire x2="1600" y1="1328" y2="1328" x1="1584" />
            <wire x2="1600" y1="1328" y2="1344" x1="1600" />
            <wire x2="1664" y1="1344" y2="1344" x1="1600" />
            <wire x2="1792" y1="1344" y2="1344" x1="1664" />
            <wire x2="1664" y1="1344" y2="1552" x1="1664" />
        </branch>
        <instance x="1632" y="1552" name="XLXI_128(15:0)" orien="R90" />
        <branch name="XLXN_432">
            <wire x2="1520" y1="2320" y2="2320" x1="1264" />
            <wire x2="1520" y1="2192" y2="2320" x1="1520" />
        </branch>
        <branch name="XLXN_433">
            <wire x2="1280" y1="2448" y2="2448" x1="1264" />
            <wire x2="1280" y1="2256" y2="2448" x1="1280" />
            <wire x2="1392" y1="2256" y2="2256" x1="1280" />
            <wire x2="1392" y1="2192" y2="2256" x1="1392" />
        </branch>
        <branch name="CLK_DDS">
            <wire x2="976" y1="944" y2="944" x1="304" />
            <wire x2="976" y1="944" y2="1456" x1="976" />
            <wire x2="1040" y1="1456" y2="1456" x1="976" />
            <wire x2="2016" y1="944" y2="944" x1="976" />
            <wire x2="2016" y1="944" y2="1536" x1="2016" />
            <wire x2="2176" y1="1536" y2="1536" x1="2016" />
            <wire x2="2176" y1="1536" y2="1648" x1="2176" />
            <wire x2="2192" y1="944" y2="944" x1="2016" />
            <wire x2="2192" y1="944" y2="1184" x1="2192" />
            <wire x2="4080" y1="944" y2="944" x1="2192" />
            <wire x2="4080" y1="944" y2="1360" x1="4080" />
            <wire x2="4240" y1="1360" y2="1360" x1="4080" />
            <wire x2="4080" y1="1360" y2="1840" x1="4080" />
            <wire x2="4096" y1="1840" y2="1840" x1="4080" />
            <wire x2="4256" y1="1840" y2="1840" x1="4096" />
            <wire x2="4096" y1="1840" y2="2112" x1="4096" />
            <wire x2="4096" y1="2112" y2="2384" x1="4096" />
            <wire x2="4112" y1="2384" y2="2384" x1="4096" />
            <wire x2="4160" y1="2384" y2="2384" x1="4112" />
            <wire x2="4112" y1="2384" y2="2576" x1="4112" />
            <wire x2="4432" y1="2576" y2="2576" x1="4112" />
            <wire x2="4432" y1="2576" y2="2896" x1="4432" />
            <wire x2="4560" y1="2896" y2="2896" x1="4432" />
            <wire x2="2624" y1="1488" y2="1488" x1="2176" />
            <wire x2="2624" y1="1488" y2="1952" x1="2624" />
            <wire x2="2880" y1="1952" y2="1952" x1="2624" />
            <wire x2="2176" y1="1488" y2="1536" x1="2176" />
            <wire x2="2880" y1="1936" y2="1952" x1="2880" />
            <wire x2="3536" y1="2384" y2="2384" x1="3520" />
            <wire x2="3552" y1="2384" y2="2384" x1="3536" />
            <wire x2="3520" y1="2384" y2="2896" x1="3520" />
            <wire x2="3568" y1="2896" y2="2896" x1="3520" />
            <wire x2="3552" y1="2080" y2="2080" x1="3536" />
            <wire x2="3552" y1="2080" y2="2112" x1="3552" />
            <wire x2="4096" y1="2112" y2="2112" x1="3552" />
            <wire x2="3536" y1="2080" y2="2384" x1="3536" />
            <wire x2="4224" y1="336" y2="336" x1="4080" />
            <wire x2="4080" y1="336" y2="816" x1="4080" />
            <wire x2="4208" y1="816" y2="816" x1="4080" />
            <wire x2="4080" y1="816" y2="944" x1="4080" />
        </branch>
        <instance x="3776" y="3440" name="XLXI_129(1:0)" orien="R0" />
        <branch name="XLXN_434(1:0)">
            <wire x2="3584" y1="3328" y2="3344" x1="3584" />
            <wire x2="3584" y1="3344" y2="3392" x1="3584" />
            <wire x2="3584" y1="3392" y2="3408" x1="3584" />
            <wire x2="3776" y1="3408" y2="3408" x1="3584" />
        </branch>
        <branch name="PWM_Reg_31(1:0)">
            <wire x2="4032" y1="3408" y2="3408" x1="4000" />
        </branch>
        <iomarker fontsize="28" x="4032" y="3408" name="PWM_Reg_31(1:0)" orien="R0" />
        <bustap x2="3488" y1="3344" y2="3344" x1="3584" />
        <bustap x2="3488" y1="3392" y2="3392" x1="3584" />
        <branch name="XLXN_434(0)">
            <wire x2="3488" y1="3344" y2="3344" x1="3456" />
        </branch>
        <branch name="XLXN_434(1)">
            <wire x2="3472" y1="3408" y2="3408" x1="3456" />
            <wire x2="3488" y1="3392" y2="3392" x1="3472" />
            <wire x2="3472" y1="3392" y2="3408" x1="3472" />
        </branch>
        <instance x="3232" y="3376" name="XLXI_130" orien="R0" />
        <branch name="SUM(31)">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3136" y="3344" type="branch" />
            <wire x2="3232" y1="3344" y2="3344" x1="3136" />
        </branch>
        <instance x="3232" y="3440" name="XLXI_131" orien="R0" />
        <branch name="DIFF(31)">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="3152" y="3408" type="branch" />
            <wire x2="3232" y1="3408" y2="3408" x1="3152" />
        </branch>
        <instance x="3936" y="1840" name="XLXI_132" orien="R0" />
        <branch name="XLXN_436">
            <wire x2="4240" y1="1168" y2="1168" x1="4000" />
            <wire x2="4000" y1="1168" y2="1648" x1="4000" />
            <wire x2="4256" y1="1648" y2="1648" x1="4000" />
            <wire x2="4000" y1="1648" y2="1712" x1="4000" />
        </branch>
    </sheet>
</drawing>