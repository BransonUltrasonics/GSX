import QtQuick 2.0

Item {
    id:cb
    implicitWidth: 320
    implicitHeight:  320
    property int selected: 1

    property alias cb: cb
    property alias mar1: mar1
    property alias mar2: mar2
    property alias mar3: mar3
    width: 320

    Rectangle {
        id: dot1
        width: parent.width
        height: width
        color: selected===1?qmlPrimaryColor:"#E9ECEF"
        radius: height/2

        MouseArea{  id:mar1
            anchors.fill:parent
            onClicked: {
                //selected=true
            }
        }
    }
    Rectangle {
        id: dot2
        anchors.left: dot1.right
        anchors.leftMargin: 15
        width: parent.width
        height: width
        color: selected===2?qmlPrimaryColor:"#E9ECEF"
        radius: height/2

        MouseArea{
            id:mar2
            anchors.fill:parent
            onClicked: {
                //  selected=false
            }
        }
    }
    Rectangle {
        id: dot3
        anchors.left: dot2.right
        anchors.leftMargin: 15
        width: parent.width
        height: width
        color: selected===3?qmlPrimaryColor:"#E9ECEF"
        radius: height/2

        MouseArea{
            id:mar3
            anchors.fill:parent
            onClicked: {
                //  selected=false
            }
        }
    }
}
