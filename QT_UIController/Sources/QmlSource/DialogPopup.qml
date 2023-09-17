import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import Style 1.0

Rectangle{
    readonly property string qmltextSave: qsTr("SAVE")
    readonly property string qmltextCancel: qsTr("CANCEL")
    readonly property string qmltextEnterComments: qsTr("Please enter your comment")
    readonly property string qmltextMax20Char: qsTr("(MAX 120 Characters)")

    id: quitDialog
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    property alias quitDialog: quitDialog
    property alias quitMessage: dialogDesc.text
    property alias labelName: labelBarId.labelName
    property int checkfocus: 1

    signal save()
    signal cancel()

    /**
    *@breif: To open the dialog popup
    **/
    function open(title, text)
    {
        labelName = qsTr(title)
        quitMessage = text

        quitDialog.visible = true
    }

    /**
    *@breif: To close the dialog popup
    **/
    function close()
    {
        quitDialog.visible = false
    }

    /**
    *@breif: To set the message in the dialog popup
    **/
    function setDialogPopupValue( keyboardInput )
    {
        if(checkfocus == 1)
        {
            quitMessage = keyboardInput
        }
    }

    /**
    *@breif: To handle whenever text changed
    **/
    function handleTextChanged()
    {
        var textdata1 = dialogDesc.text
        textdata1 = dialogDesc.text.replace(",","")
        dialogDesc.text = textdata1
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    Flickable{
        id: dailogPopupFlick
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (20/100) : 0
        clip: true
        Item
        {
            id: popupRect
            width:  parent.width/2 - 200
            height: parent.height/4
            anchors.centerIn: parent

            Rectangle {
                id: roundRect
                width:  popupRect.width
                height: 36
                radius: 10
                color: qmlPrimaryColor
            }

            LabelBars{
                id:labelBarId
                width:  popupRect.width
                height: 30
                anchors.bottom : roundRect.bottom
                anchors.left : roundRect.left
                anchors.right : roundRect.right
                labelName: qmlTextEmpty
            }

            Rectangle{
                id: dialog
                width: popupRect.width
                height: popupRect.height - 50
                color: "#FFFFFF"
                anchors.top: labelBarId.bottom
                anchors.left : labelBarId.left
                anchors.right : labelBarId.right

                TextArea
                {
                    Timer{
                        interval: 200
                        running: true
                        repeat: true
                        onTriggered:
                        {
                            if(dialogDesc.text.length>120)
                            {
                                dialogDesc.remove(121,dialogDesc.length)
                            }
                        }
                    }
                    id: dialogDesc
                    width: 300
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    font.pixelSize: Style.style3
                    font.family: Style.regular.name
                    font.bold: false
                    placeholderText : qmltextEnterComments+qmltextMax20Char
                    wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                    background: Rectangle {
                        id:backGroundId
                        radius: 2
                        color: "#FFFFFF"
                        border.color: qmlPrimaryColor
                        border.width: 1
                    }

                    onTextChanged:
                    {
                        handleTextChanged()
                    }
//                    onPressed: {
//                        var isfocus = dialogDesc.focus
//                        if(isfocus && Qt.inputMethod.visible)
//                            dailogPopupFlick.contentY = dialog.y + dialog.height
//                    }

                    onFocusChanged:
                    {
                        var isfocus = dialogDesc.focus
                        if(isfocus && Qt.inputMethod.visible)
                            dailogPopupFlick.contentY = dialog.y + dialog.height
                    }
                }
                RowLayout {
                    spacing: 30
                    anchors{
                        right: dialog.right
                        rightMargin: 20
                        bottom: dialog.bottom
                        bottomMargin: 20
                    }

                    SecondaryButton {
                        id: cancelButton
                        text:qmltextCancel
                        onClicked: {
                            cancel()
                        }
                    }
                    PrimaryButton {
                        id: saveButton
                        text: qmltextSave
                        onClicked: {
                            save()
                        }
                    }
                }
            }
        }
    }
}
