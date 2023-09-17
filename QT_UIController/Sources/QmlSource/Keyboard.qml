import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import Style 1.0

Popup {

    property alias source: keyboardItem.source
    property alias keyWidth: keyboardItem.keyWidth
    property alias keyHeight: keyboardItem.keyHeight
    property alias bounds: keyboardItem.bounds
    property alias mainFont: keyboardItem.mainFont
    property alias mainFontColor: keyboardItem.mainFontColor
    property alias secondaryFont: keyboardItem.secondaryFont
    property alias secondaryFontColor: keyboardItem.secondaryFontColor
    property alias keyColor: keyboardItem.keyColor
    property alias keyPressedColor: keyboardItem.keyPressedColor
    //property alias editNameText: editName.text
    //property alias inputText: input.text
    property int pageNo: 0

    //id: root
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose|Popup.CloseOnEscape
    dim: true
    //activeFocus: true
    onClosed:
    {
        disableMainwindowOpacity.start()
    }

    function setValue( titleLable,dwPageNo)
    {
        pageNo = dwPageNo
        //editName.text = titleLable
        keyboard.source = "keyboard_us.xml"
        //keyboardItem.allUpperCase=false
        keyboard.focus=true
        //input.focus=true
        //input.text=""
    }

    Rectangle {
        id:keyPadRec
        color: "#ffffff"
        height: parent.height-10
        width: parent.width-10

        radius: 3
        Rectangle{
            anchors.fill: parent
            MouseArea{
                anchors.fill: parent
            }
        }

        Rectangle
        {
            id:keysRec
            x: 0
            y: 2
            height:parent.height
            width: parent.width
            color: "#F8F9FA"
            KeyboardItem
            {
                id: keyboardItem
                anchors.centerIn: keysRec
                source: "keyboard_us.xml"
                keyWidth: parent.height/4 + 20
                keyHeight: parent.height/4
                onKeyClicked:
                {
                    if(key=='\b')
                    {
                        var digits = input.text
                        digits =  digits.toString().slice(0, -1)
                        input.text = digits
                    }
                    else
                        input.text+=key
                }
                onSwitchSource:
                {
                    keyboard.source = source
                }
            }
        }
    }
}
