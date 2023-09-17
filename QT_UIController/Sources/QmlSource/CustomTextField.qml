import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4
import Style 1.0

TextField {
    id: customTextField
    property string bgColor: "#BEC1C3"
    property alias readOnlyMode: customTextField.readOnly
    property alias customTFValidator: customTextField.validator
    property alias placehodertext: customTextField.placeholderText
    property string textFieldColor: qmlPrimaryColor
    property var valid1 : RegExpValidator
    {
        regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
    }
    property bool onlyForNumPad: false
    property int fontSize: Style.style1

    /**
       *@breif: To set the background color
       **/
    function setBackgroundColor(fontColor)
    {
        bgColor = fontColor
    }

    /**
       *@breif: To set the text color
       **/
    function setTextColor(fontColor)
    {
        textFieldColor = fontColor
    }
    signal click()
    signal singleClick()
    signal doubleClick()
    placeholderText : ""
    horizontalAlignment: Text.AlignLeft
    readOnly: false
    style: TextFieldStyle {
        textColor: textFieldColor//qmlPrimaryColor
        background: Rectangle {
            anchors.fill: parent
            border.color:qmlPrimaryColor
            color: bgColor
            border.width: 1
        }
    }
    verticalAlignment: Text.AlignVCenter
    font.family:Style.regular.name
    font.pixelSize: fontSize
    validator: RegExpValidator
    {
        regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
    }
    Component.onCompleted:
    {
        Qt.inputMethod.hide();
    }

    focus: false
    MouseArea {
        anchors.fill: parent
        onClicked:
        {
            click()
            if(parent.onlyForNumPad == true)
            {
                parent.selectAll()
                singleClick()
            }
            parent.focus = true;
            Qt.inputMethod.hide()
        }

        onDoubleClicked:
        {
            if(parent.onlyForNumPad == true)
            {
                parent.deselect()
                doubleClick()
                parent.focus = true
                Qt.inputMethod.hide()
            }
        }
    }
}
