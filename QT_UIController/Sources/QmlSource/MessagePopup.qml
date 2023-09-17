import QtQuick 2.0
Rectangle{
    readonly property string qmltextDone :qsTr("DONE")
    readonly property string qmltextCANCEL :qsTr("CANCEL")
    readonly property string qmltextName :qsTr("Name")

    id: quitDialog
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false

    property alias quitMessage: dialogDesc.text
    property alias labelName: labelBarId.labelName
    property alias popupPrimButton: popupPrimButtonId.visible
    property alias popupSecButton: popupSecButtonId.visible
    property alias popupPrimButton2:popupPrimButton2Id.visible
    readonly property string bgColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"
    property int ok_and_cancel_button: 1
    property int ok_button: 2
    property int without_button: 3
    property string error_title: qsTr("Error")
    property string info_title: qsTr("Information")
    property string warning_title: qsTr("Warning")

    property int displayPopupClose: 0
    property int primButtonBoth: 4

    signal onOkay()
    signal onCancel()
    signal onClickPrim2Button()

    MouseArea{
        id: quitDialogMa
        anchors.fill: parent
        onClicked: {
            if(displayPopupClose != 1)
            {
                close()
                disableMainwindowOpacity.start()
            }
        }
    }

    /**
    *@breif: To close MessagePopup
    **/
    function close()
    {
        quitDialog.visible = false
        displayPopupClose = 0;
    }

    /**
    *@breif: To open MessagePopup
    **/
    function open(title, text, status)
    {
        labelName = qsTr(title)
        quitMessage = text

        if (status === ok_and_cancel_button)
        {
            popupPrimButtonId.visible = true
            popupSecButtonId.visible = true
        }
        else if (status === ok_button)
        {
            popupPrimButtonId.visible = true
        }
        else  if (status === primButtonBoth)
        {
            popupPrimButtonId.visible = true
            popupSecButtonId.visible = false
        }
        else
        {
            popupPrimButtonId.visible = false
            popupSecButtonId.visible = false
        }

        quitDialog.visible = true
    }

    /**
    *@breif: To open MessagePopup with both are primary buttons
    **/
    function openSplPurpose(title, text, status,button1Text,button2Text)
    {
        labelName = qsTr(title)
        quitMessage = text

        if (status === ok_and_cancel_button)
        {
            popupPrimButtonId.visible = true
            popupSecButtonId.visible = true
            popupPrimButtonId.text = button1Text
            popupSecButtonId.text = button2Text
            popupPrimButton2Id.visible = false
        }
        else if (status === ok_button)
        {
            popupPrimButtonId.visible = true
            popupPrimButtonId.text = button1Text
        }
        else  if (status === primButtonBoth)
        {
            popupPrimButtonId.visible = true
            popupSecButtonId.visible = false
            popupPrimButton2Id.visible = true
            popupPrimButtonId.text = button1Text
            popupPrimButton2Id.text = button2Text
        }
        else {
            popupPrimButtonId.visible = false
            popupSecButtonId.visible = false
            popupPrimButton2Id.visible = false
        }

        quitDialog.visible = true
    }

    Item
    {
        id: popupRect
        width:  parent.width/2
        height: parent.height/8
        anchors.centerIn: parent

        Rectangle {
            id: roundRect
            width:  popupRect.width
            height: 36
            radius: 10
            color: bgColor
        }
        LabelBars{
            id:labelBarId
            width:  popupRect.width
            height: 30
            anchors.bottom : roundRect.bottom
            anchors.left : roundRect.left
            anchors.right : roundRect.right
            labelName: ""
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
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20
                font.pixelSize:14
                width: parent.width - 40
                wrapMode: Text.WordWrap
            }
            PrimaryButton {
                id:popupPrimButtonId
                anchors.right: parent.right
                anchors.rightMargin:20
                anchors.top: dialogDesc.top
                anchors.topMargin: (popupRect.height)- 80
                text: qmltextDone
                visible: false
                onClicked: {
                    onOkay()
                    close()
                }
            }
            PrimaryButton {
                id:popupPrimButton2Id
                anchors.right: parent.right
                anchors.rightMargin:170
                anchors.top: dialogDesc.top
                anchors.topMargin: (popupRect.height)-80
                text: qmltextName
                visible: false
                onClicked: {
                    onClickPrim2Button()
                    close()
                }
            }
            SecondaryButton {
                id:popupSecButtonId
                anchors.right: parent.right
                anchors.rightMargin:170
                anchors.top: dialogDesc.top
                anchors.topMargin: (popupRect.height)-80
                text: qmltextCANCEL
                visible: false
                onClicked: {
                    onCancel()
                    close()
                }
            }
        }
    }
}
