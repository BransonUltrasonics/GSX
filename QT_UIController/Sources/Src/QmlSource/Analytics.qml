import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.openedscreen 1.0
Item {
    id:analyticsItem
    anchors.fill: parent
    readonly property string qmltextProductionOverview :  qsTr("PRODUCTION OVERVIEW")
    readonly property string qmltextResults : qsTr("RESULTS")
    readonly property string qmltextAlarms : qsTr("ALARMS")
    readonly property string qmltextGenerateReport:qsTr("GENERATE REPORT")
    readonly property string qmltextData : qsTr("DATA")
    readonly property string qmltextSystem : qsTr("SYSTEM")

    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#757575"
    property string unpressedTabBkColor: "#FFFFFF"
    property string tabTextColor: "#757575"
    property string tabTextBkColor: "#FFFFFF"
    property real textRectWidth: analyticsTabBar.width*(20/100)
    property int indexofLoader: analyticsTabBar.currentIndex


    property alias analyticTabBar: analyticsTabBar

    Component.onCompleted:
    {
        config.setActivePage(5)
        analyticsTabBar.currentIndex = 0
        //tabbtn1Text.color=pressedColor
        //tab1BkRect.color = pressedColor
    }

    Rectangle{
        id:tabbarRec
        width: parent.width
        height: (49/780)*parent.height
        color: "#ffffff"
        TabBar {
            id: analyticsTabBar
            width: parent.width
            height: parent.height
            Layout.fillWidth: true
            spacing: 5
            anchors.left: parent.left
            anchors.leftMargin: 40
            font.family: Style.semibold.name
            font.pixelSize: Style.style2
          /*  TabButton
            {
                id:tabbtn0
                height: parent.height
                hoverEnabled: true
                width: 100
                contentItem: Text
                {
                    id:tabbtn0Text
                    text:qmltextProductionOverview
                    font: analyticsTabBar.font
                    opacity: 1
                    color: unpressedColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
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
                    y:tabbtn0Text.height*(38/40)+10
                    implicitWidth: parent.width
                    implicitHeight: 2
                }
            }*/
            TabButton {
                id:tabbtn1
                width: 100
                height: parent.height
                contentItem: Text
                {
                    id:tabbtn1Text
                    text:qmltextResults
                    font: analyticsTabBar.font
                    color:  analyticsTabBar.currentIndex===0 ? pressedColor : unpressedColor
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
                    id:tab1BkRect
                    y:tabbtn1Text.height*(38/40)+5
                    implicitWidth:parent.width
                    implicitHeight:2
                    color: analyticsTabBar.currentIndex===0 ? pressedColor : unpressedTabBkColor
                    opacity: 1
                }
            }
            TabButton {
                id:tabbtn2
                width: 100
                height: parent.height
                contentItem: Text
                {
                    id:tabbtn2Text
                    text:qmltextAlarms
                    font: analyticsTabBar.font
                    color:  analyticsTabBar.currentIndex===1 ? pressedColor : unpressedColor
                    horizontalAlignment: Text.Center
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    opacity: 1
                }
                background: Rectangle
                {
                    opacity: 1
                    color: unpressedTabBkColor
                }
                Rectangle
                {
                    id:tab2BkRect
                    y:tabbtn2Text.height*(38/40)+5
                    implicitWidth:parent.width
                    implicitHeight:2
                    opacity: 1
                    color: analyticsTabBar.currentIndex===1 ? pressedColor : unpressedTabBkColor
                }
            }
            onCurrentItemChanged:
            {
                changeTabElementColors(analyticsTabBar.currentIndex)
            }
            Component.onCompleted:
            {
//                if(indexofLoader == 0)
//                {
//                    analyticsTabBar.currentIndex=0
//                    tabbtn0Text.color=pressedColor
//                    textBkGdWeldMode.color = pressedColor
//                }
                 if(indexofLoader == 0)
                {
                    analyticsTabBar.currentIndex=0
                    //tabbtn1Text.color=pressedColor
                    //tab1BkRect.color = pressedColor
                    loadAnalyticsResults()
                }
                else {
                    analyticsTabBar.currentIndex=1
                    //tabbtn2Text.color=pressedColor
                    //tab2BkRect.color = pressedColor
                    loadAnalticsAlaram()
                }
            }
            onCurrentIndexChanged:
            {
                if (analyticsTabBar.currentIndex == 1)
                    analyticalarm.updateAnalyticsAlarmsList()
            }
        }
        PrimaryButton
        {
            id:analyticsGenerateReport
            anchors.right: tabbarRec.right
            anchors.rightMargin: 40
            text: qmltextGenerateReport

            onClicked:
            {
                loaderelement.setSource("Data.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebarchild.visible=true
                titlebar.text = qmltextSystem
                titlebarchildtext.text=qmltextData
                titlbaronclicked="System.qml"
            }
        }
    }

    /**
    *@breif: To change tab color on selection
    **/
    function changeTabElementColors(index){
      /*  if(index==0 || analyticsTabBar.currentIndex==0)
        {
            // tabbtn0Text.color = pressedColor
            // textBkGdWeldMode.color = pressedColor
            // loadProductionOverView()
            //            tabbtn2Text.color = unpressedColor
            //            tab2BkRect.color = unpressedTabBkColor
            //            tabbtn1Text.color = unpressedColor
            //            tab1BkRect.color = unpressedTabBkColor
        }*/
         if(index==0 || analyticsTabBar.currentIndex==0)
        {
            tabbtn1Text.color = pressedColor
            tab1BkRect.color = pressedColor
            loadAnalyticsResults()

            tabbtn2Text.color = unpressedColor
            tab2BkRect.color = unpressedTabBkColor
           // tabbtn0Text.color = unpressedColor
            ///textBkGdWeldMode.color = unpressedTabBkColor

        }
        else if(index==1 || analyticsTabBar.currentIndex==1)
        {
            tabbtn2Text.color = pressedColor
            tab2BkRect.color = pressedColor
            loadAnalticsAlaram()

            tabbtn1Text.color = unpressedColor
            tab1BkRect.color = unpressedTabBkColor
           // tabbtn0Text.color = unpressedColor
           // textBkGdWeldMode.color = unpressedTabBkColor
        }
    }

    Loader{
        id:loaderelementAnalysis
        x:0
        y:45
        width: parent.width
        height: parent.height-y
    }

    /**
    *@breif: Load analytics production overview
    **/
    function loadProductionOverView()
    {
        loaderelementAnalysis.setSource("AnalyticsProductionOverview.qml",{"width":analyticsItem.width,"height":analyticsItem.height-3.5*y})

    }

    /**
    *@breif: Load analytics Results
    **/
    function loadAnalyticsResults()
    {
        loaderelementAnalysis.setSource("AnalyticsResults.qml",{"width":analyticsItem.width,"height":analyticsItem.height-y})

    }

    /**
    *@breif: Load analytical alarms
    **/
    function loadAnalticsAlaram()
    {

        loaderelementAnalysis.setSource("AnalyticsAlarms.qml",{"width":analyticsItem.width,"height":analyticsItem.height-y})
    }

}
