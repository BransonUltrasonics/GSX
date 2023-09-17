import QtQuick 2.0
import QtQuick.Layouts 1.3
import Style 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4

Item {
    property int primaryButtonWidth: 185
    readonly property string primaryButtonText: qsTr("Optimize DB")
    readonly property string qmlTextVacuumWait :qsTr("Please wait...")
    readonly property string qmlTextVacuumRunning :qsTr("Operation may take up to a minute...")
    readonly property int  vacuumDelay : 1000

    property int nVacuumCount : 0
    property bool bVacuumInProgress: false

    Rectangle
    {   //Start of points in Wizard steps
        id:id_vacuum_rectangle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        anchors.top : parent.top

        PrimaryButton
        {
            id:id_vacuum_button
            anchors.leftMargin: 60
            anchors.left: id_vacuum_rectangle.right
            width: primaryButtonWidth
            text: primaryButtonText
            height: 60
            Image
            {
                sourceSize.width: 50
                sourceSize.height: 50
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                /*Image icon update based on System Type*/
                source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/Vacuum_ASX.png" :
                                                               "qrc:/images/image/Vacuum.png"
            }
            onClicked:
            {
                vacuum.optimizeDB();
                recipe.updateStatusText(qmlTextVacuumRunning);
                id_vacuum_wait_popup.open();
            }
        }
    }

    //waiting vacuum
    Popup {
        id: id_vacuum_wait_popup
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

        Rectangle{
            id:id_vacuum_wait_title
            width: parent.width
            anchors.left: parent.Left
            anchors.top: parent.Top
            height: 40
            color: qmlPrimaryColor
            Text {
                id: id_title
                text: qmlTextVacuumWait
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
            id: id_vacuum_wait
            width:parent.width
            height: parent.height - id_vacuum_wait_title.height
            anchors.top: id_vacuum_wait_title.bottom
            color : "#ECECEC"

            BusyIndicator {
                id: id_vacuum_busy_indicator
                running: id_vacuum_timer_delay.running
                anchors.centerIn: parent
            }
        }

        onOpened: {
            nVacuumCount=0;
            enableMainwindowOpacity.start();
            id_vacuum_timer_delay.start();
            bVacuumInProgress=true;
        }

        onClosed: {
            disableMainwindowOpacity.start();
            bVacuumInProgress=false;
        }
    }

    //waiting vacuum
    Timer {
        id:id_vacuum_timer_delay
        interval: vacuumDelay
        triggeredOnStart: false
        repeat: true
        onTriggered:
        {
            nVacuumCount++;
            if(nVacuumCount>=3)
            {
                id_vacuum_timer_delay.stop();
                id_vacuum_wait_popup.close();
            }
            console.log(nVacuumCount);
        }
    }

}
