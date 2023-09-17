import QtQuick 2.0
import Style 1.0
Item {
    readonly property string qmltextErrorType :  qsTr("Error type")
    readonly property string qmltextAlarmID :  qsTr("Alarm ID")
    readonly property string qmltextRecipe :  qsTr("Recipe")
    readonly property string qmltextPart :  qsTr("Cycle")
    readonly property string qmltextStatus :  qsTr("Status")
    readonly property string qmltextDate :  qsTr("Date")
    readonly property string qmltextTime :  qsTr("Time")
    readonly property string qmltextRecipeVersion : qsTr("Recipe Ver")
    readonly property string qmltextPCAssembly :  qsTr("PC Assembly")
    readonly property string qmltextACAssembly :qsTr("AC Assembly")
    readonly property string qmltexthashSymbol :"#"

    Rectangle {
        id: titlesRec
        width: parent.width
        height: parent.height
        x:0
        color: "#ffffff"
        border.color: "#ffffff"
        border.width: 2
        Text {
            id: errorType
            text: qmltextErrorType
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width/9
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }

        Text {
            id: alarmId
            anchors.verticalCenter: parent.verticalCenter
            text: qmltextAlarmID
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            anchors.left: parent.left
            anchors.leftMargin: parent.width/6.0
            anchors.top:parent.top
            anchors.topMargin: 10
            width: parent.width/9
        }

        Text {
            id: recipe
            anchors.verticalCenter: parent.verticalCenter
            text: qmltextRecipe
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            anchors.left: parent.left
            anchors.leftMargin:parent.width/3.8
            anchors.top:parent.top
            anchors.topMargin: 10
            width: parent.width/9
        }
        Text {
            id: recipeVer
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            text:qmltextRecipeVersion
            anchors.left: parent.left
            anchors.leftMargin:parent.width/2.8
            anchors.top:parent.top
            anchors.topMargin: 10
            width: parent.width/9
        }

        Text {
            id: part
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            text:qmltextPart+qmltexthashSymbol
            anchors.left: parent.left
            anchors.leftMargin:parent.width/2.1
            anchors.top:parent.top
            anchors.topMargin: 10
            width: parent.width/9
        }

        Text {
            id: dateTime
            anchors.verticalCenter: parent.verticalCenter
            text: qmltextDate+"/"+qmltextTime
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            anchors.left: parent.left
            anchors.leftMargin:parent.width/1.7
            anchors.top:parent.top
            anchors.topMargin: 10
            width: parent.width/9
        }
    }
}
