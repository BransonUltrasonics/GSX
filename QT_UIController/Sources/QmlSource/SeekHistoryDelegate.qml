import QtQuick 2.0
import Style 1.0

Item {
    id: seekhistoryListCtrlDelegate

    objectName: qsTr(model.modelData.scanId)

    width: parent.width
    height: 60

    implicitHeight: 60
    implicitWidth: width

    property int scaleRatio:1;

    property int custPadding:10;
    property double fontSizeScaleFactor : window.height / 768
    property string itemNumberFontFamily:Style.semibold.name
    property int  itemNumberPixelSize:Style.style1
    property string itemDateTimeFontFamily:Style.semibold.name
    property int  itemDateTimePixelSize:Style.style1;

    Rectangle {
        id: weldItemHist


        implicitWidth: parent.width
        implicitHeight: parent.height

        width: parent.width
        height : 60 * scaleRatio
        color: (index == seekhistoryListCtrlView.indexSelected) ?  "#d9e8fa" :  "#f8f9fa"
        border.width: 0


        Rectangle {
            border.width: 1
            height: 2
            width: parent.width - 20
            anchors.margins: 20
            opacity: 0.4
            border.color: "#898d96"

            visible: (index == seekhistoryListCtrlView.indexSelected || index == (seekhistoryListCtrlView.indexSelected + 1)) ? false : true
        }

        Text {
            id: scanHistoryNumber

            anchors.left: parent.left
            anchors.leftMargin: 2 * custPadding

            anchors.top: parent.top
            anchors.topMargin: custPadding

            height: 25
            color: "#898d96"
            font.pixelSize: itemNumberPixelSize
            font.family:itemNumberFontFamily

            text: model.modelData.scanId
        }

        Text {
            id: scanHistoryDate

            anchors.left: parent.left
            anchors.leftMargin: 2 * custPadding

            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 2 * custPadding


            height: 20
            color: "#898d96"
            text: model.modelData.dateTime
            font.pixelSize: itemDateTimePixelSize
            font.family:itemDateTimeFontFamily
        }

        MouseArea {
            id: scanHistoryMouseArea
            anchors.fill: parent
            cursorShape: Qt.ToolTip
            enabled: true

            onClicked:
            {
                seekhistoryListCtrlView.currentIndex = index
            }
        }
    }
}
