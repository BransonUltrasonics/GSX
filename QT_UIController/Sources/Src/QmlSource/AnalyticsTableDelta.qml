import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0

Popup {
    readonly property string qmltextChooseParameter : qsTr("Choose Parameter")
    readonly property string qmltextChooseParameterDes :qsTr("Select parameters to graph")
    readonly property string qmltextDone :qsTr("DONE")

    property alias checkbox_rect_param_selection: checkbox_rect_param_selection
    id:checkbox_rect_param_selection
    x: 0
    y: 0
    closePolicy: Popup.NoAutoClose

    Rectangle{

        id:checkbox_rect_param_selectionRec
        color:"#ffffff"
        border.width: 0
        border.color:"#000000"
        radius:3
        GridView {
            id:gridview3
            height:400
            anchors.left: parent.left
            anchors.leftMargin: (50/1920)*window.width
            anchors.right: parent.right
            anchors.rightMargin: (40/1920)*window.width
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            anchors.top:title_choose_param.bottom
            anchors.topMargin: 20

            cellWidth: (260/1920)*window.width
            cellHeight: (80/1080)*window.height
            model: deltaParametersModel
            delegate:Item {
                id: paramCheckBox
                width:gridview3.cellWidth
                height:gridview3.cellHeight-(10/1080)*window.height
                //    property string paramVal : value
                property bool checkBoxEnabled: true
                property bool checkBoxChecked: false
                Component.onCompleted:
                {
                    if(graphtableview.isIndexInList(index))
                    {
                        checkBoxChecked=true;
                    }
                }
                //objectName: name

                onCheckBoxCheckedChanged:
                {
                    graphcheckbox.checkBoxStatus = (checkBoxChecked == true)
                }

                Rectangle {
                    id: rectangle4
                    width: parent.width; height: (50/200)*parent.height
                    color: "#ffffff"
                    CustomCheckBox{
                        id:graphcheckbox
                        width: (34/220)*parent.width
                        height: (30/50)*parent.height
                        anchors.rightMargin: (176/220)*parent.width
                        anchors.leftMargin: (0/220)*parent.width
                        anchors.bottomMargin: (10/50)*parent.height
                        anchors.topMargin: (10/50)*parent.height
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.left
                        checkBoxStatus: (checkBoxChecked == true)
                       // enabled: (checkBoxEnabled == true)
                        checkBoxName: qsTr(model.modelData.name)
                        onCheckBoxStatusChanged:  {
                            //ToDo
                            if(checkBoxStatus){
                                graphtableview.addColumns(index);

                            }
                            if(!checkBoxStatus){
                                graphtableview.removeColumn(index);

                            }
                        }
                    }
                }
            }
        }
        Rectangle{
            id:title_choose_param
            height:(44/1080)*window.height
            width:parent.width
            color:qmlPrimaryColor
            Text {
                id: choose_param
                height: (34/1080)*window.height
                text: qmltextChooseParameter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.Top
                font.family: Style.regular.name
                font.pixelSize: Style.style3
                color:"#ffffff"
            }
        }
//        Text {
//            id: choose_param_description
//            anchors.top:title_choose_param.bottom
//            width: (173/1920)*window.width
//            height: (14/1080)*window.height
//            text:qmltextChooseParameterDes
//            anchors.topMargin: 10
//            anchors.left: parent.left
//            anchors.leftMargin: 20
//            verticalAlignment: Text.AlignVCenter
//            horizontalAlignment: Text.AlignLeft
//            font.pixelSize: Style.style1
//            font.family: Style.semibold.name
//        }
        PrimaryButton {
            id: button
            width:(140/1920)*window.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 50
            text: qmltextDone
            onClicked: {
                //checkbox_rect_param_selection.enabled=false
                //checkbox_rect_param_selection.visible=false
                checkbox_rect_param_selection.close()
                disableMainwindowOpacity.start()
                addColumns();
            }
        }
        Component.onCompleted: {
            enabled= false
            visible=false
        }
    }
}

