import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias mainLabel: mainLabelItem.text
    property alias secondaryLabels: secondaryLabelsItem.text
    property alias iconSource: icon.source

    property bool isChekable: false
    property bool isChecked: false

    property int bounds: 5

    property alias mainFont: mainLabelItem.font
    property alias secondaryFont: secondaryLabelsItem.font
    property alias mainFontColor: mainLabelItem.color
    property alias secondaryFontColor: secondaryLabelsItem.color

    property color keyColor: "#757575"
    property color keyPressedColor: "#ffffff"

    signal clicked()
    signal alternatesClicked(string symbol)

    Rectangle
    {
        id: backgroundItem
        anchors.fill: parent
        anchors.margins: root.bounds
        radius: 3
        color: isChecked || mouseArea.pressed ? keyPressedColor : keyColor;
        layer.enabled: true
        layer.effect: DropShadow
        {
            //transparentBorder:true
            horizontalOffset:0
            verticalOffset:1
            opacity:.2
            radius:3
            color:"#80000000"
            samples:7
        }
    }

    Column
    {
        anchors.centerIn: backgroundItem

        Text {
            id: secondaryLabelsItem
            smooth: true
            anchors.right: parent.right
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                id: icon
                smooth: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: mainLabelItem
                smooth: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Row {
        id: alternatesRow
        property int selectedIndex: -1
        visible: false
        anchors.bottom: backgroundItem.top
        anchors.left: backgroundItem.left

        Repeater {
            model: secondaryLabels.length

            Rectangle {
                property bool isSelected: alternatesRow.selectedIndex == index
                color: isSelected ? mainLabelItem.color : keyPressedColor
                height: backgroundItem.height
                width: backgroundItem.width

                Text {
                    anchors.centerIn: parent
                    text: secondaryLabels[ index ]
                    font: mainLabelItem.font
                    color: isSelected ? keyPressedColor : mainLabelItem.color
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressAndHold: alternatesRow.visible = true
        onClicked:
        {
            alarmDetails.emitKey(Qt.Key_B)
            if (isChekable) isChecked = !isChecked
            root.clicked()
        }

        onReleased: {
            alternatesRow.visible = false
            if (alternatesRow.selectedIndex > -1)
                root.alternatesClicked(secondaryLabels[alternatesRow.selectedIndex])
        }

        onMouseXChanged: {
            alternatesRow.selectedIndex =
            (mouseY < 0 && mouseX > 0 && mouseY < alternatesRow.width) ?
                        Math.floor(mouseX / backgroundItem.width) :
                        -1;
        }
    }
}
