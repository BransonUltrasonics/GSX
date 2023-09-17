import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import Style 1.0


Item {
    readonly property string qmltextIpConfig: qsTr("IP Configuration")
    readonly property string qmltextCancel:qsTr("CANCEL")
    readonly property string qmltextSave:qsTr("SAVE")
    readonly property string qmltextSaveSuccess:qsTr("IP configuration saved successfully")

    Rectangle
    {
        id: id_TitleRect
        width: parent.width
        height: 60
        color: "#E9ECEF"
        anchors.topMargin: 0
        Text
        {
            id: id_TitleTxt
            text: qmltextIpConfig
            color: qmlPrimaryColor
            font.bold: true
            font.pixelSize: Style.style7
            font.family:  Style.semibold.name
            anchors.centerIn: id_TitleRect
        }
    }
    Rectangle
    {
        id: id_parent_Column
        color:"#E9ECEF"
        width: parent.width/2
        height: parent.height * (42/100)
        anchors.top: id_TitleRect.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: parent.width * (20/100)
        border.width: 1
        border.color: "#ECECEC"

        DropShadow  {
            anchors.fill: id_MainCol
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: id_MainCol
        }
        Column
        {
            id: id_MainCol
            spacing: 10
            width: id_parent_Column.width - 20
            y: 20
            leftPadding: 10
            Repeater
            {
                model: [qsTr("IP address :"), qsTr("Subnet mask :"),qsTr("Default gateway :")]

                Rectangle
                {
                    property alias ipAddressText: id_IP_Config_txtInput.text
                    id: id_handler
                    width: id_MainCol.width
                    height: id_TitleRect.height

                    Text
                    {
                        id: id_IP_Config_Lbl
                        width: id_handler.width/2
                        height: id_handler.height
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 20
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style5
                        text: modelData

                    }

                    Rectangle
                    {
                        id:id_IP_Config_txtInput_Rect
                        width: id_handler.width/2
                        height: id_handler.height
                        anchors.left: id_IP_Config_Lbl.right
                        border.width: 1
                        border.color: "#757575"

                        LineEdit
                        {
                            id: id_IP_Config_txtInput
                            width: id_IP_Config_txtInput_Rect.width
                            height: id_IP_Config_txtInput_Rect.height
                            //validator: IntValidator{bottom: 1}
                            focus: false
                            //inputMask: "000.000.000.000;"
                           // color: "#757575"
                           // selectionColor: qmlPrimaryColor
                           // selectedTextColor: "#ffffff"
                            selectByMouse: true
                           // font.bold: true
                          //  font.pixelSize: Style.style5
                          //  font.family:  Style.semibold.name
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: RegExpValidator
                            {
                                regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                            }
                            onTextChanged:
                            {
                                ipConfigObj.saveChangedValue(ipAddressText,index)
                            }
                            Component.onCompleted:
                            {
                                if(index == 0)
                                    id_IP_Config_txtInput.text = ipConfigObj.getIP()
                                else if(index == 1)
                                    id_IP_Config_txtInput.text = ipConfigObj.getSubnetMask()
                                else
                                    id_IP_Config_txtInput.text = ipConfigObj.getGatewayAddr()
                            }
                        }
                    }
                }
            }
        }

        Row
        {
            id: id_ApplyCancel_layout
            width: parent.width
            height: 60
            anchors.top :id_MainCol.bottom
            anchors.topMargin: 30
            spacing: 20
            leftPadding: parent.width - 320

            SecondaryButton
            {
                id:  id_cancelBtn
                text: qmltextCancel
                onClicked:
                {

                }
            }
            PrimaryButton
            {
                id: id_OkBtn
                text: qmltextSave
                onClicked:
                {
                    ipConfigObj.saveIPv4Details()
                    recipe.updateStatusText(qmltextSaveSuccess)
                }
            }
        }

    }
}





