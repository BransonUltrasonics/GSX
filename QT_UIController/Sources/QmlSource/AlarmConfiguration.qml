import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4

Item {

    readonly property string qmltextResetDefaults : qsTr("RESET TO DEFAULTS")
    readonly property string qmltextSave : qsTr("SAVE")
    readonly property string qmltextCancel : qsTr("CANCEL")
    readonly property string qmltextSaveSuccessMsg : qsTr("Alarm configuration saved successfully")
    readonly property string qmltextSaveFailedMsg : qsTr("Failed to save alarm configuration")

    /**
      *@breif: Save the changes with the respective alarm and its functionality
      **/
    function updatedStatus(index,checkedstatus)
    {
        alarmconfig.saveAlarmconfigDetails(index,checkedstatus)
    }
    id:alarmconfiguration
    x:(400/1920)*window.width
    width:window.width*(1520/1920)
    height:(995/1080)*window.height
    GridView {
        id: gridView1
        x: parent.width/10+gridView2.width+50
        y: 61
        width: parent.width/1.6
        height: parent.height/10
        model: modelAlarmconfigcolumns
        boundsBehavior: Flickable.StopAtBounds
        cellHeight: gridView1.height
        cellWidth: gridView1.width/5
        delegate: Item {
            Column {
                Text {
                    text:model.modelData.ParameterName
                    font.pixelSize: Style.style3
                }
            }
        }
    }
    GridView {
        id: gridView2
        x: parent.width/10
        y: 160
        width: parent.width/8
        height: parent.height/1.7
        model: modelAlarmconfigrows
        boundsBehavior: Flickable.StopAtBounds
        cellHeight: gridView2.width/2.7
        delegate: Item {
            Row {
                spacing: 200
                Rectangle {
                    width: gridView2.width
                    height: gridView2.height/8
                    Text {
                        text: model.modelData.RowName
                        anchors.right: parent.right
                        font.pixelSize: Style.style3
                    }
                }
            }
        }
    }
    Rectangle {
        id:gridview3
        x:parent.width/10+gridView2.width+50
        y:gridView1.height+10
        width:parent.width/1.6
        anchors.top:gridView1.bottom
        height: parent.height/1.7
        Grid {
            x: 0; y: 0
            rows: 8; columns: 3;
            anchors.fill: parent
            Repeater {
                model: modelCheckBoxStatus
                Rectangle
                {
                    width: gridview3.width/5
                    height: gridView2.width/2.7
                    CheckBox
                    {
                        id:checkbox
                        width: 30
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        checked : model.modelData.CheckboxStatus
                        onCheckStateChanged:
                        {
                            updatedStatus(index,checked)
                        }
                    }
                }
            }
        }
    }
    SecondaryButton{
        id:defalutbutton
        text:qmltextResetDefaults
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 100
        onClicked:
        {
            alarmconfig.resetDefaultValues()
        }
    }
    RowLayout{
        spacing: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 70
        anchors.right: parent.right
        anchors.rightMargin: 30
        x:parent.width/10+gridView2.width+50
        SecondaryButton{
            id:cancelbutton
            text:qmltextCancel
            onClicked:
            {
                alarmconfig.cancelCurrentDetails();
            }
        }
        PrimaryButton{
            id:savebutton
            text:qmltextSave
            onClicked:
            {
                savebutton.enabled = false
                Qt.callLater(callBackBtnProcess)
            }

            onPressed: {
                savebutton.buttonColor = Style.grayColor
                savebutton.textColor =  Style.blackColor
            }

            function callBackBtnProcess()
            {
                var status;
                status=alarmconfig.saveToBL()
                if(status)
                {
                    recipe.updateStatusText(qmltextSaveSuccessMsg)
                }
                else
                {
                    recipe.updateStatusText(qmltextSaveFailedMsg)
                }
                savebutton.buttonColor = qmlPrimaryColor
                savebutton.textColor = Style.whiteColor
                savebutton.enabled = true
            }

        }
    }
}


