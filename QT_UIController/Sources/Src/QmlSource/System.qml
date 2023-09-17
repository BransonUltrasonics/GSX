import QtQuick 2.0
import Qt.labs.calendar 1.0
import Qt.labs.folderlistmodel 2.2
import com.branson.menu 1.0
import com.branson.openedscreen 1.0
import Style 1.0
Item {
    id:systemid
    readonly property string qmltextConfiguration :  qsTr("Configuration")
    readonly property string qmltextconfigTitleBar :   qsTr("CONFIGURATION")
    readonly property string qmltextTooling :  qsTr("TOOLING")
    readonly property string qmltextCalibration :  qsTr("CALIBRATION")
    readonly property string qmltextDiagnostics :  qsTr("DIAGNOSTICS")
    readonly property string qmltextData : qsTr("DATA")
    readonly property string qmltextInformation :qsTr("INFORMATION")
    readonly property string qmltextFutureCapability: qsTr("Future capability coming soon.")

    Component.onCompleted:
    {
        config.setActivePage(6)
    }

    implicitWidth:  1880
    implicitHeight: 910
    width:(1880/1920)*window.width
    height:(910/1080)*window.height
    x:(20/1920)*window.width
    y:(20/1080)*window.height
    property var rect_iconWidthRatio:  .35
    property var rect_iconHeightRatio: .45
    property var rect_iconImplicitWidth: 600
    property var rect_iconImplicitHeight:420
    property var rect_iconY : 90
    property var spaceBwRectangles:20

    property var implicitTextWidth: 388
    property var implicitTextHeight: 51
    property var implicitTextFontSize: 40
    property var implicitTextY: 370

    Rectangle {

        id: rect_configuration
        width:/*width>height ? height:*/(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
        height:/*width<height ? width:*/ (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
        color: "#ffffff"
        border.width: 1

        Rectangle {
            id: rect_icon
            x: parent.width/2-width/2
            y: (rect_iconY/rect_iconImplicitHeight)*parent.height

            width:rect_iconWidthRatio*parent.width
            height:width
            color: "#ececec"
            radius: width/2
            Image {
                x:parent.width/2-width/2
                y:parent.height/2-height/2
                sourceSize.height: parent.height/2
                sourceSize.width: parent.width/2
                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/image/configuration.svg"
            }

        }

        Text {
            id: text0
            x: parent.width/2-width/2
            y: rect_icon.y+(implicitTextY/600)*parent.height

            text: qmltextConfiguration
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.regular.name
            font.pixelSize:  Style.style7
            color: "#898D96"
        }
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked:
            {
                if (isPermissionAllowed(Features.SYSTEM_CONFIGURATION))
                {
                    sysconfig.Init()
                    loaderelement.setSource("Configuration.qml",{"width":(400/1920)*window.width,"height":window.height})
                    titlebarchild.visible=true
                    titlebarchildtext.text=qmltextconfigTitleBar
                    titlbaronclicked="System.qml"
                }
            }
        }
    }

    Rectangle {
        id: rect_configuration1
        x: rect_configuration.x+ rect_configuration.width+(spaceBwRectangles/parent.implicitWidth)*parent.width
        y: 0
        width:/*width>height ? height:*/(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
        height:/*width<height ? width:*/ (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
        color: "#ffffff"
        border.width: 1
        Rectangle {
            id: rect_icon1
            x: parent.width/2-width/2
            y: (rect_iconY/rect_iconImplicitHeight)*parent.height
            width:rect_iconWidthRatio*parent.width
            height:width
            color: "#ececec"
            radius: width/2
            Image {
                x: parent.width/2-width/2
                y: parent.height/2-height/2
                source: "qrc:/images/image/tooling.svg"
                sourceSize.height: parent.height/2
                fillMode: Image.PreserveAspectFit
                sourceSize.width: parent.width/2
            }
        }

        Text {
            id: text1
            x: parent.width/2-width/2
            y: rect_icon1.y+(implicitTextY/600)*parent.height

            text: qmltextTooling
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.regular.name
            font.pixelSize:  Style.style7
            color: "#898D96"
        }
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked:
            {
                messagePopup.open(messagePopup.info_title, qmltextFutureCapability, messagePopup.without_button)
//                loaderelement.setSource("Tooling.qml",{"width":(400/1920)*window.width,"height":window.height})
//                titlebarchild.visible = true
//                titlebarchildtext.text = qmltextTooling
//                titlbaronclicked = "System.qml"
            }
        }
    }

    Rectangle {
        id: rect_configuration2
        x: rect_configuration1.x+rect_configuration1.width+(spaceBwRectangles/parent.implicitWidth)*parent.width
        y: 0
        width:(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
        height: (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
        color: isPermissionAllowed(Features.QUICK_CALIBRATION) ? "#ffffff": "#757575"
        border.width: 1
        enabled: isPermissionAllowed(Features.QUICK_CALIBRATION) ? true : false
        Rectangle {
            id: rect_icon2
            x: parent.width/2-width/2
            y: (rect_iconY/rect_iconImplicitHeight)*parent.height
            width:rect_iconWidthRatio*parent.width
            height:width
            radius: width/2
            color: isPermissionAllowed(Features.QUICK_CALIBRATION) ? "#ececec": "#757575"
            Image {
                x: parent.width/2-width/2
                y: parent.height/2-height/2
                source: "qrc:/images/image/calibration.svg"
                sourceSize.height: parent.height/2
                fillMode: Image.PreserveAspectFit
                sourceSize.width: parent.width/2
            }
        }

        Text {
            id: text2
            x: parent.width/2-width/2
            y: rect_icon2.y+(implicitTextY/600)*parent.height

            text:qmltextCalibration
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.regular.name
            font.pixelSize:  Style.style7
            color: isPermissionAllowed(Features.QUICK_CALIBRATION) ? "#898D96": "#BEC1C3"
        }
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked:{
                if (isPermissionAllowed(Features.QUICK_CALIBRATION))
                {
                    loaderelement.setSource("Calibration.qml",{"width":(400/1920)*window.width,"height":window.height})
                    titlebarchild.visible=true
                    titlebarchildtext.text=qmltextCalibration
                    titlbaronclicked="System.qml"

                }
            }
        }
    }

//    Rectangle {
//        id: rect_configuration3

//        y: rect_configuration.y+rect_configuration.height+(spaceBwRectangles/parent.implicitHeight)*parent.height
//        width:/*width>height ? height:*/(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
//        height:/*width<height ? width:*/ (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
//        color: "#ffffff"
//        border.width: 1
//        Rectangle {
//            id: rect_icon3
//            x: parent.width/2-width/2
//            y: (rect_iconY/rect_iconImplicitHeight)*parent.height
//            width:rect_iconWidthRatio*parent.width
//            height:width
//            color: "#ececec"
//            radius: width/2
//            Image {
//                x: parent.width/2-width/2
//                y: parent.height/2-height/2
//                source: "qrc:/images/image/diagnostics.svg"
//                sourceSize.height: parent.height/2
//                fillMode: Image.PreserveAspectFit
//                sourceSize.width: parent.width/2
//            }
//        }

//        Text {
//            id: text3
//            x: parent.width/2-width/2
//            y: rect_icon3.y+(implicitTextY/600)*parent.height

//            text: qmltextDiagnostics
//            font.capitalization: Font.AllUppercase
//            verticalAlignment: Text.AlignVCenter
//            horizontalAlignment: Text.AlignHCenter
//            font.family: Style.regular.name
//            font.pixelSize:  Style.style7
//            color: "#898D96"
//        }
////        MouseArea{
////            anchors.fill: parent
////            cursorShape: "PointingHandCursor"
////            onClicked:{
////                if (isPermissionAllowed(Features.DIAGONISTICS))
////                {
////                    // TODO
////                }
////            }
////        }
//    }

    Rectangle {
        id: rect_SystemData
        y: rect_configuration.y+rect_configuration.height+(spaceBwRectangles/parent.implicitHeight)*parent.height

//        x: rect_configuration3.x+rect_configuration3.width+(spaceBwRectangles/parent.implicitWidth)*parent.width
       // y: rect_configuration1.y+rect_configuration1.height+(spaceBwRectangles/parent.implicitHeight)*parent.height
        width:/*width>height ? height:*/(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
        height:/*width<height ? width:*/ (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
        color: isPermissionAllowed(Features.SYSTEM_DATA) ? "#ffffff": "#757575"
        border.width: 1
        enabled: isPermissionAllowed(Features.SYSTEM_DATA) ? true : false
        Rectangle {
            id: rect_SystemDataIcon
            x: parent.width/2-width/2
            y: (rect_iconY/rect_iconImplicitHeight)*parent.height
            width:rect_iconWidthRatio*parent.width
            height:width
            color: isPermissionAllowed(Features.SYSTEM_DATA) ? "#ececec": "#757575"
            radius: width/2
            Image {
                x: parent.width/2-width/2
                y: parent.height/2-height/2
                source: "qrc:/images/image/data.svg"
                sourceSize.height: parent.height/2
                fillMode: Image.PreserveAspectFit
                sourceSize.width: parent.width/2
            }
        }

        Text {
            id: text_SystemData
            x: parent.width/2-width/2
            y: rect_SystemDataIcon.y+(implicitTextY/600)*parent.height

            text: qmltextData
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.regular.name
            font.pixelSize:  Style.style7
            color: isPermissionAllowed(Features.SYSTEM_DATA) ? "#898D96": "#BEC1C3"
        }
        MouseArea{
            id:dataMA
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if((isPermissionAllowed(Features.SYSTEM_DATA)))
                {
                    loaderelement.setSource("Data.qml",{"width":(400/1920)*window.width,"height":window.height})
                    titlebarchild.visible=true
                    titlebarchildtext.text=qmltextData
                    titlbaronclicked="System.qml"
                }
            }
        }
    }

    Rectangle {
        id: rect_configuration5
        x: rect_SystemData.x+rect_SystemData.width+(spaceBwRectangles/parent.implicitWidth)*parent.width
        y: rect_configuration2.y+rect_configuration2.height+(spaceBwRectangles/parent.implicitHeight)*parent.height
        width:/*width>height ? height:*/(rect_iconImplicitWidth/parent.implicitWidth)*parent.width
        height:/*width<height ? width:*/ (rect_iconImplicitHeight/parent.implicitHeight)*parent.height
        color: "#ffffff"
        border.width: 1
        Rectangle {
            id: rect_icon5
            x: parent.width/2-width/2
            y: (rect_iconY/rect_iconImplicitHeight)*parent.height
            width:rect_iconWidthRatio*parent.width
            height:width
            color: "#ececec"
            radius: width/2
            Image {
                x: parent.width/2-width/2
                y: parent.height/2-height/2 + 10
                source: "qrc:/images/image/info_Black.svg"
                sourceSize.height: parent.height/2
                fillMode: Image.PreserveAspectFit
                sourceSize.width: parent.width/2
            }
        }

        Text {
            id: text5
            x: parent.width/2-width/2
            y: rect_icon5.y+(implicitTextY/600)*parent.height

            text:qmltextInformation
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.regular.name
            font.pixelSize:  Style.style7
            color: "#898D96"
        }
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked:
            {
                userList1Model.getEventRecords()
                loaderelement.setSource("InformationLabPanel.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebarchild.visible=true
                titlebarchildtext.text=qmltextInformation
                titlbaronclicked="System.qml"
            }
        }
    }
}
