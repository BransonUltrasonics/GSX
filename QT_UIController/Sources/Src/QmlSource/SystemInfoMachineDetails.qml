import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtCharts 2.2
import QtQuick.Window 2.2
Item {
    readonly property string qmltextSoftwareVersions:  qsTr("Software Versions")
    readonly property string qmltextPowerSupply:  qsTr("Power Supply")
    readonly property string qmltextAssemblies:   qsTr("Assemblies")
    readonly property string qmltextActuator:  qsTr("Actuator")
    readonly property string qmltextConnectivity:   qsTr("Connectivity")
    readonly property string qmltextWebServicesLoggedIn :qsTr("Please logout from the Web Services before upgrading the system")

    id: item1
    FirmwareUpgrade
    {
        id:id_fwupgrade_pop
    }
    Flickable{
        id:flickable
        contentHeight: Window.height + 200
        //contentWidth: Window.width
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        bottomMargin: Qt.inputMethod.visible ? parent.height* (5/100) : 0
        clip: true

        Rectangle
        {
            id:mainrec
            anchors.fill: parent
            Column
            {
                id:machinedetailscolumns
                width: parent.width - 10
                height: Window.height/1.2
                spacing: 10
                Rectangle{
                    id:modelid
                    width: parent.width
                    height: parent.height/15
                    GridView{
                        id:modellist
                        width: parent.width
                        height: parent.height
                        cellHeight: parent.height
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelmodellist

                        delegate:
                            Rectangle{
                            id:modelbackground
                            width: 0.4*connectivitylist.width
                            height: 50
                            color: "#ECECEC"
                            SystemConfigMenuItem {
                                id: configlistmenu
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") +qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                                Text{ x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id:softwareversion
                    width: parent.width
                    height: (login.getUserLevel() == "Executive") ?((parent.height/4.5) + 70):(parent.height/4.5)
                    Rectangle{
                        id:headertext
                        width: parent.width
                        height: 35
                        Text{
                            id:softwareverheader
                            text: qmltextSoftwareVersions
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            color: "#898D96"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle{
                        id:underline
                        height: 1
                        width: parent.width/1.1
                        color:"#898D96"
                        anchors.top:headertext.bottom
                    }
                    GridView{
                        id:versionlist
                        width: parent.width
                        height: (login.getUserLevel() == "Executive") ?parent.height - 40:parent.height
                        layoutDirection: Qt.RightToLeft
                        anchors.top: underline.bottom
                        anchors.topMargin: 10
                        anchors.rightMargin: 100
                        cellHeight: (login.getUserLevel() == "Executive") ? ((parent.height/2.5)-40): (parent.height/2.5)
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelversionlist
                        delegate:
                            Rectangle{
                            id:background
                            width: 0.4*versionlist.width
                            height: 50
                            color: "#ECECEC"
                            SystemConfigMenuItem {
                                id: systemConfigMenuItem2
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") + qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style2
                                }
                                Text{ x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style2
                                }
                            }
                        }
                    }
                    Rectangle {
                        id:id_fwupgrade
                        anchors.top: softwareversion.top
                        anchors.topMargin: 150
                        anchors.leftMargin: 40
                        height: 40
                        visible: (login.getUserLevel() == "Executive")
                        PrimaryButton {
                            width: 180
                            text:qsTr("SOFTWARE UPGRADE")
                            onClicked: {
                                //open upgrade popup here
                                enableMainwindowOpacity.start()
                                id_fwupgrade_pop.open()
                            }
                        }
                    }
                }
                Rectangle{
                    id:powersupply
                    width: parent.width
                    height: parent.height/3.3
                    Rectangle{
                        id:powersupplyheader
                        width: parent.width
                        height: 35
                        Text{
                            id:powersupplytext
                            text: qmltextPowerSupply
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            color: "#898D96"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle{
                        id:powersupplyline
                        height: 1
                        width: parent.width/1.1
                        color:"#898D96"
                        anchors.top:powersupplyheader.bottom
                    }
                    GridView{
                        id:powersupplylist
                        width: parent.width
                        height: parent.height
                        anchors.top: powersupplyline.bottom
                        anchors.topMargin: 10
                        cellHeight: parent.height/3.5
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelpowersupplylist
                        delegate:
                            Rectangle{
                            id:powersupplybg
                            width: 0.4*powersupplylist.width
                            height: 50
                            color: "#ECECEC"
                            SystemConfigMenuItem {
                                id: powersupplymenu
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") + qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                                Text{ x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id:asemblies
                    width: parent.width
                    height: parent.height/4.5
                    visible: false
                    Rectangle{
                        id:asembliestext
                        width: parent.width
                        height: 35
                        Text{
                            id:asembliesheader
                            text: qmltextAssemblies
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            color: "#898D96"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle{
                        id:asembliesline
                        height: 1
                        width: parent.width/1.1
                        color:"#898D96"
                        anchors.top:asembliestext.bottom
                    }
                    GridView{
                        id:asemblieslist
                        width: parent.width
                        height: parent.height
                        anchors.top: asembliesline.bottom
                        anchors.topMargin: 10
                        cellHeight: parent.height/2.5
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelassemblylist

                        delegate:
                            Rectangle{
                            id:asembliesbackground
                            width: 0.4*asemblieslist.width
                            height: 50
                            color: "#ECECEC"
                            clip: true
                            SystemConfigMenuItem {
                                id: listmenu
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") + qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                                Text{
                                    id: assemblyTextName
                                    x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    id:actuator
                    width: parent.width
                    height: parent.height/3.3
                    Rectangle{
                        id:actuatortext
                        width: parent.width
                        height: 35
                        Text{
                            id:actuatorheader
                            text: qmltextActuator
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            color: "#898D96"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle{
                        id:actuatorline
                        height: 1
                        width: parent.width/1.1
                        color:"#898D96"
                        anchors.top:actuatortext.bottom
                    }
                    GridView{
                        id:actuatorlist
                        width: parent.width
                        height: parent.height
                        anchors.top: actuatorline.bottom
                        anchors.topMargin: 10
                        cellHeight: parent.height/3.5
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelactuatorlist
                        delegate:
                            Rectangle{
                            id:actuatorbackground
                            width: 0.4*asemblieslist.width
                            height: 50
                            color: "#ECECEC"
                            SystemConfigMenuItem {
                                id: actuatorlistmenu
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") +qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                                Text{ x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    id:connectivity
                    width: parent.width
                    height: parent.height/2.5
                    Rectangle{
                        id:connectivitytext
                        width: parent.width
                        height: 35
                        Text{
                            id:connectivityheader
                            text:qmltextConnectivity
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            color: "#898D96"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle{
                        id:connectivityline
                        height: 1
                        width: parent.width/1.1
                        color:"#898D96"
                        anchors.top:connectivitytext.bottom
                    }
                    GridView{
                        id:connectivitylist
                        width: parent.width
                        height: parent.height
                        anchors.top: connectivityline.bottom
                        anchors.topMargin: 10
                        cellHeight: parent.height
                        cellWidth: parent.width/2
                        boundsBehavior: Flickable.StopAtBounds
                        model: modelconnectivitylist
                        delegate:
                            Rectangle{
                            id:connectivitybackground
                            width: 0.4*connectivitylist.width
                            height: 50
                            color: "#ECECEC"
                            SystemConfigMenuItem {
                                id: connectivitylistmenu
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                Text{
                                    x: 0.5172413793103448*parent.width
                                    y: parent.height/2-height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:qsTr(":  ") + qsTr(model.modelData.ParameterContent)
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                                Text{ x: (7/290)*parent.width
                                    y: 0.2857142857142857*parent.height/1.5
                                    width: .355*parent.width
                                    height: .42*parent.height
                                    text:model.modelData.ParameterName
                                    elide: Text.ElideMiddle
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: Style.regular.name
                                    font.pixelSize:  Style.style1
                                }
                            }
                        }
                    }
                }
            }
            Rectangle{
                id: thirdPartyInfo
                width: parent.width
                height: 50
                anchors.top:machinedetailscolumns.bottom
                anchors.topMargin: 190
                Text{
                    id:thirdPartyInfoheader
                    text:qsTr("Third Party Software Information")
                    font.pixelSize: Style.style1
                    font.family: Style.semibold.name
                    color: "#898D96"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle{
                    id:thirdPartyInfoline
                    height: 1
                    width: parent.width/1.1
                    color:"#898D96"
                    anchors.top:thirdPartyInfoheader.bottom
                    anchors.topMargin: 10
                }
                Text{
                    id:thirdPartyInfoDesc
                    width: thirdPartyInfoline.width
                    text:qsTr("Please refer to the following URL for information about third party software (e.g., open source software) used in this product: https://www.emerson.com/documents/automation/open-source-software-notice-en-us-5317230.pdf")
                    wrapMode: Text.WordWrap
                    anchors.top: thirdPartyInfoline.bottom
                    anchors.topMargin: 10
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style1
                }
            }
        }
    }
}








