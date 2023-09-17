import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import com.branson.menu 1.0
import QtQuick.Window 2.2
import com.branson.eventlogging 1.0
import QtQuick 2.6
import QtQuick.Controls 1.4 as OldControls

Item {
    id:mainItem
    property var serviceNoteText: ""
    property var serviceFinalText: ""

    readonly property string qmltextCancel:  qsTr("CANCEL")
    readonly property string qmltextSave:  qsTr("SAVE")
    readonly property string qmltextServiceNotes:  qsTr("Service Notes")
    readonly property string qmltextColon:  qsTr(" : ")

    readonly property string qmltextInformation :qsTr("INFORMATION")

    readonly property string qmltextBoardReplaced: "Board Replaced"
    property var serviceBoardReplace: sysconfig.getServiceSwitchStatus(qmltextBoardReplaced)
    property DialogPopup dialogPopup
    property string emptyString: ""
    readonly property string qmltextCommtReqMsg:qsTr("Comment is required")
    readonly property string qmlTextBoardisreplaced:qsTr("Board is replaced")

    property alias service: mainItem

    function loadSystemInfo()
    {
        userList1Model.getEventRecords()
        loaderelement.setSource("InformationLabPanel.qml",{"width":(400/1920)*window.width,"height":window.height})
        titlebarchild.visible=true
        titlebarchildtext.text=qmltextInformation
        titlbaronclicked="System.qml"
    }

    function createDialogpopup(message)
    {
        var comment = qmlTextEmpty
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(mainwindow)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === qmlTextEmpty)
            {
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_HW_CHANGED, comment)
                sysconfig.resetServiceSwitchStatus()
             }
            dialogPopup.close()
            dialogPopup.onSave.disconnect(onOkay)
            dialogPopup.destroy()
        })
        dialogPopup.onCancel.connect(function onCancel()
        {
            dialogPopup.close()
            dialogPopup.onCancel.disconnect(onCancel)
            dialogPopup.destroy()
        })
    }

    Flickable{
        id: verticalFlick
        width: parent.width
        height: parent.height * 0.7
        flickableDirection: Flickable.VerticalFlick
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
        clip: true
        DropShadow{
            anchors.fill: rect1
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: rect1
        }
        Rectangle{
            id:rect1
            width: parent.width - 30
            height: parent.height//parent.height
            color:"#ECECEC"
            Rectangle {
                id: centerRect
                anchors.centerIn: parent
                width: 20
                height: parent.height
                color: "#ffffff"
                Rectangle {
                    id: centerLine
                    anchors.centerIn: parent
                    width: 1
                    height: parent.height
                    color: "#000000"
                }
            }
            ListView
            {
                id: list_view
                width: (parent.width/2)-40
                height: parent.height - 20
                anchors.left:rect1.left
                anchors.leftMargin: 30
                anchors.top:rect1.top
                anchors.topMargin: 10
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickDirection
                clip: true
                spacing: 10
                cacheBuffer :3000
                model:sysconfig.bransonSecondLevelFlag ? serviceRightParamModel : 0
                property bool refreshSwitch: false
                delegate: SystemConfigGeneralItem
                {
                    id:delRec
                    width:list_view.width - 40
                    height:model.modelData.ParameterName == qmltextServiceNotes ? 130:60
                    paramText:model.modelData.ParameterName
                    isText:model.modelData.ParameterText
                    isButton:model.modelData.PrimaryButton
                    isServiceNote:model.modelData.ParameterTextEdit
                    isToggleSwitch:model.modelData.ParameterSwitch
                    checkStatus:model.modelData.ParameterSwitchState

                    textArea_serviceNote.onTextChanged:
                    {
                        serviceNoteText = textArea_serviceNote.text
                    }
                    textArea_serviceNote.onFocusChanged:
                    {
                        var isfocus = textArea_serviceNote.focus
                        if(isfocus)
                            verticalFlick.contentY = textArea_serviceNote.height*3 + textArea_serviceNote.y
                    }
                    toggleSwitch.onPressedChanged:
                    {
                        list_view.refreshSwitch = true
                    }

                    toggleSwitch.onCheckedChanged:
                    {
                        if(list_view.refreshSwitch == true)
                        {
                            list_view.refreshSwitch = false
                            sysconfig.setServiceSwitchStatus(index,toggleSwitch.checked)
                        }
                    }

                    Component.onCompleted:
                    {
                        if((paramText === qmltextServiceNotes) )
                        {
                            param_text.parent.height = 120
                            serviceNoteTextArea.height = 100
                        }
                        if(isButton)
                        {
                            paramButton.text = model.modelData.ParamButtonText
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar{}
            }
            ListView
            {
                id: texteditlistView
                width: (parent.width/2)-70
                height: parent.height-20
                anchors.right:rect1.right
                anchors.rightMargin: 30
                anchors.top:rect1.top
                anchors.topMargin: 20
                spacing: 10
                clip: true
                model:sysconfig.bransonSecondLevelFlag ? serviceNotesModel : 0
                delegate: Item
                {
                    id:recItem
                    width: texteditlistView.width - 15
                    height: texteditlistView.height/4.5
                    DropShadow{
                        anchors.fill: serviceNoteTextArea2
                        horizontalOffset: 1
                        verticalOffset: 1
                        color: "#000000"
                        opacity: 0.5
                        radius: 3
                        source: serviceNoteTextArea2
                    }
                    Rectangle
                    {
                        id:serviceNoteTextArea2
                        anchors.fill: parent
                        Flickable {
                            id: flickable2
                            anchors.fill: parent
                            TextArea.flickable:TextArea {
                                id: textArea_recipe_description
                                focus: false
                                wrapMode: Text.WordWrap
                                text:model.modelData.ParameterName
                                readOnly: true
                                font.family: Style.regular.name
                                font.pixelSize: Style.style3
                                background:Rectangle {
                                    anchors.fill: parent
                                }                                
                            }
                            ScrollBar.vertical: ScrollBar { }
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar{}
                Component.onCompleted:
                {
                    sysconfig.updateServiceNotesFromFile()
                }
            }
        }
    }
    RowLayout{
        id:buttonslayout
        width: parent.width/4
        height:80
        anchors.top: verticalFlick.bottom
        anchors.topMargin: 40
        anchors.right:parent.right
        anchors.rightMargin: 50
        spacing: 10
        SecondaryButton {
            id: cancelRect
            text: qmltextCancel
            onClicked:
            {
                sysconfig.resetServiceSwitchStatus()
            }
        }
        PrimaryButton {
            id: saveRect
            text: qmltextSave
            onClicked: {

                //console.log("SAVE serviceBoardReplace " + serviceBoardReplace)
                //console.log("SAVE sysconfig.getServiceSwitchStatus(qmltextBoardReplaced) " + sysconfig.getServiceSwitchStatus(qmltextBoardReplaced))


                if(serviceBoardReplace!==sysconfig.getServiceSwitchStatus(qmltextBoardReplaced))
                {
                    createDialogpopup(qmlTextBoardisreplaced)
                }
                else
                {
                    sysconfig.saveServiceData(serviceNoteText)
                }

                if(serviceNoteText.length >= 1)
                {
                    serviceFinalText =  mainwindow.usernameTextTitleBar + qmltextColon +Qt.formatDate(sysconfig.TitleBarDate,"yyyy/MM/dd").toString() + "  " + Qt.formatTime(sysconfig.TitleBarDate,"hh:mm:ss").toString() + " : " +serviceNoteText
                    sysconfig.createServiceNotesList(serviceFinalText)
                    serviceNoteText = qmlTextEmpty
                }
                recipe.updateStatusText(qsTr("Saved successfully"))


            }
        }
    }
}
