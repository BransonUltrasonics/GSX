import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: progressBarOuterRectangle

    property real progressVal:0.0
    property alias progressBarControl: progressBarControl
    radius : 2
    border.color: "black"
    border.width : 1
    height:parent.height
    ProgressBar {
        id:progressBarControl
        x:4
        y:4
        height:parent.height - 8
        width: parent.width - 8
        value: progressVal

        style : ProgressBarStyle {
            background: Rectangle {

                color : "white"
                border.width: 0
            }

           progress: Rectangle {
               color: qmlPrimaryColor
            }
        }
    }
}
