import QtQuick 2.0
import Style 1.0

Item {

    readonly property string qmltextInternal :   qsTr("Internal")
    readonly property string qmltextExternal : qsTr("External")
    property bool checked
    property alias radioLeft: radioLeft
    property alias radioRight: radioRight
    property alias rb0: rb0
    property alias rb1: rb1
    property alias rightText: rbTex0.text
    property alias leftText: rbTex1.text

    Rectangle {
        id: rb0
        width: parent.width
        height: width
        color:"#E9ECEF"
        radius: height/2
        Rectangle {
            id: innerDotLeft
            x: parent.width/2-width/2
            y: parent.height/2-height/2
            width: (20/32)*parent.width
            height: (20/32)*parent.height
            color: checked===true?qmlPrimaryColor:"#E9ECEF"
            radius: height/2
            MouseArea{  id:radioLeft
                anchors.fill:parent
                onClicked: {
                    checked=true
                }
            }
        }
        Text {
            id: rbTex0
            x: rb0.x+rb0.width+(10/32)*parent.width
            y: parent.height/2-height/2
            width: (61/32)*parent.width
            height: parent.height*.6
            text: qmltextInternal
            wrapMode: Text.WordWrap
            font.capitalization: Font.AllUppercase
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            color: checked===true?qmlPrimaryColor:"#E9ECEF"
        }
    }
    Rectangle {
        id: rb1
        x: rbTex0.x+rbTex0.width+(60/32)*parent.width
        y: 0
        width: parent.width
        height: width
        color:"#E9ECEF"
        radius: height/2
        Rectangle {
            id: innerDotRight
            x: parent.width/2-width/2
            y: parent.height/2-height/2
            width: (20/32)*parent.width
            height: (20/32)*parent.height
            radius: height/2
            color: checked===false?qmlPrimaryColor:"#E9ECEF"
            MouseArea{
                id:radioRight
                anchors.fill:parent
                onClicked: {
                    checked=false
                }
            }
        }
        Text {
            id: rbTex1
            x: rb1.width+(10/32)*parent.width
            y: parent.height/2-height/2
            width: (61/32)*parent.width
            height: parent.height*.6
            text: qmltextExternal
            wrapMode: Text.WordWrap
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            color: checked===false?qmlPrimaryColor:"#E9ECEF"
            font.capitalization: Font.AllUppercase
        }
    }
}
