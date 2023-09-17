import QtQuick 2.2
import QtQuick.Layouts 1.3 as NewControls
import QtQuick.Controls 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQml 2.2
import Style 1.0
import com.branson.runtimefeatures 1.0

Rectangle {
    readonly property string qmltextMaintenanceStatus:qsTr("Maintenance Status")
    readonly property string qmltextMachineDetails:qsTr("Machine Details")
    readonly property string qmltextEventLog:qsTr("Event Log")
    readonly property string qmltextClear:qsTr("CLEAR")
    readonly property string qmltextGenerateReport:qsTr("GENERATE REPORT")
    readonly property string qmltextEventLogText:qsTr("Event Log")
    readonly property string qmltextMoreInfo:qsTr("MORE INFO")

    id:informationPanelId
    width: parent.width
    height: parent.height

    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#757575"
    property string whiteBackgroundColor: "#FFFFFF"
    property real textRectWidth: tabbar.width*(20/100)
    property bool g_Primary: false
    property int dwMachineDetailsIndex: 0
    property int dwEventLogIndex: 1
    property int dwMaintenanceStatusIndex: 0

    Component.onCompleted:
    {
        tabbar.currentIndex = 0
        if(config.getFeatureFlagStatus(RuntimeFeatures.UI_EVENT_LOG) != 1)
        {
            tabbar.removeItem(1)
        }
    }

    TabBar {
        id: tabbar
        width: parent.width*(40/100)
        height: 50
        font.family: Style.regular.name
        font.pixelSize: Style.style3
        anchors.left:parent.left
        anchors.leftMargin: 60
        spacing: 0
        onCurrentIndexChanged:
        {
            if(tabbar.currentIndex === 0)
            {
                machineDetails.getmachineDetails()
            }
        }
        //        TabButton {
        //            id:tabbtn0
        //            height: parent.height
        //            width:80
        //            font.wordSpacing: 6
        //            font.capitalization: Font.MixedCase
        //            hoverEnabled: true
        //            contentItem: Text {
        //                text:qmltextMaintenanceStatus
        //                width: 5
        //                font: tabbar.font
        //                opacity: enabled ? 1.0 : 1
        //                color:  tabbar.currentIndex==dwMaintenanceStatusIndex ? pressedColor : unpressedColor
        //                anchors.leftMargin: (40/480)*parent.width
        //                anchors.left: parent.left
        //                anchors.top:parent.top
        //                anchors.topMargin: 7
        //                anchors.horizontalCenter: parent.horizontalCenter
        //                horizontalAlignment: Text.AlignHCenter
        //                verticalAlignment: Text.AlignVCenter
        //                wrapMode: Text.WordWrap
        //            }
        //            background: Rectangle {
        //                implicitWidth: 50
        //                implicitHeight: 50
        //                height: 2
        //                width: textRectWidth
        //                opacity: enabled ? 1 : 1
        //                anchors.top: tabbtn0.bottom
        //                anchors.topMargin: 0
        //                color:  tabbar.currentIndex==dwMaintenanceStatusIndex?  pressedColor : whiteBackgroundColor
        //                radius: 0
        //            }
        //        }

        TabButton {
            id:tabbtn1
            width: 160
            height: parent.height

            contentItem: Text {
                text:qmltextMachineDetails
                font: tabbar.font
                opacity: enabled ? 1.0 : 1
                color:  tabbar.currentIndex===dwMachineDetailsIndex ? pressedColor : unpressedColor
                anchors.leftMargin: (40/480)*parent.width
                anchors.left: parent.left
                anchors.top:parent.top
                anchors.topMargin: 7
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: 50
                implicitHeight: 50
                height: 2
                anchors.top: tabbtn1.bottom
                anchors.topMargin: 0
                opacity: enabled ? 1 : 1
                color:  tabbar.currentIndex==dwMachineDetailsIndex ? pressedColor : whiteBackgroundColor
                radius: 0
            }
        }

        TabButton {
            id:tabbtn2
            width: 120
            height: parent.height
            anchors.left:tabbtn1.right
            anchors.leftMargin: 15
            contentItem: Text {
                text:qmltextEventLog
                font: tabbar.font
                opacity: enabled ? 1.0 : 1
                color:  tabbar.currentIndex===dwEventLogIndex ? pressedColor : unpressedColor
                anchors.leftMargin: (40/480)*parent.width
                anchors.left: parent.left
                anchors.top:parent.top
                anchors.topMargin: 7
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                height: 2
                implicitWidth: 50
                implicitHeight: 50
                anchors.top: tabbtn2.bottom
                anchors.topMargin: 0
                opacity: enabled ? 1 : 1
                color:  tabbar.currentIndex==dwEventLogIndex?  pressedColor : whiteBackgroundColor
                radius: 0
            }
        }
    }//tab bar ends


    SecondaryButton{
        id:moreInfoBunID
        x:212
        y: 67
        text:qmltextMoreInfo
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 8
        visible: tabbar.currentIndex==dwEventLogIndex ? true : false
        onClicked: {
            popupid.open()
            enableMainwindowOpacity.start()
        }
    }

//    SecondaryButton{
//        id:button
//        x:212
//        y: 67
//        text:qmltextGenerateReport
//        visible: tabbar.currentIndex==dwEventLogIndex ? true : false
//        anchors.top: parent.top
//        anchors.topMargin: 10
//        anchors.right: parent.right
//        anchors.rightMargin: 8

//    }

    Rectangle {
        id: rectangle
        height: 50
        //color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: tabbar.bottom
        anchors.topMargin: 10
        visible: tabbar.currentIndex==dwEventLogIndex ? true : false

        Text {
            id: text1
            x: 33
            width: 166
            height: 14
            text:qmltextEventLogText+"     /    "+userList1Model2.eventHeaderTime
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 15
            font.pixelSize: 12
        }

        Image {
            id: image
            x: 310
            y: 10
            width: 40
            height: 34
            anchors.right: parent.right
            anchors.rightMargin: 10
            source: "qrc:/images/image/filter.svg"
            visible: false
        }

        EventLogPopUp
        {
            id:popupid
        }

        //        SecondaryButton{
        //            id:moreInfoBunID
        //            x:123
        //            y: 8
        //            text:qmltextMoreInfo

        //            anchors.right: image.left
        //            anchors.rightMargin: 110
        //            onClicked: {
        //                popupid.open()
        //                enableMainwindowOpacity.start()
        //            }
        //        }
    }

    NewControls.StackLayout
    {
        id: view
        width: parent.width
        height: parent.height
        NewControls.Layout.fillWidth: true
        NewControls.Layout.fillHeight: true
        currentIndex: tabbar.currentIndex
        x:0
        //        Item
        //        {
        //            id: tab0
        //        }
        Item
        {
            id: tab1
            SystemInfoMachineDetails
            {
                anchors.top: parent.top
                anchors.topMargin: 90
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 170
                width:parent.width - 170
                Component.onCompleted:
                {
                    machineDetails.getmachineDetails()
                }
            }
        }
        Item
        {
            id:tab2
            TableView
            {
                id:eventlogtableId
                //y:110
                anchors.top: parent.top
                anchors.topMargin: 130
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                width: informationPanelId.width

                activeFocusOnTab: true
                frameVisible: true
                backgroundVisible: true
                currentRow: -1
                headerVisible: true
                Component.onCompleted:
                {
                    if(rowCount  > 0)
                    {
                        userList1Model.getSelectedrowdata(0)
                        eventlogtableId.selection.select(0)

                    }
                    var strHeaderOrder = config.getEventTableOrder()
                    if(strHeaderOrder == "")
                        return
                    var strlist = strHeaderOrder.split(",")
                    for(var i=0;i<strlist.length;i++)
                    {
                        for(var j=0;j<eventlogtableId.columnCount;j++)
                        {
                            if(i!=j && eventlogtableId.getColumn(j).title == strlist[i])
                            {
                                eventlogtableId.moveColumn(j,i);
                                break;
                            }
                        }
                    }
                }

                onClicked:
                {
                    userList1Model.getSelectedrowdata(row)
                }

                TableViewColumn{role: "Title";      title:qsTr("Event #");      width: parent.width/20}
                TableViewColumn{role: "Time";       title:qsTr("Date & Time");  width: parent.width*(2/20)}
                TableViewColumn{role: "Eventtitle"; title:qsTr("Event Name");   width: parent.width*(2/20)}
                TableViewColumn{role: "User";       title:qsTr("User Name");    width: parent.width/20}
                TableViewColumn{role: "ID";         title:qsTr("Event ID");     width: parent.width/20}
                TableViewColumn{role: "Identifier"; title:qsTr("Identifier");   width: parent.width*(3/20)}
                TableViewColumn{role: "From";       title:qsTr("From");         width: parent.width/20}
                TableViewColumn{role: "To";         title:qsTr("To");           width: parent.width/20}
                TableViewColumn{role: "PsSerial";   title:qsTr("P/S Serial #"); width: parent.width*(2/20)}
                TableViewColumn{role: "ActSerial";  title:qsTr("Act Serial #"); width: parent.width*(2/20)}
                TableViewColumn{role: "AuxSerial";  title:qsTr("AUX Serial #"); width: parent.width*(2/20)}
                TableViewColumn{role: "Comments";   title:qsTr("Comments");     width: parent.width*(2/20)}
                model: userList1Model
                headerDelegate: Rectangle{
                    height:45
                    width: textItem.implicitWidth
                    color:  "#ECECEC"

                    Connections
                    {
                        target: styleData
                        onPressedChanged:
                        {
                            if(g_Primary)
                            {
                                if(styleData.pressed == false)
                                {
                                    var strHeaderOrder="";
                                    for(var i=0;i <eventlogtableId.columnCount;i++)
                                    {
                                        strHeaderOrder = strHeaderOrder + eventlogtableId.getColumn(i).title + ","
                                    }
                                    config.setEventTableOrder(strHeaderOrder)
                                }
                            }
                            g_Primary =!g_Primary;
                        }
                    }

                    Text {
                        id: textItem
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.leftMargin: 12
                        text: styleData.value
                        elide: Text.ElideRight
                        color: "#757575"
                        renderType: Text.NativeRendering
                        font.pixelSize: 14

                    }

                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        anchors.topMargin: 1
                        width: 1
                        color: "#ccc"
                    }
                }
                rowDelegate: Item {
                    height: 40

                    Rectangle
                    {
                        id: rectangle1
                        color: styleData.selected ? qmlPrimaryColor : "#FFFFFF"
                        anchors.fill: parent
                        border.width: 0.5
                        border.color: "#E9ECEF"
                    }
                    Text{
                        anchors.fill: parent
                        font.pointSize: 10
                        text:title
                    }
                }

                itemDelegate: Item
                {
                    height: 40
                    Text
                    {
                        id: folderText1
                        text:styleData.value
                        fontSizeMode: Text.Fit
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        elide: Text.ElideRight
                        clip:true
                        font.pointSize: 10
                        color: styleData.selected ? "#FFFFFF" :"#898D96"
                    }
                }
                //ScrollBar.horizontal: ScrollBar{}
            }
        }
    }
}
