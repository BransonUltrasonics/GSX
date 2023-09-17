import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4

Item {
    id: plusid

    Button {
        id: button
        width: (50/260)*parent.width
        height: (40/50)*parent.height
        anchors.rightMargin: (115/260)*parent.width
        anchors.leftMargin: (115/260)*parent.width
        anchors.bottomMargin: (5/50)*parent.height
        anchors.topMargin: (5/50)*parent.height
        anchors.fill: parent
        checkable: true
        checked: false
        Image {
            id: plusImage
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: "qrc:/images/image/add-icon.svg"
        }
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            border.width:  1
            border.color: qmlPrimaryColor
            radius: 4
            gradient: Gradient {
                GradientStop { position: 0 ; color: qmlPrimaryColor }
                GradientStop { position: 1 ; color:  qmlPrimaryColor }
            }
        }
        MouseArea {
            anchors.fill:parent
            cursorShape: Qt.PointingHandCursor

            onClicked: {
                /*TBD*/
            }
        }
    }
}
