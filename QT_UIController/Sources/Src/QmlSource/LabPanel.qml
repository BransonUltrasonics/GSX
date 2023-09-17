import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.runtimefeatures 1.0

Item {
    readonly property string qmltextWeldMode: qsTr("Weld Mode")
    readonly property string qmltextWeldProcess:qsTr("Weld Process")
    readonly property string qmltextParameterAtoZ: qsTr("Parameters A-Z")
    readonly property string qmltextLimits: qsTr("Limits")
    readonly property string qmltextStackRecipe: qsTr("Stack Recipe")
    readonly property string qmltextRecipeRevisions: qsTr("Branson Only")

    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#757575"
    property string unpressedTabBkColor: "#FFFFFF"
    property string tabTextColor: "#757575"
    property string tabTextBkColor: "#FFFFFF"
    property int recipeIndex1: recipeIndex1
    readonly property string bransonOnlyText: qsTr("Branson Only")
    property bool runTimeConfigLimits: config.getFeatureFlagStatus(RuntimeFeatures.UI_LIMITS)

    TabBar
    {
        id: tabbar
        width: parent.width
        height: (49/780)*parent.height
        Layout.fillWidth: true
        spacing: 0
        anchors.left: labPanel.left
        anchors.leftMargin: 40
        font.family: Style.semibold.name
        font.pixelSize: Style.style2
        TabButton
        {
            id:tabbtn0
            height: parent.height
            hoverEnabled: true
            width: implicitWidth
            contentItem: Text
            {
                id:textWeldMode
                text:qmltextWeldMode
                font: tabbar.font
                opacity: 1
                color: unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                opacity: 1
                id:textBkGdWeldMode
                x:0
                y:textWeldMode.height*(38/40)+8
                implicitWidth: parent.width
                implicitHeight: 2
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(0)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        TabButton {
            id:tabbtn1
            width: implicitWidth
            height: parent.height
            contentItem: Text
            {
                id:textWeldProcess
                text:qmltextWeldProcess
                font: tabbar.font
                color:  unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                visible: true
                opacity: 1
            }
            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                id: textBkGdWeldProcess
                y:textWeldProcess.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
                opacity: 1
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(1)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        TabButton {
            id:tabbtn2
            width: implicitWidth
            height: parent.height
            contentItem: Text
            {
                id:textParamAZ
                text:qmltextParameterAtoZ
                font: tabbar.font
                color:  unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                opacity: 1
            }

            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                id:textBkGdParamAZ
                y:textParamAZ.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
                opacity: 1
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(2)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        TabButton {
            id:tabbtn3
            width: implicitWidth
            height: parent.height
            visible: runTimeConfigLimits
            contentItem: Text
            {
                id: textLimits
                text:qmltextLimits
                font: tabbar.font
                color: unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                visible: true
                opacity: 1
            }

            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                opacity: 1
                id: textBkGdLimits
                y:textLimits.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(3)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        TabButton {
            id:stackrecipetab
            width: implicitWidth
            height: parent.height
            contentItem: Text
            {
                id:textStackRecipe
                text:qmltextStackRecipe
                font: tabbar.font
                color:  unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                opacity: 1
            }
            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                id:textBkGdSTackRecipe
                y:textStackRecipe.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
                opacity: 1
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(4)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }

        TabButton {
            id:tabbtn5
            width: implicitWidth
            height: parent.height
            visible: sysconfig.bransonKeyGlobalFlag
            contentItem: Text
            {
                id:textRecipeRevisions
                text:qmltextRecipeRevisions
                font: tabbar.font
                color:  unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                visible: sysconfig.bransonKeyGlobalFlag
                opacity: 1
            }
            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                id:textBkGdRecipeRevisions
                y:textRecipeRevisions.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
                opacity: 1
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(5)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        onCurrentItemChanged:
        {
            changeTabElementColors(tabbar.currentIndex)
            loadelement(tabbar.currentIndex)
            if(tabbar.currentIndex == 1)
            {
                if(config.getFeatureFlagStatus(RuntimeFeatures.UI_PRE_TRIGGER) == 1 ||
                        config.getFeatureFlagStatus(RuntimeFeatures.UI_AFTER_BURST) == 1 ||
                        config.getFeatureFlagStatus(RuntimeFeatures.UI_ENERGY_BRAKE) == 1)
                   loadelement(tabbar.currentIndex)
                else
                    tabbtn1.enabled = false
            }
            else
                loadelement(tabbar.currentIndex)
        }
    }

    /**
    *@breif: To Change tab element color based on current index
    **/
    function changeTabElementColors(index)
    {
        if(index==0 || tabbar.currentIndex==0)
        {
            textWeldMode.color = pressedColor
            textBkGdWeldMode.color = pressedColor
        }
        else
        {
            textWeldMode.color = unpressedColor
            textBkGdWeldMode.color = unpressedTabBkColor
        }

        if(index==1 || tabbar.currentIndex==1)
        {
            if(config.getFeatureFlagStatus(RuntimeFeatures.UI_PRE_TRIGGER) != 1 &&
                    config.getFeatureFlagStatus(RuntimeFeatures.UI_AFTER_BURST) != 1 &&
                    config.getFeatureFlagStatus(RuntimeFeatures.UI_ENERGY_BRAKE) != 1)
            {
                tabbtn1.enabled = false
                textWeldProcess.color = unpressedColor
                textBkGdWeldProcess.color = unpressedTabBkColor
            }
            else
            {
                textWeldProcess.color = pressedColor
                textBkGdWeldProcess.color = pressedColor
            }
        }
        else
        {
            textWeldProcess.color = unpressedColor
            textBkGdWeldProcess.color = unpressedTabBkColor
        }

        if(index==2 || tabbar.currentIndex==2)
        {
            textParamAZ.color = pressedColor
            textBkGdParamAZ.color = pressedColor
        }
        else
        {
            textParamAZ.color = unpressedColor
            textBkGdParamAZ.color = unpressedTabBkColor
        }

        if(index==3 || tabbar.currentIndex==3)
        {
            textLimits.color = pressedColor
            textBkGdLimits.color = pressedColor
        }
        else
        {
            textLimits.color = unpressedColor
            textBkGdLimits.color = unpressedTabBkColor
        }
        if(index==4 || tabbar.currentIndex==4)
        {
            textStackRecipe.color = pressedColor
            textBkGdSTackRecipe.color = pressedColor
        }
        else
        {
            textStackRecipe.color = unpressedColor
            textBkGdSTackRecipe.color = unpressedTabBkColor
        }
        if(index==5 || tabbar.currentIndex==5)
        {
            textRecipeRevisions.color = pressedColor
            textBkGdRecipeRevisions.color = pressedColor
        }
        else
        {
            textRecipeRevisions.color = unpressedColor
            textBkGdRecipeRevisions.color = unpressedTabBkColor
        }
    }

    /**
    *@breif: To load respective QML files
    **/
    function loadelement(index)
    {
        if(index==0 || tabbar.currentIndex==0)
        {
            callWeldModeMenu()
        }
        else if(index==1 || tabbar.currentIndex==1)
        {
            callWeldProcessMenu()
        }
        else if(index==2 || tabbar.currentIndex==2)
        {
            callParamAtoZ()
        }
        else if(index==3 || tabbar.currentIndex==3)
        {
            callLimitsMenu()
        }
        else if(index==4 || tabbar.currentIndex==4)
        {
            callStackRecipe()
        }
        else if(index==5 || tabbar.currentIndex==5)
         {
             callBransonOnlyParameters()
         }

    }

    Component.onCompleted:
    {
        tabbar.currentIndex=0
        textWeldMode.color=pressedColor
        textBkGdWeldMode.color = pressedColor
        callWeldModeMenu()
    }
    Loader{
        id:loaderelement
        x:0
        y:38
        width: labPanel.width
        height: labPanel.height
    }

    /**
    *@breif: To load WeldModeMenu file
    **/
    function callWeldModeMenu()
    {
        loaderelement.setSource("WeldMode.qml",{"width":parent.width,"height":parent.height})
    }
    function callWeldProcessMenu()
    {
        loaderelement.setSource("WeldProcessMenu.qml",{"width":parent.width,"height":parent.height})
    }

    /**
    *@breif: To load ParamAtoZ file
    **/
    function callParamAtoZ()
    {
        loaderelement.setSource("ParamAtoZ.qml",{"width":parent.width,"height":parent.height})
    }

    /**
    *@breif: To load LimitsMenu file
    **/
    function callLimitsMenu()
    {
        loaderelement.setSource("LimitsMenu.qml",{"width":parent.width,"height":parent.height})
    }

    /**
    *@breif: To load WeldProcessStackRecipe file
    **/
    function callStackRecipe()
    {
        loaderelement.setSource("WeldProcessStackRecipe.qml",{"width":parent.width,"height":parent.height})
    }

    /**
    *@breif: To load BransonOnlyParameters file
    **/
    function callBransonOnlyParameters()
    {
        loaderelement.setSource("BransonOnlyParameters.qml",{"width":parent.width,"height":parent.height})
    }
}
