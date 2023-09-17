import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick 2.0

Rectangle{
    readonly property string qmltextLoading : qsTr("Loading")

   id: busyIndicatorRectId
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false

    property alias running: spinner.running
    property alias message: label.text
    property alias busyIndicatorRectIdArea: busyIndicatorRectIdArea.enabled

//    function displayBusyIndicator(busyMessage)
//    {
//        busyIndicatorRectId.visible = true
//        busyIndicatorRectId.running = true
//        busyIndicatorRectId.message =  busyMessage
//        busyIndicatorRectId.busyIndicatorRectIdArea = true
//    }

//    function quitBusyIndicator()
//    {
//        busyIndicatorRectId.running = true.visible = false
//        busyIndicatorRectId.running = false
//        busyIndicatorRectId.busyIndicatorRectIdArea = false
//    }

    MouseArea{
        id: busyIndicatorRectIdArea
        anchors.fill: parent
        onClicked: {}
        enabled: false
    }

    Row
    {
        anchors.centerIn: parent
        spacing: 8

        BusyIndicator{
            id:spinner
            running:false
        }
        Text {
            id: label
            text:qmltextLoading+"..."
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignTop
            font.family: "Lato"
            font.pixelSize: 16
        }
    }
}
