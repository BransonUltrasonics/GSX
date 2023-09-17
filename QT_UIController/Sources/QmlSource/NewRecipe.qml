import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import com.branson.menu 1.0
import Style 1.0
import com.branson.eventlogging 1.0
import com.branson.deleteRecipeResponse 1.0
import com.branson.helperenums 1.0

Item {
    readonly property string qmltextOverrideRecipeMsg : qsTr("Do you want to override the recipe number?")
    readonly property string qmltextDiscardChangesMsg : qsTr("Do you want to discard unsaved recipe changes?")
    readonly property string qmltextSuspectRejectMsg : qsTr("Suspect/Reject limits are not valid")
    readonly property string qmltextRecipes:qsTr("RECIPES")
    readonly property string qmltextlab:qsTr("LAB")
    readonly property string qmltextActions:qsTr("Actions")
    readonly property string qmltextCannotCreateMsg: qsTr("User cannot create more than 1000 recipes.")
    readonly property string qmltextInvalidValueCorrectMsg : qsTr("Invalid value is entered, Enter correct value again")

    readonly property string qmltextProductionSetup:qsTr("PRODUCTION SETUP")
    readonly property string qmltextProduction:qsTr("PRODUCTION")
    readonly property string qmltextProductionRun:qsTr("PRODUCTION RUN")
    readonly property string qmltextSetActive:qsTr("SET AS ACTIVE")
    readonly property string qmltextEditRecipe:qsTr("EDIT RECIPE")

    property alias newrecipeitem:newrecipeitem
    property string recipeNameAssign:"NewRecipe"
    property string recipecompNameassign:"BELL"
    property string recipeWeldModeassign:"power"
    property string recipeWeldModeNameScreen:" "
    property var recipeWeldModevalueassign:78
    property string recipeWeldModeunitassign:"N"
    property int clickedIndex: clickedIndex
    property int recipeHoldTimeValue:0
    property int isActive: 0
    property int recipeFlag: 0
    property string isVerifyImg: "qrc:/images/image/verify.svg"
    property int suspectRejectStatus: 0
    property string partImgSource: "qrc:/images/image/part_icon_u135.png"
    property string recipeNumber:"0"
    property bool verifiedstatus: false
    property int lockedstatus: 0
    property bool isCreateRecipe: false
    property bool modifiedStatus: false
    property bool isNewRecipeFlag:true
    id:newrecipeitem

    property MessagePopup mPopup

    signal signalClickTrash()

    Timer
    {
        id:recipeActions
        interval: 10
        triggeredOnStart: false
        onTriggered:
        {
            if(recipeFlag == 0)
            {
                recipeActionAnimation.restart()
                visiblerectmousearea.visible=false
            }
            else
            {
                reverserecipeActionAnimation.restart()
                visiblerectmousearea.visible=true
            }
        }
    }

    /**
    *@breif: To save the recipe values
    **/
    function saveValue()
    {
        var textInput = numpopup.keyInputText
        var dwRecipeNumber = parseInt(textInput)
        if(dwRecipeNumber<=0 || dwRecipeNumber>1000)
        {
            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueCorrectMsg)
            return
        }
        var ValidateRecipeNumber=recipe.validateRecipeNumber(textInput)

        if(!ValidateRecipeNumber)
        {
            messagePopup.open(messagePopup.warning_title, qmltextOverrideRecipeMsg, messagePopup.ok_and_cancel_button)
            messagePopup.onOkay.connect(function onOkay()
            {
                gridparentrect.destroy()
                copyRecipe(recipeNumber,textInput,!isNewRecipeFlag)

                numpopup.close()
                messagePopup.close()
                messagePopup.onOkay.disconnect(onOkay)
            })

            messagePopup.onCancel.connect(function onCancel()
            {
                messagePopup.close()
                messagePopup.onCancel.disconnect(onCancel)                
            })
        }
        else
        {
            gridparentrect.destroy()
            copyRecipe(recipeNumber,textInput,isNewRecipeFlag)
            numpopup.close()
        }
    }

    /**
    *@breif: Message Popup open to set recipe number during copy
    **/
    function setRecipeNumberDuringCopy(newRecipenumber)
    {
        numpopup.open("COPY RECIPE NUMBER",qsTr("COPY RECIPE NUMBER"), newRecipenumber, "")
        numpopup.save.connect(saveValue)
        numpopup.cancel.connect(function pressCancel()
        {
            numpopup.close()
            numpopup.cancel.disconnect(pressCancel)
        })
    }

    /**
    *@breif: When edit recipe is clicked
    **/
    function editRecipe(recipeNumber)
    {
        recipe.isActiveRequired(0)
        recipe.editExistingRecipe(recipeNumber)
        recipe.readResonantFrequencyFromDB()
        recipe.updateRecipeMenuList(window.userLevelId)
        loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNameassign":recipeNameAssign,"recipeNumberInPanel":recipeNumber,"isaddrecipeclick":0,"isEditrecipeclick":1,"menuCopyclickDisabled":0})
        titlebar.text=qmltextRecipes
        titlebarchild.visible=true
        titlebarchildtext.text=qmltextlab
        titlbaronclicked="RecipeListControl.qml"
    }

    function setActiveRecipe(recipeNumber)
    {
        suspectRejectStatus = recipe.midPointValidationForAllParams(false,false,false)

        recipe.setSelectedRecipeToActive(recipeNumber)
        loaderelement.setSource("RecipeListControl.qml")

        if(suspectRejectStatus != 0)
        {
            recipe.updateStatusText(qmltextSuspectRejectMsg)
        }
    }

    /**
    *@breif: Copies the selected recipe
    **/
    function copyRecipe(sourceRecipeNumber,newRecipeNumber,newRecipeFlag)
    {
        recipe.copySelectedRecipe(sourceRecipeNumber,newRecipeNumber,newRecipeFlag)
        recipe.editExistingRecipe(newRecipeNumber)
        recipe.readResonantFrequencyFromDB()
        recipe.updateRecipeMenuList(window.userLevelId)
        loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNameassign":recipeNameAssign,"clickedRecipeIndex":clickedIndex,"recipeNumberInPanel":recipeNumber,"isaddrecipeclick":1,"isEditrecipeclick":1,"menuCopyclickDisabled":0})
        titlebar.text=qmltextRecipes
        titlebarchild.visible=true
        titlebarchildtext.text=qmltextlab
        titlbaronclicked="RecipeListControl.qml"
        Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_COPIED)
    }

    function updateRecipeScreen()
    {
        //update recipe values
        recipename.text = (recipe.getModifiedStatus(recipeNumber)?"* ":"   ") + recipeNumber + " : " + recipeNameAssign
    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateRecipeScreen()
            messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.without_button)
        }
    }

    RectangularGlow {
        id: effect3
        anchors.fill: visiblerect
        glowRadius: 3
        spread: 0.2
        opacity: 0.2
        color: "#000000"
        cornerRadius: visiblerect.radius + glowRadius
        visible: isCreateRecipe?false:true
    }
    Rectangle
    {
        id: visiblerect
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#f8f9fa"
        clip: true
        border.width: isActive
        border.color: qmlSecondaryColor
        visible: isCreateRecipe?false:true

        Rectangle
        {
            id: recipeimage
            height: ((100/330)*parent.height) - 2
            color: "#e9ecef"
            border.color: "#e9ecef"
            anchors.top: visiblerect.top
            anchors.topMargin: 2
            anchors.left: visiblerect.left
            anchors.leftMargin: 2
            anchors.right: visiblerect.right
            anchors.rightMargin: 2

            Image {
                id: recipeIcon_image
                sourceSize.width: parent.width*(40/100)
                sourceSize.height: parent.height*(40/100)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/part_icon_u135.png"
            }
        }

        Rectangle {
            id: recipenamerect
            height: 0.1272727272727273*parent.height
            color: qmlSecondaryColor
            border.color: qmlSecondaryColor
            anchors.top: recipeimage.bottom
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            border.width: 0

            Text {
                id: recipename
                text:(modifiedStatus?"* ":"   ") + recipeNumber + " : " + recipeNameAssign
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                color: "#ffffff"
                font.bold: true
                font.pixelSize: Style.style6
                font.family: Style.semibold.name
            }
        }

        Text {
            id: weldModeId
            x: 60
            y: 44
            width: parent.width*.86
            //height: 0.1272727272727273*parent.height
            height: 80
            wrapMode: Text.WordWrap
            text: recipeWeldModeNameScreen+" : "+recipeWeldModevalueassign+" "+recipeWeldModeunitassign
            anchors.left: parent.left
            anchors.leftMargin: (5/100)*parent.width
            anchors.top: recipenamerect.bottom
            anchors.topMargin: (20/100)*parent.height
            font.bold: true
            font.pixelSize: Style.style5
            font.family: Style.regular.name
            color: "#757575"
        }

        Image {
            id:  status_image
            anchors.leftMargin: parent.width*(4/100)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            sourceSize.width: parent.width*(15/100)
            sourceSize.height: parent.height*(8/100)
            anchors.left: parent.left
            source: isVerifyImg
            visible: verifiedstatus
        }

        Image {
            id: locked_Image
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            sourceSize.width: parent.width*(15/100)
            sourceSize.height: parent.height*(8/100)
            anchors.left: status_image.right
            anchors.leftMargin: 20
            source: "qrc:/images/image/lock_ash.svg"
            visible: lockedstatus?true:false
        }

        MouseArea {
            id: visiblerectmousearea
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            cursorShape: Qt.PointingHandCursor
            onReleased:
            {
                recipeFlag = 0
                recipeActions.start()
            }
        }

        Rectangle {
            id: invisiblerect
            x: 0
            y: (350/330)*parent.height
            color: "#f8f9fa"
            border.width: isActive
            border.color: qmlSecondaryColor
            visible: true


            width: parent.width
            height: parent.height

            NumberAnimation on y {
                id:recipeActionAnimation
                running: visiblerectmousearea.pressed
                to:0
                duration:250
            }

            NumberAnimation on y {
                id:reverserecipeActionAnimation
                running: invisiblerectmousearea.pressed
                to: visiblerect.height
                duration: 250
            }

            MouseArea {
                id: invisiblerectmousearea
                width: parent.width
                height: 0.8666666666666667*parent.height
                z: 0
                cursorShape: Qt.PointingHandCursor

                onReleased:
                {
                    recipeFlag = 1
                    recipeActions.start()
                }
            }

            Rectangle {
                id: actionrect
                anchors.top: invisiblerect.top
                anchors.topMargin: 2
                anchors.left: invisiblerect.left
                anchors.leftMargin: 2
                anchors.right: invisiblerect.right
                anchors.rightMargin: 2
                height: 0.1363636363636364*parent.height
                color: "#e9ecef"

                Text {
                    id: action
                    width: 0.4166666666666667*parent.width
                    height: 0.8444444444444444*parent.height
                    text: qmltextActions
                    color: "#757575"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: Style.style5
                    font.family: Style.regular.name
                }

                Image {
                    id: action_image
                    anchors.right: action.left
                    anchors.rightMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: (12/45)*parent.height
                    sourceSize.width: parent.width*(60/100)
                    sourceSize.height: parent.height*(60/100)
                    /*Image icon update based on System Type*/
                    source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                                   "qrc:/images/image/drop-down-arrow_blue.svg"
                }
            }

            Rectangle {
                id: actioncontrolrect
                anchors.bottom: invisiblerect.bottom
                anchors.left: invisiblerect.left
                anchors.right: invisiblerect.right
                height: 0.1212121212121212*parent.height
                color: "#e9ecef"
                anchors.bottomMargin: 2
                anchors.leftMargin: 2
                anchors.rightMargin: 2

                Rectangle {
                    id: copyreciperect
                    x: (20/216)*parent.width
                    y: (7/40)*parent.height
                    width: 0.1712962962962963*parent.width
                    height: 0.65*parent.height
                    color: "#e9ecef"
                    enabled: isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? true : false
                    Image {
                        id: copyreciperectImage
                        fillMode: Image.PreserveAspectFit
                        anchors.fill: parent
                        source: "qrc:/images/image/u1581.png"
                        MouseArea {
                            id:copyreciperectImageMA
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                var newRecipeNumber;
                                var flagid2 = recipe.getRecipeModifiedState()
                                if(flagid2)
                                {
                                    var component = Qt.createComponent("MessagePopup.qml")
                                    mPopup = component.createObject(window)
                                    mPopup.open(messagePopup.info_title,qmltextDiscardChangesMsg, messagePopup.ok_and_cancel_button)
                                    mPopup.onOkay.connect(function onOkay()
                                    {
                                        mPopup.close()
                                        mPopup.onOkay.disconnect(onOkay)
                                        mPopup.destroy()
                                        newRecipeNumber=recipe.getNewRecipeNumber()
                                        setRecipeNumberDuringCopy(newRecipeNumber)
                                     })

                                    mPopup.onCancel.connect(function onCancel()
                                    {
                                        mPopup.close()
                                        mPopup.onCancel.disconnect(onCancel)
                                        mPopup.destroy()
                                    })
                                }
                                else
                                {
                                    newRecipeNumber=recipe.getNewRecipeNumber();
                                    setRecipeNumberDuringCopy(newRecipeNumber)
                                }
                            }
                        }
                    }
                }


                Rectangle{
                    id: deleteRecipeRect
                    anchors.centerIn: parent
                    width: copyreciperect.width
                    height: copyreciperect.height
                    color: copyreciperect.color
                    enabled: isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? true : false
                    Image{
                        id: deleteRecipeImage
                        fillMode: Image.PreserveAspectFit
                        anchors.fill: parent
                        source: "qrc:/images/image/u1579.png"
                        MouseArea{
                            id: deleteRecipeMouseArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            enabled: (window.userLevelId === HelperEnums.EXECUTIVE? true : false)
                            onClicked: {
                                signalClickTrash()
                            }
                        }
                    }
                }

                Rectangle {
                    id: inforeciperect
                    anchors.top:parent.top
                    anchors.topMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.left: copyreciperect.right
                    anchors.leftMargin: parent.width*(51/100)
                    anchors.bottom: parent.bottom
                    color: "#e9ecef"

                    Image {
                        id: infoImage
                        anchors.fill: parent
                        sourceSize.width: parent.width
                        sourceSize.height: parent.height
                        source: "qrc:/images/image/info.svg"

                        /* Added by Mounika */
                        MouseArea {
                            id: infoImgMA
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                recipeinfo.recipeInfoInit(recipeNumber,recipeNameAssign)
                                recipeInformationPopup.fillRecipeDetails(recipeNumber)
                                recipeInformationPopup.open()
                                enableMainwindowOpacity.start()
                            }
                        }
                    }
                }

            }

            MouseArea {
                id: columnmouseArea
                x: (24/220)*parent.width
                y: (60/330)*parent.height
                width: 0.7818181818181818*parent.width
                height: 0.6363636363636364*parent.height
            }
            Column {
                id: columnbutton
                x: (24/220)*parent.width
                y: (58/330)*parent.height
                width: 0.7818181818181818*parent.width
                height: 0.6363636363636364*parent.height
                anchors.top: actionrect.bottom
                anchors.topMargin: 20
                anchors.centerIn: parent
                spacing: 15

                Button {
                    id: productionrun
                    width: 0.9593023255813953*parent.width
                    height: 0.1904761904761905*parent.height
                    text: qmltextProductionRun
                    font.pixelSize: Style.style2
                    font.family: Style.regular.name
                    highlighted: true

                    background: Rectangle {
                        color: qmlPrimaryColor
                        radius: 4
                    }

                    MouseArea {
                        id: productionrunmouse
                        x: 0
                        y: 0
                        width: parent.width
                        height: parent.height
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            titlebar.text=qmltextProduction
                            titlebarchild.visible= false
                            production.productionInit();
                            leftMenuData_obj.resetParent("PRODUCTION")
                            loaderelement.setSource("ProductionWindow.qml")
                        }
                    }
                }

                Button {
                    id: editrecipe
                    y: (55/210)*parent.height
                    width: 0.9593023255813953*parent.width
                    height: 0.1904761904761905*parent.height
                    highlighted: true
                    //enabled: isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? true : false
                    background: Rectangle {
                        color: qmlPrimaryColor//isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? qmlPrimaryColor : "#757575"
                        radius: 4
                    }
                    contentItem: Text {
                        text: qmltextEditRecipe
                        font{
                            family: Style.regular.name
                            pixelSize: Style.style2
                            capitalization: Font.AllUppercase
                        }
                        color: "#FFFFFF"//isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? "#FFFFFF": "#BEC1C3"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    MouseArea {
                        id: editrecipemouse
                        width: parent.width
                        height: parent.height
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            var flagid1 = recipe.getModifiedStatus(recipeNumber)
                            var flagid2 = recipe.getRecipeModifiedState()
                            if(!flagid1 && flagid2)
                            {
                                var component = Qt.createComponent("MessagePopup.qml")
                                mPopup = component.createObject(window)
                                mPopup.open(messagePopup.info_title,qmltextDiscardChangesMsg, messagePopup.ok_and_cancel_button)
                                mPopup.onOkay.connect(function onOkays()
                                {
                                    gridparentrect.destroy()
                                    editRecipe(recipeNumber)
                                    mPopup.close()
                                    mPopup.onOkay.disconnect(onOkays)
                                    mPopup.destroy()
                                })

                                mPopup.onCancel.connect(function onCancel()
                                {
                                    mPopup.close()
                                    mPopup.onCancel.disconnect(onCancel)
                                    mPopup.destroy()
                                })
                            }
                            else
                            {
                                gridparentrect.destroy()
                                editRecipe(recipeNumber)
                            }
                        }
                    }
                }
                Button {
                    id: productionrunsetup
                    y: (110/210)*parent.height
                    width: 0.9593023255813953*parent.width
                    height: 0.1904761904761905*parent.height
                    text: qmltextProductionSetup
                    font.pixelSize: Style.style2
                    font.family: Style.regular.name
                    highlighted: true
                    background: Rectangle {
                        color: qmlPrimaryColor
                        radius: 4
                    }

                    MouseArea {
                        id: productionsetupmouse
                        x: 0
                        y: 0
                        width: parent.width
                        height: parent.height
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            titlebarchild.visible=true
                            titlebarchildtext.text=qmltextProductionSetup
                            titlbaronclicked="RecipeListControl.qml"
                            recipeSetupInfo.recipeSetupInfoInit(recipeNumber,recipeNameAssign)
                            loaderelement.setSource("RecipeProductionSetUp.qml",{"width":1920,"height":995})
                        }
                    }
                }

                Button {
                    id: setasactive
                    y: (165/210)*parent.height
                    width: 0.9593023255813953*parent.width
                    height: 0.1904761904761905*parent.height
                    text: qmltextSetActive
                    font.pixelSize: Style.style2
                    font.family: Style.regular.name
                    highlighted: true
                    background: Rectangle
                    {
                        id:backRec
                        radius: 4
                    }
                    Component.onCompleted:
                    {
                        var userLevel = login.getUserLevel();
                        if(userLevel == "Operator")
                        {
                            if(((isPermissionAllowed(Features.SET_AS_ACTIVE) == true) && (verifiedstatus == true)) ||
                               ((isPermissionAllowed(Features.SET_AS_ACTIVE) == true) && (isPermissionAllowed(Features.RUN_UNVALIDATED_WELD_RECIPE) == true)))
                            {
                                enabled = true
                                backRec.color = qmlPrimaryColor
                            }
                            else
                            {
                                enabled = false
                                backRec.color = "#757575"
                            }
                        }
                        else
                        {
                            enabled = true
                            backRec.color = qmlPrimaryColor
                        }
                    }

                    MouseArea
                    {
                        id: setasactivemouse
                        x: 0
                        y: 0
                        width: parent.width
                        height: parent.height
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            var flagid1 = recipe.getModifiedStatus(recipeNumber)
                            var flagid2 = recipe.getRecipeModifiedState()
                            if(!flagid1 && flagid2)
                            {
                                var component = Qt.createComponent("MessagePopup.qml")
                                mPopup = component.createObject(window)
                                mPopup.open(messagePopup.info_title,qmltextDiscardChangesMsg, messagePopup.ok_and_cancel_button)
                                mPopup.onOkay.connect(function onOkays()
                                {
                                    gridparentrect.destroy()
                                    setActiveRecipe(recipeNumber)
                                    mPopup.close()
                                    mPopup.onOkay.disconnect(onOkays)
                                    mPopup.destroy()
                                    recipe.loadRecipeCards()
                                })

                                mPopup.onCancel.connect(function onCancel()
                                {
                                    mPopup.close()
                                    mPopup.onCancel.disconnect(onCancel)
                                    mPopup.destroy()
                                })
                            }
                            else
                            {
                                gridparentrect.destroy()
                                setActiveRecipe(recipeNumber)
                            }
                        }
                    }
                }
            }
        }
    }
    RectangularGlow {
        id: effect
        anchors.fill: newobjrect
        glowRadius: 3
        spread: 0.2
        opacity: 0.2
        color: "#000000"
        cornerRadius: newobjrect.radius + glowRadius
        visible: isCreateRecipe
    }
    Rectangle {
        id: newobjrect
        width: parent.width
        height: parent.height
        radius: 3
        visible: isCreateRecipe
        color: isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? "#BEC1C3": "#757575"
        enabled: isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) ? true : false
        Image {
            id: image3
            anchors.centerIn: parent
            sourceSize.width: (parent.width)*(25/100)
            sourceSize.height: parent.height*(25/100)
            source: "qrc:/images/image/add_circle.svg"
        }

        MouseArea {
            id: mouseArea
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            cursorShape: Qt.PointingHandCursor
            onClicked:
            {
                if(recipe.existingRecipeCount() >= 1000)
                {
                    messagePopup.open(messagePopup.info_title,qmltextCannotCreateMsg, messagePopup.without_button)
                    return
                }

                var flagid2 = recipe.getRecipeModifiedState()
                if(flagid2)
                {
                    var component = Qt.createComponent("MessagePopup.qml")
                    mPopup = component.createObject(window)
                    mPopup.open(messagePopup.info_title,qmltextDiscardChangesMsg, messagePopup.ok_and_cancel_button)
                    mPopup.onOkay.connect(function onOkay()
                    {
                        mPopup.close()
                        mPopup.onOkay.disconnect(onOkay)
                        mPopup.destroy()
                        createNewRecipe()
                    })
                    mPopup.onCancel.connect(function onCancel()
                    {
                        mPopup.close()
                        mPopup.onCancel.disconnect(onCancel)
                        mPopup.destroy()
                    })
                }
                else
                {
                    createNewRecipe()
                }
            }
        }
    }

    /**
    *@breif: Creates new recipe
    **/
    function createNewRecipe()
    {
        gridparentrect.destroy()
        recipe.createNewRecipe()
        recipe.updateRecipeMenuList(window.userLevelId)
        loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNameassign":recipeNameAssign,"recipeNumberInPanel":recipe.getRecipeNumber(),"isaddrecipeclick":1})
        titlebar.text=qmltextRecipes
        titlebarchild.visible=true
        titlebarchildtext.text=qmltextlab
        titlbaronclicked="RecipeListControl.qml"
    }
}
