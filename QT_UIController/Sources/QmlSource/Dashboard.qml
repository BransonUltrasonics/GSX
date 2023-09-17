/*
 * Dashboard.qml
 *
 *  Created on: Aug 1, 2017
 */
import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.menu 1.0
import com.branson.openedscreen 1.0
import com.branson.runtimefeatures 1.0

Item {
    readonly property string qmltextActions :  qsTr("Actions")
    readonly property string qmltextProductionRun :  qsTr("PRODUCTION RUN")
    readonly property string qmltextProduction :  qsTr("PRODUCTION")
    readonly property string qmltextEditRecipe :  qsTr("EDIT RECIPE")
    readonly property string qmltextNA :  qsTr("na")
    readonly property string qmltextRecipe :  qsTr("RECIPES")
    readonly property string qmltextLab :  qsTr("LAB")
    readonly property string qmltextProductionSetUp :   qsTr("PRODUCTION SETUP")
    readonly property string qmltextViewAllRecipes : qsTr("VIEW ALL RECIPES")
    readonly property string qmltextViewAllAlarms : qsTr("VIEW ALL ALARMS")
    readonly property string qmltextViewAllResults: qsTr("VIEW ALL RESULTS")
    readonly property string qmltextActiveRecipe :qsTr("ACTIVE RECIPE")
    readonly property string qmltextWeldResults :qsTr("WELD RESULTS")
    readonly property string qmltextAlarmLog :qsTr("ALARM LOG")
    readonly property string qmltextTroubleShootError :qsTr("TROUBLESHOOT ERROR")
    readonly property string qmltextAlarmDate :qsTr("AlarmDate")
    readonly property string qmltextLastWeld:qsTr("Last Weld")
    readonly property string qmltextProductionOverview:qsTr("PRODUCTION OVERVIEW")
    readonly property string qmltextANALYTICS:qsTr("ANALYTICS")
    readonly property string qmltextColon:" : "
    readonly property string qmltextSpace :" "


    anchors.fill: parent
    id: item1
    property var left_MarginMain : (mainrec.width*(10/100))/4
    property var dashboardCard_width: (mainrec.width*(90/100))/3
    property var lastWeldTopGap: productionmain.height/20
    property var lastWeldLeftGap:productionmain.width/20
    property alias mainrec: mainrec
    property int activeRecipeEditFlag: 0

    property int activeRecipeFlag: 0
    property int lastWeldFlag: 0
    property int alarmFlag: 0
    Component.onCompleted:
    {
        config.setActivePage(1)
        dashboard.setDashboardOpenStatus(true)
        refreshPartsPerMin.restart()
        if(config.getFeatureFlagStatus(RuntimeFeatures.UI_ALARM_LOG) != 1)
        {
            alarmlog_text.visible = false
            mouseArea_Alarm1.visible = false
        }
    }
    Component.onDestruction:
    {
        //config.setActivePage(OpenedScreenEnums.NONE)
        dashboard.setDashboardOpenStatus(false)
        activeRecipeActions.stop()
        lastWeldAcctions.stop()
        alarmActions.stop()
        refreshPartsPerMin.stop()
    }

    function updateDashboardScreen()
    {
        //update dashboard values
        dashboard.init()
        recipeName_Text.text = (recipe.getModifiedStatus(dashboard.m_RecipeNumber)?"* ":"" ) + dashboard.m_RecipeNumber+" : "+dashboard.m_RecipeName
        companyName_Text.text = dashboard.m_Company
        weldMode.text = dashboard.m_WeldMode + qmltextColon
        controlvalue.text = dashboard.m_ControlValue + qmltextSpace + dashboard.m_ControlValueUnit
        lock_Image.visible = dashboard.m_IsLockedValue?true:false
        status_Image.visible = dashboard.m_IsValidated ? 1 : 0
    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateDashboardScreen()
            messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.without_button)
        }
    }

    Timer
    {
        id:refreshPartsPerMin
        interval: 5000
        repeat: true
        triggeredOnStart: true
        onTriggered:
        {
            dashboard.refreshPartsPerMin()
        }
    }

    Timer
    {
        id:activeRecipeActions
        interval: 10
        triggeredOnStart: false
        onTriggered:
        {
            if(activeRecipeFlag == 0)
                anim1.restart()
            else
                revanim1.restart()
        }
    }
    Timer
    {
        id:lastWeldAcctions
        interval: 10
        triggeredOnStart: false
        onTriggered:
        {
            if(lastWeldFlag == 0)
                anim2.restart()
            else
                revanim2.restart()
        }
    }
    Timer
    {
        id:alarmActions
        interval: 10
        triggeredOnStart: false
        onTriggered:
        {
            if(alarmFlag == 0)
                anim3.restart()
            else
                revanim3.restart()
        }
    }

    /* Main Rectangle for all 3 elements on Dashboard*/
    Rectangle{
        id:mainrec
        width: item1.width
        height: item1.height
        color: "#F8F9FA"

        RectangularGlow {
            id: effect
            anchors.fill: activeRecipe
            glowRadius: 3
            spread: 0.2
            color: "#000000"
            opacity: 0.2
            height: parent.height*3/4
            width: dashboardCard_width
            cornerRadius: activeRecipe.radius + glowRadius
        }

        /*Active Recipe */

        Rectangle{
            id: activeRecipe
            height: parent.height*3/4
            width: dashboardCard_width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*(4.8/100)
            border.color: "#F8F9FA"
            anchors.left: parent.left
            anchors.leftMargin: left_MarginMain
            anchors.top: active.bottom
            anchors.topMargin: parent.height*(1/100)//7
            border.width: 1
            color: "#F8F9FA"
            Rectangle{
                id: activeRecipe_1
                width: activeRecipe.width
                height: activeRecipe.height
                color: "#ffffff"
                radius: 0
                border.color: "#ffffff"
                border.width: 1
                MouseArea {
                    id: ma_ActiveRecipe_1
                    width: activeRecipe.width
                    height: activeRecipe.height
                    hoverEnabled: true
                    onReleased:
                    {
                        activeRecipeFlag = 0
                        activeRecipeActions.start()
                    }
                    onEntered:
                    {
                        cursorShape=Qt.PointingHandCursor
                    }
                }
                Rectangle {
                    id: aR_Rect1
                    width: activeRecipe_1.width
                    height: activeRecipe_1.height*(3/10)
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
                    width: activeRecipe_1.width
                    height: activeRecipe_1.height*(1/10)
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
                        id: recipeName_Text
                        color: "#ffffff"
                        text:(recipe.getModifiedStatus(dashboard.m_RecipeNumber)?"* ":"" ) + dashboard.m_RecipeNumber+" : "+dashboard.m_RecipeName
                        anchors.topMargin: 0
                        anchors.top: parent.top
                        anchors.verticalCenter:  parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width*(4/100)
                        font.family: Style.semibold.name
                        font.bold: true
                        font.pixelSize: Style.style5
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text {
                    id: companyName_Text
                    color: "#898D96"
                    text:dashboard.m_Company
                    anchors.top: aR_Rect2.bottom
                    anchors.topMargin: parent.height*(12/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(4/100)
                    font.pixelSize: Style.style3
                    font.family: Style.regular.name
                }
                Text {
                    id: weldMode
                    anchors.top: companyName_Text.bottom
                    anchors.topMargin:  parent.height*(4/100)
                    anchors.left: parent.left
                    anchors.leftMargin:  parent.width*(4/100)
                    color: "#898D96"
                    text:dashboard.m_WeldMode + qmltextColon
                    font.family: Style.regular.name
                    font.pixelSize: Style.style1
                }
                Text {
                    id:controlvalue
                    anchors.top: companyName_Text.bottom
                    anchors.topMargin:  parent.height*(3.8/100)
                    anchors.left: weldMode.right
                    anchors.leftMargin:  parent.width*(2/100)
                    color: "#898D96"
                    text:dashboard.m_ControlValue + qmltextSpace + dashboard.m_ControlValueUnit
                    font.bold: true
                    font.family: Style.regular.name
                    font.pixelSize: Style.style3
                }
                Image {
                    id: lock_Image
                    sourceSize.width: parent.width*(7/100)
                    sourceSize.height: parent.height*(7/100)
                    anchors.left: status_Image.right
                    anchors.leftMargin:  parent.width*(2/100)
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height*(2.5/100)
                    source: "qrc:/images/image/lock_ash.svg"
                    visible: dashboard.m_IsLockedValue?true:false
                }
                Image {
                    id: status_Image
                    sourceSize.width: parent.width*(7/100)
                    sourceSize.height: parent.height*(7/100)
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height*(2.5/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(4/100)
                    source: "qrc:/images/image/verify.svg"
                    visible: dashboard.m_IsValidated ? 1 : 0
                }
                visible: true
            }

            clip: true
            Rectangle {
                id: activerecipe_2
                y: parent.height
                width: activeRecipe.width
                height: activeRecipe.height
                color: "#f8f9fa"
                border.color: "#f8f9fa"
                border.width: 1
                NumberAnimation on y{
                    id:revanim1
                    running: true
                    to:activeRecipe.height
                    duration: 250
                }
                NumberAnimation on y{
                    id:anim1
                    running: true
                    to:0
                    duration: 250
                }

                MouseArea {
                    id: ma_ActiveRecipe_2
                    x: 4
                    y: 2
                    width: activerecipe_2.width-4
                    height: activerecipe_2.height-aR_Rect4.height
                    onReleased:
                    {
                        activeRecipeFlag = 1
                        activeRecipeActions.start()
                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                }

                Rectangle {
                    id: aR_Rect3
                    height: activerecipe_2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    color: "#E9ECEF"
                    border.color: "#E9ECEF"
                    radius: 4
                    Text {
                        id: aR_Rect3_Text
                        anchors.centerIn: parent
                        color: "#898D96"
                        text: "    "+qmltextActions
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: Style.style5
                        font.bold: true
                        font.family: Style.regular.name
                    }

                    Image {
                        id: aR_Rect3_Img
                        sourceSize.width: parent.width*(30/100)
                        sourceSize.height: aR_Rect3.height*(40/100)
                        /*Image icon update based on System Type*/
                        source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                                       "qrc:/images/image/drop-down-arrow_blue.svg"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: aR_Rect3_Text.left
                    }
                }


                Button {
                    id: aR_Button1
                    height: activeRecipe.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: activerecipe_2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: activerecipe_2.width*1/10
                    anchors.top: aR_Rect3.bottom
                    anchors.topMargin: activerecipe_2.width*1/10

                    background: Rectangle {
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 3
                        radius: 4
                    }
                    contentItem: Text {
                        text: qmltextProductionRun
                        styleColor: "#FFFFFF"
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                    }
                    autoRepeat: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                    onClicked: {
                        titlebar.text=qmltextProduction
                        titlebarchild.visible= false
                        production.productionInit();
                        leftMenuData_obj.resetParent(qmltextProduction)
                        loaderelement.setSource("ProductionWindow.qml")
                    }
                }
                Button {
                    id: aR_Button2
                    height: activerecipe_2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: activerecipe_2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: activerecipe_2.width*1/10
                    anchors.top: aR_Button1.bottom
                    anchors.topMargin: aR_Button1.height*1/8
                    enabled:dashboard.m_IsActiveValue
                    background: Rectangle {
                        radius: 4
                        color:dashboard.m_IsActiveValue ? qmlPrimaryColor : "#757575"
                    }
                    contentItem: Text {
                        text: qmltextEditRecipe
                        styleColor: "#FFFFFF"
                        color: dashboard.m_IsActiveValue ? "#FFFFFF": "#BEC1C3"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                    }
                    autoRepeat: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                    onClicked:
                    {
                        if(companyName_Text.text.toLowerCase() == qmltextNA)
                        {
                            //TODO:: Need to show some popup here.
                        }
                        else
                        {
                            leftMenuData_obj.resetParent("RECIPES")
                            recipe.loadRecipeCards()
                            recipe.editExistingRecipe(dashboard.m_RecipeNumber)
                            recipe.readResonantFrequencyFromDB()
                            loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNameassign":dashboard.m_RecipeName,"recipeNumberInPanel":dashboard.m_RecipeNumber,"isaddrecipeclick":0,"isEditrecipeclick":1,"menuCopyclickDisabled":0})
                            titlebar.text = qmltextRecipe
                            titlebarchild.visible = true
                            titlebarchildtext.text = qmltextLab
                            titlbaronclicked="RecipeListControl.qml"
                        }
                    }
                }

                Button {
                    id: aR_Button3
                    height: activerecipe_2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: activerecipe_2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: activerecipe_2.width*1/10
                    anchors.top: aR_Button2.bottom
                    anchors.topMargin: aR_Button1.height*1/8
                    enabled:dashboard.m_IsActiveValue ? true : false
                    background: Rectangle {
                        radius: 4
                        color:dashboard.m_IsActiveValue ? qmlPrimaryColor : "#757575"
                    }
                    contentItem: Text {
                        text:qmltextProductionSetUp
                        styleColor: "#FFFFFF"
                        color: dashboard.m_IsActiveValue ? "#FFFFFF": "#BEC1C3"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                    }
                    autoRepeat: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                    onClicked:
                    {
                        if(companyName_Text.text.toLowerCase() == qmltextNA)
                        {
                            //TODO:: Need to show some popup here.
                        }
                        else
                        {
                            recipeSetupInfo.recipeSetupInfoInit(dashboard.m_RecipeNumber,dashboard.m_RecipeName)
                            loaderelement.setSource("RecipeProductionSetUp.qml",{"width":1920,"height":995})
                            titlebarchildtext.text = qmltextProductionSetUp
                            leftMenuData_obj.resetParent(qmltextRecipe)
                            titlebar.text=qmltextRecipe
                            titlebarchild.visible=true
                            titlbaronclicked="RecipeListControl.qml"
                        }
                    }
                }
                Button {
                    id: aR_Button4
                    height: activerecipe_2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: activerecipe_2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: activerecipe_2.width*1/10
                    anchors.top: aR_Button3.bottom
                    anchors.topMargin: aR_Button1.height*1/8
                    background: Rectangle {
                        radius: 4
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 3
                    }
                    contentItem: Text {
                        text: qmltextViewAllRecipes
                        styleColor: "#FFFFFF"
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                    }
                    autoRepeat: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                    onClicked: {
                        recipe.loadRecipeCards()
                        loaderelement.setSource("RecipeListControl.qml")
                        leftMenuData_obj.resetParent(qmltextRecipe)
                        titlebar.text=qmltextRecipe
                        titlebarchild.visible= false
                        mainWindow.leftaction.recipeColor="#0099ff"
                        mainWindow.leftaction.recipeImage="qrc:/images/image/OnclickRecipes.svg"
                    }
                }
                Rectangle {
                    id: aR_Rect4
                    height: activerecipe_2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.bottom:  parent.bottom
                    anchors.bottomMargin:  0
                    color: "#E9ECEF"
                    border.color: "#E9ECEF"
                    radius: 4
                    Image {
                        id: aR_Rect4_Img
                        sourceSize.width: parent.width*(15/100)
                        sourceSize.height: parent.height*(70/100)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/images/image/info.svg"
                    }
                    MouseArea{
                        id:infomouse
                        width: parent.width*(10/100)
                        height: parent.height*(70/100)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked:
                        {
                            if(companyName_Text.text.toLowerCase() == qmltextNA)
                            {
                                //TODO:: Need to show some popup here.
                            }
                            else
                            {
                                recipeinfo.recipeInfoInit(dashboard.m_RecipeNumber,dashboard.m_RecipeName)
                                recipeInformationPopup.fillRecipeDetails(dashboard.m_RecipeNumber)
                                recipeInformationPopup.open()
                                enableMainwindowOpacity.start()
                            }
                        }
                        onEntered: {
                            cursorShape=Qt.PointingHandCursor
                        }
                    }
                }
                z: 2
            }
        }

        RectangularGlow {
            id: effect1
            anchors.fill: weldResult
            glowRadius: 3
            spread: 0.2
            color: "#000000"
            opacity: 0.2
            cornerRadius: weldResult.radius + glowRadius
            height: parent.height*3/4
            width: dashboardCard_width
        }
        /*Weld Result */
        Rectangle {
            id: weldResult
            color: "#F8F9FA"
            anchors.bottomMargin:  parent.height*(4.8/100)
            anchors.leftMargin: left_MarginMain
            anchors.bottom: parent.bottom
            anchors.top: weldresults_text.bottom
            anchors.topMargin: parent.height*(1/100)
            border.color: "#F8F9FA"
            anchors.left: activeRecipe.right
            border.width: 1
            clip: true
            height: parent.height*3/4
            width: dashboardCard_width
            Rectangle {
                id: weldResult1
                width: weldResult.width
                height: weldResult.height
                color: "#f8f9fa"
                border.width: 1
                border.color: "#F8F9FA"

                Rectangle {
                    id: productionmain
                    color: "#f8f9fa"
                    height: weldResult1.height*(55/100)
                    width:  weldResult1.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin:weldResult1.height*(45/100)
                    Rectangle{
                        id: productionRec
                        height: (parent.height)*1/10
                        width:  parent.width
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 1
                        Text {
                            id: production_Text
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(4/100)
                            anchors.top: parent.top
                            anchors.topMargin: parent.height*(30/100)
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Style.style3
                            color: "#ffffff"
                            text: qmltextProduction
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family:  Style.semibold.name
                        }
                    }
                    GridView{
                        id: productionParam
                        width: parent.width - 20
                        height: parent.height - productionRec.height - 20
                        anchors.top:productionRec.bottom
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        clip: true
                        cellWidth:  productionParam.width/2
                        cellHeight: productionParam.height/2
                        boundsBehavior: Flickable.StopAtBounds
                        flickableDirection: Flickable.AutoFlickDirection
                        model: modelProdctionParams
                        delegate: DashboardProductionDelegate
                        {
                            paramName: model.modelData.ParameterName
                            paramValue: model.modelData.ParameterValue
                            innerRecColor: model.modelData.ParameterRecColor
                            innerRecBorderColor: model.modelData.ParameterRecColor
                            goodText.font.pixelSize : model.modelData.FontSize
                            height: productionParam.cellHeight-20
                            width:  productionParam.cellWidth-20
                        }
                    }
                }
                Rectangle {
                    id: lastWeldMain
                    height: weldResult1.height*(45/100)
                    width:  weldResult1.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin:0
                    anchors.top: productionmain.bottom
                    anchors.topMargin: 0
                    anchors.left:parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    color: "#f8f9fa"

                    Rectangle{
                        id: lastWeld
                        width: parent.width
                        height:(weldResult1.height/2)*0.1
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 1

                        Text {
                            id: lastWeldText
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width*(4/100)
                            anchors.top: parent.top
                            anchors.topMargin: parent.height*(30/100)
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Style.style3
                            verticalAlignment: Text.AlignVCenter
                            color: "#ffffff"
                            text:qmltextLastWeld
                            horizontalAlignment: Text.AlignHCenter
                            font.family: Style.semibold.name
                        }
                    }
                    GridView{
                        id: lastWeldParam
                        width: parent.width - 20
                        height: parent.height - lastWeld.height - 10
                        anchors.top:lastWeld.bottom
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        clip: true
                        cellWidth:  lastWeldParam.width/2
                        cellHeight: lastWeldParam.height/3
                        boundsBehavior: Flickable.StopAtBounds
                        flickableDirection: Flickable.AutoFlickDirection
                        model: modelLastWeldParams
                        delegate: DashboardLastWeldDelegate{
                            parameterName: model.modelData.ParameterName
                            parameterValue:model.modelData.ParameterValue
                            parameterUnit: model.modelData.ParameterUnit
                            height: lastWeldParam.cellHeight-25
                            width:  lastWeldParam.cellWidth-20
                        }
                    }
                }
            }

            MouseArea {
                id: weldResult1_MouseArea
                width: weldResult1.width
                height: weldResult1.height
                onReleased:
                {
                    lastWeldFlag = 0
                    lastWeldAcctions.start()
                }
                onEntered: {
                    cursorShape=Qt.PointingHandCursor
                }
            }
            Rectangle {
                id: weldResult2
                x: 0
                y: parent.height*1.5
                width: weldResult.width
                height: weldResult.height
                color: "#f8f9fa"
                radius: 0
                border.color: "#f8f9fa"
                border.width: 1
                NumberAnimation on y{
                    id:revanim2
                    running: true
                    to:weldResult.height
                    duration: 250
                }
                NumberAnimation on y{
                    id:anim2
                    running: true
                    to:0
                    duration: 250
                }
                MouseArea {
                    id: weldResult2_MouseArea
                    x: 4
                    y: 4
                    width: weldResult.width-8
                    height: weldResult.height-8
                    onReleased:
                    {
                        lastWeldFlag = 1
                        lastWeldAcctions.start()
                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                }
                Rectangle {
                    id: weldResult2Rect
                    width: weldResult2.width
                    height: weldResult2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    color: "#E9ECEF"
                    border.color: "#E9ECEF"
                    border.width: 2
                    Text {
                        id: weldResult2RectTxt
                        anchors.centerIn: parent
                        text: "    "+qmltextActions
                        font.bold: true
                        color: "#898D96"
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Image {
                        id: weldResult2RectImg
                        sourceSize.width: weldResult2Rect.width*(30/100)
                        sourceSize.height: weldResult2Rect.height*(40/100)
                        /*Image icon update based on System Type*/
                        source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                                       "qrc:/images/image/drop-down-arrow_blue.svg"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: weldResult2RectTxt.left
                    }
                }
//                Button
//                {
//                    id: weldButton1
//                    height: weldResult2.height/8
//                    anchors.right: parent.right
//                    anchors.rightMargin: weldResult2.width*1/10
//                    anchors.left: parent.left
//                    anchors.leftMargin: weldResult2.width*1/10
//                    anchors.top: weldResult2Rect.bottom
//                    anchors.topMargin: weldResult2.width*1/10

//                    background: Rectangle {
//                        color: qmlPrimaryColor
//                        border.color: qmlPrimaryColor
//                        border.width: 3
//                        radius: 4
//                    }
//                    contentItem: Text {
//                        text: qmltextProductionOverview
//                        styleColor: "#FFFFFF"
//                        color: "#ffffff"
//                        horizontalAlignment: Text.AlignHCenter
//                        verticalAlignment: Text.AlignVCenter
//                        elide: Text.ElideRight
//                        font.family: Style.regular.name
//                        font.pixelSize: Style.style5
//                    }
//                    MouseArea {
//                        anchors.fill: parent
//                        cursorShape: Qt.ToolTip

//                        onPressed:
//                        {
//                            mouse.accepted = false ;
//                        }
//                    }
//                    autoRepeat: true
//                }
                Button {
                    id: weldButton2
                    height: weldResult2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: weldResult2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: weldResult2.width*1/10
                    anchors.top: weldResult2Rect.bottom
                    anchors.topMargin: weldResult2Rect.width*1/10
                    background: Rectangle {
                        radius: 4
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 3
                    }
                    contentItem: Text {
                        id:textViewAllResults
                        text: qmltextViewAllResults
                        styleColor: "#FFFFFF"
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5

                        MouseArea{
                            anchors.fill:textViewAllResults
                            onClicked: {
                                titlebar.text=qmltextANALYTICS
                                leftMenuData_obj.resetParent(qmltextANALYTICS)
                                loaderelement.setSource("Analytics.qml",{indexofLoader:0})
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                    autoRepeat: true
                }
            }
        }
        RectangularGlow {
            id: effect3
            anchors.fill: alarmLog
            glowRadius: 3
            spread: 0.2
            opacity: 0.2
            color: "#000000"
            height: parent.height*3/4
            width: dashboardCard_width
            cornerRadius: alarmLog.radius + glowRadius
        }
        /*Alarm Logs */
        Rectangle{
            id: alarmLog
            anchors.bottomMargin: parent.height*(4.8/100)
            anchors.leftMargin: left_MarginMain
            anchors.left: weldResult.right
            anchors.bottom: parent.bottom
            anchors.top: alarmlog_text.bottom
            anchors.topMargin: parent.height*(1/100)
            width: dashboardCard_width
            height: parent.height*3/4
            color: "#F8F9FA"
            border.color: "#F8F9FA"
            border.width: 1
            clip: true
            Rectangle {
                id: alarmLog1
                width: parent.width
                height: parent.height
                color: "#F8F9FA"
                border.color: "#F8F9FA"
                border.width: 1
                Rectangle {
                    id: alarmLogRec1
                    width: parent.width*(95/100)
                    height: parent.height*(95/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(4/100)
                    anchors.top: parent.top
                    anchors.topMargin: parent.height*(2/100)
                    color: "#F8F9FA"
                    border.color: "#F8F9FA"
                    clip: true

                    ListView {
                        id: alarmlistView1
                        width: parent.width-10
                        height: parent.height
                        spacing: 1
                        model:alarmList1Model
                        section.property: qmltextAlarmDate
                        section.delegate: sectionHeading
                        delegate: DashboardAlarmListDelegate
                        {
                            width: parent.width - 20
                            height: alarmlistView1.height*(1/10)
                            anchors.bottom: sectionHeading.bottom
                            anchors.bottomMargin:5
                            typeOfImg: model.modelData.AlarmType
                            alarmTime: model.modelData.AlarmTime
                            alarmError: model.modelData.AlarmId
                        }
                        ScrollBar.vertical: ScrollBar{}
                    }

                    Component{
                        id: sectionHeading
                        Rectangle {
                            id: sectionHeadingRec
                            width: parent.width - 10
                            height: 40
                            color: "#F8F9FA"
                            Text {
                                text: section
                                font.pixelSize: Style.style1
                                font.family: Style.semibold.name
                                color: "#898D96"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
                MouseArea {
                    id: mouseArea_Alarm1
                    width: alarmLog.width-25
                    height: alarmLog.height
                    hoverEnabled: true
                    onReleased:
                    {
                        alarmFlag = 0
                        alarmActions.start()
                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                }
            }

            Rectangle {
                id: alarmLog2
                x: 1
                y:parent.height*1.5
                width: alarmLog.width
                height: alarmLog.height
                color: "#f8f9fa"
                border.color: "#f8f9fa"
                border.width: 1
                NumberAnimation on y{
                    id:revanim3
                    running: true
                    to:alarmLog.height
                    duration: 250
                }
                NumberAnimation on y{
                    id:anim3
                    running: true
                    to:0
                    duration: 250
                }
                MouseArea {
                    id: mouseArea_Alarm2
                    width: alarmLog2.width-6
                    height: alarmLog2.height-6
                    onReleased:
                    {
                        alarmFlag = 1
                        alarmActions.start()

                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                }

                Rectangle {
                    id: alarmLog2_Rect1
                    width: alarmLog2.width
                    height: alarmLog2.height/8
                    color: "#E9ECEF"
                    border.color: "#E9ECEF"
                    border.width: 2
                    Text {
                        id: alarmLog2_Text1
                        anchors.centerIn: parent
                        text: "    "+qmltextActions
                        color: "#898D96"
                        font.family: Style.regular.name
                        font.bold: true
                        font.pixelSize: Style.style5
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Image {
                        id: alarmLog_DropDown
                        sourceSize.width: parent.width*(30/100)
                        sourceSize.height: parent.height*(40/100)
                        /*Image icon update based on System Type*/
                        source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                                       "qrc:/images/image/drop-down-arrow_blue.svg"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: alarmLog2_Text1.left
                    }
                }

//                Button {
//                    id: alarmButton1
//                    anchors.right: parent.right
//                    anchors.rightMargin: alarmLog2.width*1/10
//                    anchors.left: parent.left
//                    anchors.leftMargin: alarmLog2.width*1/10
//                    anchors.top: alarmLog2_Rect1.bottom
//                    anchors.topMargin: alarmLog2.width*1/10
//                    height: alarmLog2.height/8
//                    background: Rectangle {
//                        color: qmlPrimaryColor
//                        border.color: qmlPrimaryColor
//                        border.width: 3
//                        radius: 4
//                    }
//                    contentItem: Text {
//                        text:qmltextTroubleShootError
//                        styleColor: "#FFFFFF"
//                        color: "#ffffff"
//                        horizontalAlignment: Text.AlignHCenter
//                        verticalAlignment: Text.AlignVCenter
//                        elide: Text.ElideRight
//                        font.family: Style.regular.name
//                        font.pixelSize: Style.style5
//                    }
//                    autoRepeat: true
//                    MouseArea {
//                        anchors.fill: parent
//                        cursorShape: Qt.ToolTip

//                        onPressed:
//                        {
//                            mouse.accepted = false ;
//                        }
//                    }
//                }
                Button {
                    id: alarmButton2
                    height: alarmLog2.height/8
                    anchors.right: parent.right
                    anchors.rightMargin: alarmLog2.width*1/10
                    anchors.left: parent.left
                    anchors.leftMargin: alarmLog2.width*1/10
                    anchors.top: alarmLog2_Rect1.bottom
                    anchors.topMargin: alarmLog2.width*1/10
                    background: Rectangle {
                        radius: 4
                        color: qmlPrimaryColor
                        border.color: qmlPrimaryColor
                        border.width: 3
                    }
                    contentItem: Text {
                        id:textViewAllAlarms
                        text:qmltextViewAllAlarms
                        styleColor: "#FFFFFF"
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.family: Style.regular.name
                        font.pixelSize: Style.style5
                        MouseArea{
                            anchors.fill:textViewAllAlarms
                            onClicked:
                            {
                                titlebar.text=qmltextANALYTICS
                                analyticalarm.updateAnalyticsAlarmsList()
                                leftMenuData_obj.resetParent(qmltextANALYTICS)
                                loaderelement.setSource("Analytics.qml",{indexofLoader:1})
                            }
                        }
                    }
                    autoRepeat: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip

                        onPressed:
                        {
                            mouse.accepted = false ;
                        }
                    }
                }
            }
        }
        Text {
            id: active
            anchors.left: parent.left
            anchors.leftMargin: left_MarginMain
            anchors.top: parent.top
            anchors.topMargin: parent.height*(4.8/100)
            color: "#898D96"
            text: qmltextActiveRecipe
            font.bold: true
            font.family: Style.semibold.name
            font.pixelSize:  Style.style1
        }
        Text {
            id: weldresults_text
            anchors.left: parent.left
            anchors.leftMargin: parent.width*(35/100)
            anchors.top: parent.top
            anchors.topMargin: parent.height*(4.8/100)
            color: "#898D96"
            text:qmltextWeldResults
            font.family: Style.semibold.name
            font.bold: true
            font.pixelSize:   Style.style1
        }
        Text {
            id: alarmlog_text
            y: 26
            anchors.top: parent.top
            anchors.topMargin: parent.height*(4.8/100)
            anchors.left: parent.left
            anchors.leftMargin: parent.width*(67.5/100)
            color: "#898D96"
            text: qmltextAlarmLog
            font.family: Style.semibold.name
            font.bold: true
            font.pixelSize:   Style.style1
        }
    }
}
