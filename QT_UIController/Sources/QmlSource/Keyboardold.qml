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
    property alias editNameText: editName.text
    property alias inputText: input.text
    property int pageNo: 0

    //id: root
    x: 0
    y: 0
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
        editName.text = titleLable
        keyboard.source = "keyboard_us.xml"
        //keyboardItem.allUpperCase=false
        keyboard.focus=true
        input.focus=true
        input.text=""
    }

    Rectangle {
        id:keyPadRec
        color: "#ffffff"
        height: 301 //parent.height-10
        width: 660-220//parent.width-10

        radius: 3
        Rectangle{
            anchors.fill: parent
            MouseArea{
                anchors.fill: parent
            }
        }

        Rectangle
        {
            x: 0
            y: 0
            height: 25
            width: parent.width
            color:qmlPrimaryColor
            radius: 3
            Text
            {
                id: editName
                x: 10
                text: ""//qsTr("User Name")
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                color:"#F8F9FA"
            }
            Image {
                id: closekeyboard
                sourceSize.width: parent.width*(10/100)
                sourceSize.height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                /*Image icon update based on System Type*/
                source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
            }
            MouseArea{
                id: closeImageMA
                width: parent.width*(10/100)
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                onClicked:
                {
                    keyboard.close()
                    disableMainwindowOpacity.start()
                }
            }
        }
        TextField {
            id: input
            font.pixelSize: Style.style2
            font.family:Style.semibold.name
            width: 394
            x:25//0//131
            y:44
            height:35
            maximumLength: 16
            focus: true
            style: TextFieldStyle
            {
                textColor: qmlPrimaryColor
                background: Rectangle
                {
                    radius: 2
                    border.color: qmlPrimaryColor
                    border.width: 1
                }
            }
        }

        Rectangle
        {
            id:keysRec
            x: 0//108
            y: 93
            height:200
            width: 440
            color: "#F8F9FA"
            KeyboardItem
            {
                id: keyboardItem
                anchors.centerIn: keysRec
                source: "keyboard_us.xml"

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
