import QtQuick 2.0
import QtQuick.Layouts 1.3
import Style 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4

Popup {
    id: id_loader_wait_popup
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    width :(( parent.width * 16 ) /100)
    height: ((parent.height * 16 )/100)
    padding:0
    leftMargin: (parent.width - (parent.width/2)) /2
    background: Rectangle {
        color: "#ECECEC"
    }
    x: (window.width - width) / 2
    y: (window.height - height) / 2

    readonly property string qmlTextLoaderWait :qsTr("Please wait...")
    readonly property int loaderDelay : 1000
    //property bool activateloader: false

    signal loaderOpen;
    signal loaderClose;
    signal loaderRunning;

    Rectangle{
        id:id_loader_wait_title
        width: parent.width
        anchors.left: parent.Left
        anchors.top: parent.Top
        height: 40
        color: qmlPrimaryColor
        Text {
            id: id_title
            text: qmlTextLoaderWait
            font.family: Style.regular.name
            font.pixelSize:  0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color:"#FFFFFF"
            font.bold: true
        }
    }

    Rectangle
    {
        id: id_loader_wait
        width:parent.width
        height: parent.height - id_loader_wait_title.height
        anchors.top: id_loader_wait_title.bottom
        color : "#ECECEC"

        BusyIndicator {
            id: id_loader_indicator
            running: id_loader_delay.running
            anchors.centerIn: parent
        }
    }

    Timer {
        id:id_loader_delay
        interval: loaderDelay
        triggeredOnStart: false
        repeat: true
        onTriggered:
        {
            loaderRunning();
        }
    }

    onOpened: {
        id_loader_delay.start();
        loaderOpen();
    }

    onClosed: {
        loaderClose();
        id_loader_delay.stop();
        id_loader_wait_popup.close();
    }
}

