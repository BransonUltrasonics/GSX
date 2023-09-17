import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

Item {
    readonly property string qmltextCurrentStackInfo: qsTr("Current Stack Information")
    readonly property string qmltextNewStackInfo: qsTr("   New Stack Information   ")
    readonly property string qmltextChangeStack: qsTr("CHANGE STACK")
    readonly property string qmltextSave: qsTr("SAVE")
    readonly property string qmltextChangeStackMsg: qsTr("You have clicked on \"CHANGE STACK\" button. Scan the Barcode of new Converter!")
    property string paramColor: "#ECECEC"
    property string paramTextColor: "#898d96"
    id:smartStackId

    ListModel{
        id:model
        ListElement{
            name:"recipe"
        }
    }

    Rectangle {
        id: smartStackRec
        width:parent.width * (9.99/10)
        height: parent.height * (9.1/10)
        RowLayout{
            id:topButtonsId
            anchors.left: parent.left
            anchors.leftMargin: 320
            anchors.top: parent.top
            anchors.topMargin: 60
            spacing: 60
            SecondaryButton{
                id:currentStackInfoButton
                text: qmltextCurrentStackInfo
                enabled: false
            }
            SecondaryButton{
                id:newStackInfoButton
                text: qmltextNewStackInfo
                enabled: false
            }
        }
        Rectangle{
            id:unitRec
            width: parent.width - centerRec.width
            anchors.top:topButtonsId.bottom
            anchors.topMargin: 10
            anchors.bottom: downButtonsId.top
            anchors.bottomMargin: 10
            anchors.left: centerRec.right
            color: "#ffffff"
            DropShadow{
                source: rectangle1
                anchors.fill: rectangle1
                horizontalOffset: 1
                verticalOffset: 1
                color: "#000000"
                opacity: 0.2
                radius: 3
            }
            Rectangle{
                id:rectangle1
                width: 30
                height: 30
                radius: 3
                color:paramColor
                anchors.top:parent.top
                anchors.topMargin: parent.height/2 - 95
                Text {
                    id: paramUnit1
                    text: "%"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Style.style2
                    font.family: Style.semibold.name
                    color: paramTextColor
                }
            }
            DropShadow{
                source: rectangle2
                anchors.fill: rectangle2
                horizontalOffset: 1
                verticalOffset: 1
                color: "#000000"
                opacity: 0.2
                radius: 3
            }
            Rectangle{
                id:rectangle2
                width: 30
                height: 30
                radius: 3
                color:paramColor
                anchors.top:rectangle1.bottom
                anchors.topMargin: 25
                Text {
                    id: paramUnit2
                    text: "V"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Style.style2
                    font.family: Style.semibold.name
                    color: paramTextColor
                }
            }
        }

        Rectangle{
            id:centerRec
            anchors.top:topButtonsId.bottom
            anchors.topMargin: 10
            anchors.bottom: downButtonsId.top
            anchors.bottomMargin: 10
            width: smartStackRec.width*(8/10)
            color: "#ffffff"
            Rectangle{
                id:frstLVRec
                width: parent.width/3
                height: parent.height
                color: "#ffffff"
                ListView{
                    id:list1
                    width: parent.width
                    height: parent.height
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.top:parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    spacing: 20
                    cacheBuffer: 0
                    model:fixedStackModel
                    delegate: SmartStackDelegate
                    {
                        width:parent.width - 80
                        height: 40
                        parameterName: model.modelData.ParameterName
                    }
                }
            }
            Rectangle{
                id:scndLVRec
                width: parent.width/3
                height: parent.height
                color: "#ffffff"
                anchors.left: frstLVRec.right
                ListView{
                    id:list2
                    width: parent.width
                    height: parent.height
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.top:parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    spacing: 20
                    cacheBuffer: 0
                    model:CurrentStackModel
                    delegate: SmartStackDelegate
                    {
                        width:parent.width - 80
                        height: 40
                        parameterName:  model.modelData.ParameterName
                    }
                }
            }
            Rectangle{
                id:thrdLVRec
                width: parent.width/3
                height: parent.height
                color: "#ffffff"
                anchors.left: scndLVRec.right
                ListView{
                    id:list3
                    width: parent.width
                    height: parent.height
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.top:parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    spacing: 20
                    cacheBuffer: 0
                    model:NewStackModel
                    delegate: SmartStackDelegate
                    {
                        width:parent.width - 80
                        height: 40
                        parameterName: model.modelData.ParameterName
                    }
                }
            }
        }

        RowLayout{
            id:downButtonsId
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 50
            spacing: 50
            SecondaryButton{
                id:changeStackButton
                text: qmltextChangeStack
                onClicked:
                {
                    //displayQuitPopup(messagePopup.warning_title,qmltextChangeStackMsg)
                    recipe.updateStatusText(qmltextChangeStackMsg)
                    obj_smartStack.sendChangeStackReqToSC()
                }
            }
            PrimaryButton{
                id:saveButton
                text: qmltextSave
                onClicked: {
                }
            }
        }
    }
}
