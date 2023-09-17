/*
 * Popups.qml
 *
 *  Created on: Nov 02, 2017
 */

import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0

Popup {
    id: recipeInformation
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    dim: true

    function fillRecipeDetails(recipeNumber){
        recipeName_Text1.text = (recipe.getModifiedStatus(recipeNumber)?"* ":"" ) + recipeNumber+" : "+recipeinfo.getInfoRecipeName()
        info_Company.text = recipeinfo.getCompany()
        info_Control1.text = recipeinfo.getControlmode() + ":"
        info_Controlvalue.text = recipeinfo.getControlmodeValue() + " " + recipeinfo.getControlModeUnit()
        productionIns1.text = recipeinfo.getProductionInstruction()
        recipedesc1.text = recipeinfo.getRecipeDescription()
    }

    MouseArea{
        id:recipemouse
        x:popupRec.x
        y:popupRec.y
        width: loaderelementwidth*(70/100)
        height: loaderelementheight*(86/100)
    }
    Rectangle {
        id: popupRec
        width: window.width*(70/100)
        height: window.height*(70/100)
        color: "#757575"
        border.width: 2
        border.color:"#757575"
        Column{
            id:recipeinfoCol
            width: parent.width
            height: parent.height
            spacing: 2
            Rectangle {
                id: titleRec
                x: 0
                y: 0
                width: parent.width
                height: parent.height*(9/100)
                color: "#ffffff"
                border.width: 2
                border.color: "#ffffff"
                Text {
                    id: recipeTitle
                    text: qsTr("RECIPE INFORMATION")
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                    font.pixelSize: Style.style5
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(2/100)
                    color: "#757575"
                    font.family: Style.regular.name
                }

                Image {
                    id: image
                    sourceSize.width: parent.width*(7/100)
                    sourceSize.height: parent.height*(75/100)
                    anchors.right: parent.right
                    anchors.rightMargin: parent.width*(1/100)
                    anchors.verticalCenter: parent.verticalCenter
                    /*Image icon update based on System Type*/
                    source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
                }
                MouseArea {
                    id: mouseArea
                    width: parent.width*(4/100)
                    height: parent.height*(90/100)
                    x: image.x
                    y: image.y
                    anchors.right: parent.right
                    anchors.rightMargin: parent.width*(1/100)
                    onClicked: {
                        recipeInformation.close()
                        disableMainwindowOpacity.start()
                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                    z:1
                }
            }
            Rectangle{

                id: belowTitleRec
                width: parent.width
                height: parent.height-titleRec.height
                color: "#757575"
                x:0
                y:titleRec.height
                Row{
                    id:recipeinfoRow
                    width: parent.width
                    height: parent.height
                    spacing: 2
                    Rectangle{
                        id: activeRecipeRec
                        x:0
                        y:0
                        width: belowTitleRec.width*(38/100)
                        height: belowTitleRec.height
                        color: "#f8f9fa"
                        radius: 0
                        border.color: "#e9ecef"
                        border.width: 1

                        Rectangle {
                            id: aR_Rect1
                            width: activeRecipeRec.width
                            height: activeRecipeRec.height*(3/10)
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            anchors.top: parent.top
                            anchors.topMargin: 0
                            border.color: "#e9ecef"
                            color: "#e9ecef"
                            Image {
                                id: object
                                width: parent.width*(19/100)
                                height: parent.height*(45/100)
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/images/image/part_icon_u135.png"
                            }
                        }
                        Rectangle {
                            id: aR_Rect2
                            width: activeRecipeRec.width
                            height: activeRecipeRec.height*(1/10)
                            anchors.top: aR_Rect1.bottom
                            anchors.topMargin: 0
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            color: qmlPrimaryColor
                            border.color: qmlPrimaryColor
                            border.width: 3
                            Text {
                                id: recipeName_Text1
                                color: "#ffffff"
                                text: qsTr(" ")
                                anchors.topMargin: 0
                                anchors.top: parent.top
                                anchors.verticalCenter:  parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: parent.width*(7/100)
                                font.family: Style.semibold.name
                                font.bold: true
                                font.pixelSize: Style.style5
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        Text {
                            id: info_Company
                            color: "#757575"
                            text: qsTr(" ")
                            anchors.top: aR_Rect2.bottom
                            anchors.topMargin: parent.height*(12/100)
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(7/100)
                            font.pixelSize: Style.style3
                            font.family: Style.regular.name
                        }
                        Text {
                            id: info_Control1
                            anchors.top: info_Company.bottom
                            anchors.topMargin:  parent.height*(4/100)
                            anchors.left: parent.left
                            anchors.leftMargin:  parent.width*(7/100)
                            color: "#757575"
                            text: ""
                            font.family: Style.regular.name
                            font.pixelSize: Style.style1
                        }
                        Text {
                            id:info_Controlvalue
                            anchors.top: info_Company.bottom
                            anchors.topMargin:  parent.height*(3.8/100)
                            anchors.left: info_Control1.right
                            anchors.leftMargin:  parent.width*(2/100)
                            color: "#757575"
                            text: ""
                            font.bold: true
                            font.family: Style.regular.name
                            font.pixelSize: Style.style3
                        }
                        Image {
                            id: info_Lock_Image
                            sourceSize.width: parent.width*(7/100)
                            sourceSize.height: parent.height*(7/100)
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(7/100)
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height*(2.5/100)
                            source: "qrc:/images/image/lock_ash.svg"
                            visible: false
                        }
                        Image {
                            id: info_Status_Image
                            sourceSize.width: parent.width*(7/100)
                            sourceSize.height: parent.height*(5/100)
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height*(2.2/100)
                            anchors.left: info_Lock_Image.right
                            anchors.leftMargin:  parent.width*(2/100)
                            source: "qrc:/images/image/verify.svg"
                            visible: false
                        }
                    }

                    Rectangle{
                        id:detailsRec
                        width: belowTitleRec.width-activeRecipeRec.width-2
                        height: belowTitleRec.height
                        x:0
                        y:0
                        color: "#F8F9FA"
                        border.color: "#ECECEC"
                        border.width: 1
                        Rectangle{
                            id:info3
                            width: parent.width
                            height: titleRec.height
                            x:0
                            border.color: qmlPrimaryColor
                            color: qmlPrimaryColor
                            Text {
                                id:details
                                text: qsTr("DETAILS")
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: "#ffffff"
                                font.pixelSize: Style.style5
                                font.bold: true
                                font.family: Style.semibold.name
                            }
                        }
                        Text{
                            id:recipedesc
                            y: titleRec.height+(info3.height)
                            text: qsTr("Recipe Description:")
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(8/100)
                            color: "#757575"
                            font.pixelSize: Style.style3
                            font.bold: true
                            font.family: Style.regular.name
                        }
                        Text{
                            id:recipedesc1
                            y: titleRec.height+1.6*(info3.height)
                            width: parent.width*(80/100)
                            wrapMode: Text.WordWrap
                            text: ""
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(8/100)
                            color: "#757575"
                            font.pixelSize: Style.style1
                            font.family: Style.regular.name

                        }

                        Text{
                            id:productionIns
                            y: titleRec.height+4*(info3.height)
                            text: qsTr("Production Instruction:")
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(8/100)
                            color: "#757575"
                            font.pixelSize: Style.style3
                            font.bold: true
                            font.family: Style.regular.name
                        }
                        Text{
                            id:productionIns1
                            y: titleRec.height+4.6*(info3.height)
                            width: parent.width*(80/100)
                            wrapMode: Text.WordWrap
                            text: ""
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(8/100)
                            color: "#757575"
                            font.pixelSize: Style.style1
                            font.family: Style.regular.name
                        }
                    }
                }
            }
        }
    }
}
