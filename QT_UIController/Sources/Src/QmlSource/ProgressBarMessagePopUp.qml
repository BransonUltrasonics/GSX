import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0
import QtCharts 2.2
import QtQuick.Window 2.2

Rectangle{
    readonly property string qmlTextCopyIsProcessing :qsTr("DB is copying to USB")

    id: processingMessagePopup
    width: Window.width
    height: Window.height
    color: "#80000000"
    visible: false
    radius: 3
    MouseArea{
        id: processingMessagePopupMA
        anchors.fill: parent
        onClicked: {

        }
    }

    /**
    *@breif: To close ProgressBarMessagePopUp
    **/
    function close()
    {
        processingMessagePopup.visible = false
    }

    /**
    *@breif: To open ProgressBarMessagePopUp
    **/
    function open()
    {
        processingMessagePopup.visible = true
    }

    Item
    {
        id: popupRect
        width:  parent.width/3.8
        height: parent.height/9
        anchors.centerIn: parent
        Rectangle
        {
            anchors.fill: parent
            color: "#FFFFFF"
            radius: 3
            Text
            {
                id:copy_continue_text
                width:  popupRect.width*0.6
                anchors.verticalCenter: parent.verticalCenter
                anchors.left :parent.left
                anchors.leftMargin: 20
                text:qmlTextCopyIsProcessing
                font.pixelSize: Style.style2
                font.family: Style.semibold.name
                color: "#212529"
            }
            BusyIndicator {
                id:copy_indicaor
                anchors.left:copy_continue_text.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                running: true
            }
        }
    }
}
