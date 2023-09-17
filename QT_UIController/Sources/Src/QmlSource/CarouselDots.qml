import QtQuick 2.0

Item {
    id:cb
    implicitWidth: 320
    implicitHeight:  320
    property bool selected: false

     property alias cb: cb
    property alias marLeft: marLeft
    property alias marRight: marRight
    width: 320

    Rectangle {
        id: dotLeft
        x: 0
        y: 0
        width: parent.width
        height: width
        color: selected===true?qmlPrimaryColor:"#E9ECEF"
        radius: height/2

        MouseArea{  id:marLeft
            anchors.fill:parent
            onClicked: {
                //selected=true
            }
        }
    }

    Rectangle {
        id: dotRight
        y: 0
        width: parent.width
        height: width
        anchors.left: dotLeft.right
        anchors.leftMargin: 15
        color: selected===false?qmlPrimaryColor:"#E9ECEF"
        radius: height/2

        MouseArea{
            id:marRight
            anchors.fill:parent
            onClicked: {
              //  selected=false
            }
        }
    }
}
