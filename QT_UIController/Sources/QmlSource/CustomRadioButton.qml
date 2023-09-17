import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import Style 1.0


RadioButton {
    id: radioButton

    signal radioButtonClick()
//    property alias buttonGroup: radioButton.exclusiveGroup
    property alias radiobuttontext: radioButton.text
    property string labelText: " "
    property string offColor: "#898d96"
    property string onColor: qmlPrimaryColor
    property string img: "qrc:/images/image/32x32-dot-grey.png"
//    ExclusiveGroup { id: group }

    text: labelText
//    exclusiveGroup: group
    style: RadioButtonStyle {
        indicator: Image {
            id:greydot
            width: 32
            height:  32
            source: img
            Image {
                id:bluedot
                anchors.centerIn: parent
                visible: control.checked
                width: 20
                height: 20
                source: "qrc:/images/image/32x32-dot-blue.png"
            }
        }
        label: Text {
            color: control.checked == 1 ? onColor : offColor
            text:radioButton.text
            wrapMode: Text.WordWrap
            font.family: Style.regular.name
            font.pixelSize: Style.style3
        }
    }
    onClicked: {
        radioButtonClick()
    }
}

