import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import Style 1.0

Item {
    //id: root
    property string source
    property int keyWidth: 40
    property int keyHeight: 40
    property int bounds: 5
    property alias mainFont: proxyMainTextItem.font
    property alias mainFontColor: proxyMainTextItem.color
    property alias secondaryFont: proxySecondaryTextItem.font
    property alias secondaryFontColor: proxySecondaryTextItem.color
    property color keyColor: qmlPrimaryColor
    property color keyPressedColor: qmlSecondaryColor

    property int xmlIndex: 1

    property bool allUpperCase: false

    signal keyClicked(string key)
    signal switchSource(string source)
    signal enterClicked()

    Text {
        id: proxyMainTextItem
        color: "#F8F9FA"
        font.pixelSize: Style.style4
        font.family: Style.semibold.name
        font.capitalization: keyboardItem.allUpperCase ? Font.AllUppercase :
                                                 Font.MixedCase
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: proxySecondaryTextItem
        color: "#BEC1C3"
        font.pixelSize: Style.style4
        font.family: Style.semibold.name
        font.capitalization: keyboardItem.allUpperCase ? Font.AllUppercase :
                                                 Font.MixedCase
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Column {
        id: column
        anchors.centerIn: parent

        Repeater {
            id: rowRepeater
            model: XmlListModel
            {
                source: keyboardItem.source
                query: "/Keyboard/Row"
            }

            Row {
                id: keyRow
                property int rowIndex: index
                anchors.horizontalCenter: if(parent) parent.horizontalCenter

                Repeater {
                    id: keyRepeater

                    model: XmlListModel {
                        source: keyboardItem.source
                        query: "/Keyboard/Row[" + (rowIndex + 1) + "]/Key"

                        XmlRole { name: qsTr("labels"); query: "@labels/string()" }
                        XmlRole { name: qsTr("ratio"); query: "@ratio/number()" }
                        XmlRole { name: qsTr("icon"); query: "@icon/string()" }
                        XmlRole { name: qsTr("checkable"); query: "@checkable/string()" }
                    }

                    Key {
                        id: key
                        width: keyWidth * ratio
                        height: keyHeight
                        iconSource: icon
                        mainFont: proxyMainTextItem.font
                        mainFontColor: proxyMainTextItem.color
                        secondaryFont: proxySecondaryTextItem.font
                        secondaryFontColor: proxySecondaryTextItem.color
                        keyColor: keyboardItem.keyColor
                        keyPressedColor: keyboardItem.keyPressedColor
                        bounds: keyboardItem.bounds
                        isChekable: checkable
                        isChecked: isChekable &&
                                   command &&
                                   command === "shift" &&
                                   allUpperCase

                        property var command
                        property var params: labels

                        onParamsChanged: {
                            var labelSplit = params.split(/[|]+/)

                            mainLabel = params.split(/[!|]+/)[0].toString();
                            if (labelSplit[1]) secondaryLabels = labelSplit[1];
                            command = params.split(/[!]+/)[1];
                        }

                        onClicked: {
                            if (command)
                            {
                                var commandList = command.split(":");

                                switch(commandList[0])
                                {
                                    case "source":
                                        keyboardItem.switchSource(commandList[1])
                                        return;
                                    case "shift":
                                        keyboardItem.allUpperCase = !keyboardItem.allUpperCase
                                        return;
                                    case "backspace":
                                        keyboardItem.keyClicked('\b');
                                        return;
                                    case "enter":
                                        keyboardItem.emitEnterClicked();
                                        return;
                                    case "tab":
                                        keyboardItem.keyClicked('\t');
                                        return;
                                    default: return;
                                }
                            }
                            if (mainLabel.length === 1)
                                keyboardItem.emitKeyClicked(mainLabel);
                        }
                        onAlternatesClicked: keyboardItem.emitKeyClicked(symbol);
                    }
                }
            }
        }
    }

    function emitKeyClicked(text) {
        keyClicked( allUpperCase ? text.toUpperCase() : text);
    }
    function emitEnterClicked()
    {
        switch(keyboard.pageNo)
        {
        case 1:
            loadmain.item.sendUsername(inputText)
            break;
        case 2:
            loaderelement.item.setUserMngKeyValue(inputText)
            break;
        case 3:
            loaderelement.item.setTextFieldValue(inputText)
            break;
        default:
            break;
        }
        inputText = ""
        disableMainwindowOpacity.start()
        keyboard.close()
        keyboard.pageNo=0
    }
}
