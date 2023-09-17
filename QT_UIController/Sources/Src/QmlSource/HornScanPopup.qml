import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
import QtQuick.Layouts 1.3
Popup {
    readonly property string qmltextHORNSCANPopUpMsg:qsTr("Horn Scan is in progress. Do you want to abort or continue ?")
    readonly property string qmltextAbort:qsTr("Abort")
    readonly property string qmltextContinue:qsTr("Continue")
    readonly property string qmltextInformation:qsTr("Information")

    property int layoutSpacing:10
    readonly property string titleColor: "#FFFFFF"
    id:hornScanPopup
    closePolicy: Popup.NoAutoClose
    modal: true
    focus: true
    dim: true

    Rectangle
    {
        id: popupRect
        width:  window.width/2
        height: window.height/8
        anchors.centerIn: parent

        LabelBars{
            id:labelBarId
            width:  popupRect.width
            height: 30
            anchors.top : parent.top
            anchors.left : parent.left
            labelName: qmltextInformation
        }

        Rectangle{
            id: dialog
            width: popupRect.width
            height: popupRect.height
            color: titleColor
            anchors.top: labelBarId.bottom
            anchors.left : labelBarId.left
            anchors.right : labelBarId.right

            Text{
                id: dialogDesc
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20
                font.pixelSize:14
                width: parent.width - 40
                wrapMode: Text.WordWrap
                text: qmltextHORNSCANPopUpMsg
            }
            RowLayout
            {
                spacing: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 20
                PrimaryButton {
                    id: okButton
                    text: qmltextAbort
                    onClicked:
                    {
                        /* Simple logic will be implemented directly with the Business Logic Communication */
                        loaderelement.item.abortScanFromPopup();
                        hornPopupClose();
                    }
                }
                SecondaryButton {
                    id: cancelButton
                    text: qmltextContinue
                    onClicked: {
                        hornPopupClose();
                    }
                }
            }
        }
    }


    /**
    *@breif: To close horn Popup
    **/
    function hornPopupClose()
    {
        disableMainwindowOpacity.start();
        hornScanPopup.close();
    }


    /**
    *@breif: To open horn Popup
    **/
    function hornPopupOpen()
    {
        enableMainwindowOpacity.start();
        hornScanPopup.open();
    }
}
