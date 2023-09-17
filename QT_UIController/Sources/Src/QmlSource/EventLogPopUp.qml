import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick 2.7
import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import Style 1.0

Popup{
    readonly property string qmltextUser:               qsTr("User")
    readonly property string qmltextMoreInfo:           qsTr("More Information")
    readonly property string qmltextDateTime:           qsTr("Date & Time")
    readonly property string qmltextEventName:          qsTr("Event Name")
    readonly property string qmltextEventID:            qsTr("Event ID")
    readonly property string qmltextEventFrom:          qsTr("From")
    readonly property string qmltextEventTo:            qsTr("To")
    readonly property string qmltextEventIdentifier:    qsTr("Identifier")
    readonly property string qmltextPsSerialNo:         qsTr("PsSerialNo")
    readonly property string qmltextActSerialNo:        qsTr("ActSerialNo")
    readonly property string qmltextAuxSerialNo:        qsTr("AuxSerialNo")
    readonly property string qmltextDone:               qsTr("DONE")
    readonly property string qmltextComment:            qsTr("Comment")
    readonly property string qmltextColon:              " : "

    property alias eventLogPopupId: eventLogPopupId
    id:eventLogPopupId
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    Rectangle{
        width: 665
        height: 450
        x:  400
        y : 50
        color: "#FFFFFF"
        DropShadow{
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.20
            source: eventlogpoptitleId
            anchors.fill: eventlogpoptitleId
        }

        Rectangle{
            id:eventlogpoptitleId
            width: parent.width
            height: 45
            color:qmlPrimaryColor
            Text {
                id:eventlogheaderID
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qmltextMoreInfo
                font.pixelSize: Style.style5//15
                color: "#FFFFFF"
                font.family: Style.regular.name
            }
        }

        Text {
            id: userID
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: eventlogpoptitleId.bottom
            anchors.topMargin: 20
            width: 29
            height: 14
            text: qmltextUser+qmltextColon+userList1Model2.eventUser
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: dateTime
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: userID.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextDateTime + qmltextColon + userList1Model2.eventDateTime
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: eventID
            anchors.left: dateTime.right
            anchors.leftMargin: 80
            anchors.top: eventlogpoptitleId.bottom
            anchors.topMargin: 20
            text:qmltextEventID + qmltextColon + userList1Model2.eventID
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: eventName
            anchors.left: eventID.left
            anchors.top: dateTime.top
            text: qmltextEventName + qmltextColon + userList1Model2.eventName
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: eventFrom
            anchors.left: dateTime.left
            anchors.top: dateTime.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextEventFrom + qmltextColon + userList1Model2.eventFrom
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: eventTo
            anchors.left: eventName.left
            anchors.top: eventFrom.top
            text: qmltextEventTo + qmltextColon + userList1Model2.eventTo
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: eventIdentifier
            anchors.left: eventFrom.left
            anchors.top: eventFrom.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextEventIdentifier + qmltextColon + userList1Model2.eventIdentifier
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: psSerialID
            anchors.left: eventIdentifier.left
            anchors.top: eventIdentifier.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextPsSerialNo + qmltextColon + userList1Model2.eventPsSerial
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: actSerialID
            anchors.left: eventTo.left
            anchors.top: psSerialID.top
            text: qmltextActSerialNo + qmltextColon + userList1Model2.eventActSerial
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: auxSerialID
            anchors.left: psSerialID.left
            anchors.top: psSerialID.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextAuxSerialNo + qmltextColon + userList1Model2.eventAuxSerial
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }

        Text {
            id: commentId
            anchors.left: auxSerialID.left
            anchors.top: auxSerialID.bottom
            anchors.topMargin: 20
            height: 14
            text: qmltextComment + qmltextColon
            font.pixelSize: Style.style3
            color: "#757575"
            font.family: Style.regular.name
        }
        Rectangle{
            anchors.top: commentId.bottom
            anchors.topMargin: 20
            anchors.left: commentId.left
            width: 527
            anchors.bottom: parent.bottom
            Label {
                id: textEdit
                wrapMode: Text.Wrap
                anchors.fill: parent
                text: userList1Model2.eventUsercmt
                font.pixelSize: Style.style3
                color: "#757575"
                font.family: Style.regular.name
            }
        }

        PrimaryButton {
            id: button
            x: 499
            y: 383
            text: qmltextDone
            onClicked:
            {
                eventLogPopupId.close()
                disableMainwindowOpacity.start()
            }
        }
    }
}
