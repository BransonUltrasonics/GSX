import QtQuick 2.0
import QtQuick.Controls 2.2
import Style 1.0
import QtQuick 2.7
import QtTest 1.1
import QtQuick.Layouts 1.3

Item {
    readonly property string qmltextMenu: qsTr("MENU")
    readonly property string qmltextSystem: qsTr("SYSTEM")

    id:leftactionCenterItemId
    z:1
    NumberAnimation on x
    {
        id:leftActionAnimationReturn
        from:leftactionCenterItemId.x ;to:-leftactionCenterItemId.width-leftmenuoutsideareaID.width
        duration: 250
    }
    width: 480 //while using as element that value will over write this values
    property var rectanglewidth: rectangleDetailsId.width-50
    property var  rectangleheight: (rootrectangle.height-100)/5.8
    property var menuIconWidth: 49
    property var menuIconHeight: 38
    property var menuSepratorHeight: 6
    property alias  leftactionCenterItemId: leftactionCenterItemId
    property alias  menuRectangleHgt: menuRectangle.height

    property var menuIndex: 0
    property var menuName: qsTr("dashboard")
    Component.onCompleted:
    {
        leftActionAnimationReturn.restart()
    }


    /**
    *@breif: To reset left menu details
    **/
    function resetLeftMenu()
    {
        leftMenuData_obj.resetParent(menuName)
        titlebarchild.visible = false
        titlebar.text = menuName
        disableMainwindowOpacity.start()
    }
    Timer
    {
        id:resetleftMenuTimer
        interval: 100
        triggeredOnStart: false
        onTriggered:
        {
            resetLeftMenu()
        }
    }

    Rectangle {
        id: rootrectangle
        width: parent.width
        color: "transparent"
        anchors.rightMargin: 0
        anchors.fill: parent
        //For mobile screen and small screen Canvas element is not required
        Canvas{
            id: canvash
            x: 0
            y: 0
            width: rootrectangle.height
            height:  rootrectangle.height
            z: 0
            rotation: 90
            opacity: 1
            onPaint:
            {
                var ctx = getContext("2d")
                ctx.fillStyle = "#212529"
                ctx.beginPath()
                ctx.arc(canvash.width/2, canvash.height+150,canvash.width-(25),0,Math.PI,1)
                ctx.fill()
            }
            MouseArea
            {
                anchors.fill: parent
            }
        }

        Rectangle
        {
            id: rectangleDetailsId
            width: parent.width/2+100
            color:"#212529"
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Rectangle
            {
                id: menuRectangle
                x: -307
                y: -19
                width: 138
                height: 24
                color: "#212529"
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin:30// 10

                Text
                {
                    id: textMenu
                    y: 2
                    width: 58
                    height: 22
                    color: "#ffffff"
                    font.family: Style.regular.name
                    text: qmltextMenu
                    anchors.left: parent.left//image.right
                    anchors.leftMargin:70// 6
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: Style.style7
                }

                Image
                {
                    id: image
                    y: 9+15
                    width: 21//11
                    height: 26//15
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    source: "qrc:/images/image/menu_close_arrow.png"
                }

                MouseArea
                {
                    id: backmouseArea
                    x: 0
                    y: 0
                    width: 138
                    height: 24
                    onClicked:
                    {
                        leftActionAnimationReturn.restart()
                        disableMainwindowOpacity.start()
                    }
                }
            }

            Rectangle
            {
                id: id_menuParentRect
                anchors.topMargin: 60
                x: 50
                anchors.top: menuRectangle.bottom
                width:(rectangleDetailsId.width)
                height:(rectangleDetailsId.height*3/4)
                color: "#212529"

                Column
                {
                    id: id_menuOptionLayout
                    x : 70
                    width:parent.width
                    height: parent.height
                    Repeater
                    {
                        model: leftMenuModelData

                        delegate:Rectangle
                        {
                            id: id_menuOptionRect
                            width: rectangleDetailsId.width - 45
                            height: id_menuOptionLayout.height/5 + 10
                            color: "#212529"
                            Component.onCompleted:
                            {
                                if(model.modelData.MenuOption == qmltextSystem)
                                {
                                    id_menuOptionRect.width = rectangleDetailsId.width - 80
                                }
                            }
                            RowLayout
                            {
                                id: id_menuImageTextLayout
                                width: parent.width
                                opacity: 1
                                Image
                                {
                                    id: id_menuIcon
                                    width: menuIconWidth
                                    height:menuIconHeight
                                    source: model.modelData.MenuIcon
                                    anchors.right: id_spacer.left
                                }
                                Rectangle
                                {
                                    id: id_spacer
                                    width: menuIconWidth
                                    height:menuIconHeight
                                    color: "#212529"
                                    anchors.right: id_menuOption.left
                                }
                                Text
                                {
                                    id: id_menuOption
                                    color: model.modelData.MenuColor
                                    width: parent.width - id_menuIcon.width - id_spacer.width
                                    font.family:Style.regular.name
                                    text: model.modelData.MenuOption
                                    font.pixelSize: Style.style8
                                    anchors.left : id_spacer.right
                                }
                                MouseArea
                                {
                                    id: mouseAreaDashboard
                                    anchors.fill: id_menuOption
                                    hoverEnabled: true
                                    onEntered:
                                    {
                                        cursorShape=Qt.PointingHandCursor
                                    }
                                    onClicked:
                                    {
                                        leftActionAnimationReturn.restart()
                                        if((titlebar.text == model.modelData.MenuOption)&&(titlebarchild.visible == false))
                                        {
                                            disableMainwindowOpacity.start()
                                            return
                                        }

                                        menuIndex = model.modelData.MenuIndex
                                        menuName = model.modelData.MenuOption
                                        switch(menuIndex)
                                        {
                                        case 1:
                                            dashboard.init();
                                            break;
                                        case 2:
                                            recipe.loadRecipeCards()
                                            break;
                                        case 3:
                                            production.productionInit();
                                            break;
                                        case 4:
                                            break;
                                        case 5:
                                            break;
                                        default:
                                            break;
                                        }
                                        loaderelement.setSource(model.modelData.MenuAction)
                                        titlebarchild.visible = false
                                        titlebar.text = menuName
                                        disableMainwindowOpacity.start()
                                        leftMenuData_obj.resetParent(menuName)
                                    }
                                }
                            }
                            ToolSeparator
                            {
                                id: id_menuSeprator
                                x: -50
                                z: 10
                                width:id_menuParentRect.width - 10
                                height: menuSepratorHeight//height is not given in to the HMI library,make it Dynamic also not making good look
                                anchors.top: id_menuOptionRect.bottom
                                anchors.topMargin: -35
                                orientation: Qt.Vertical
                                Component.onCompleted:
                                {
                                    if(model.modelData.MenuOption == qmltextSystem)
                                    {
                                        id_menuSeprator.visible = false
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id:leftmenuoutsideareaID
        width: 15+leftactionCenterItemId.parent.width-leftactionCenterItemId.width+(rootrectangle.width/10)//leftactionCenterItemId.width//-leftaction.width+100//parent.width
        height: parent.height
        opacity:0
        anchors.left :  rootrectangle.right
        anchors.leftMargin:  -rootrectangle.width/10//60
        z:1
        color: "transparent"
        visible: true
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                leftActionAnimationReturn.restart()
                disableMainwindowOpacity.start()
            }
        }
    }
}
