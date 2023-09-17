import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
import com.branson.menu 1.0

Rectangle {
    readonly property string qmltextEmergencyStop: qsTr("Emergency Stop")
    readonly property string qmltextReset: qsTr("RESET")
    readonly property string qmltextPalmButtonsMsg : qsTr("Press palm buttons to retract horn to home position")

    id: emergencyStopPopup
    width: parent.width
    height: parent.height
    visible: false
    color: "#80000000"

    MouseArea
    {
        anchors.fill: parent
        id:emergencyStopPopupMa
        onClicked: {

        }
    }

    property alias resetbuttonvisibility: resetbutton.visible

    /**
    *@breif: Open emergency stop popup
    **/
    function open()
    {
        emergencyStopPopup.visible = true
    }

    /**
    *@breif: Close emergency stop popup
    **/
    function close()
    {
        emergencyStopPopup.visible = false
    }

    function clearEstopResetPopUp()
    {
        resetbutton.visible = false
        emergencyStopPopup.close()
        disableMainwindowOpacity.start()
    }

    Connections{
        target: alarmNotify

        onClearEstopResetPopUpSignal: {
            clearEstopResetPopUp()
        }
    }

    Rectangle{
        id: emergencyStopRec
        width: 350
        height: 350
        x: window.width/2 - width/2
        y:window.height/2 - height/2
        opacity: 1
        color: "transparent"
        Rectangle{
            id: emergencyStopRound
            width: parent.width
            height: parent.height
            radius: width/2
            color: "transparent"
            Image {
                id: imageEmergency
                width: parent.width
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                /*Image icon update based on System Type*/
                source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/EmergencyStop_ASX.png" :
                                                               "qrc:/images/image/EmergencyStop.png"
                Text
                {
                    id: emergencyText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qmltextEmergencyStop
                    color: "#ffffff"
                    font.pixelSize: Style.style6
                    font.family: Style.semibold.name
                }
                SecondaryButton
                {
                    id:resetbutton
                    anchors.top: emergencyText.bottom
                    anchors.topMargin: 10
                    text:qmltextReset
                    visible: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked:
                    {
                        resetToBl.start()
                    }
                }
            }
        }
    }
    Timer
    {
        id: resetToBl
        interval: 2000
        triggeredOnStart: false
        onTriggered:
        {
            var status
            status = alarmDetails.sendResetEmergencyReq()
            if(status === 1)
            {
                resetbutton.visible = false
                emergencyStopPopup.close()
                disableMainwindowOpacity.start()
            }
        }
    }
}
