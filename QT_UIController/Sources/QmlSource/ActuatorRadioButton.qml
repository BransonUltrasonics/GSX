import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import Style 1.0

Item {

    property bool checked
    property color selectedColor: qmlPrimaryColor
    property color unselectedColor: "#E9ECEF"
    property color bordercolor: "black"
    readonly property color textColor: "#868e96"

    property int rowWidth: 200
    property int maxWidth: 32
    property int maxHeight: 32
    property int innerRectWidth: 20
    property int innerRecHeight: 20
    property alias rbTextdata:rbText.text
    property alias radioLeft: radioLeft
    property bool checkedStatus

    RowLayout {
        id:groupId
        width:rowWidth+20
        RadioButton {
            id: rb0
            Text {
                id: rbText
                x:rb0.width+20
                y: parent.height/2-height/2
                text: rbTextdata
                color: checkedStatus===true?selectedColor:textColor
                font {
                    family: Style.semibold.name
                    pixelSize: Style.style2
                    capitalization: Font.AllUppercase
                }
            }
            MouseArea
            {
                id:radioLeft
                anchors.fill:parent
                onClicked:
                {
                    if(checkedStatus)
                        checked=true
                    else
                        checked=false
                }
            }
            style: RadioButtonStyle {
                indicator: Rectangle {
                    width: maxWidth
                    height: maxHeight
                    color:unselectedColor
                    border.color: bordercolor
                    border.width: 1
                    radius: height/2
                    Rectangle {
                        id: innerDotLeft
                        x: parent.width/2-width/2
                        y: parent.height/2-height/2
                        width: innerRectWidth
                        height:innerRecHeight
                        color: checkedStatus===true?selectedColor:unselectedColor
                        radius: height/2
                    }
                }
            }
        }
    }
}
