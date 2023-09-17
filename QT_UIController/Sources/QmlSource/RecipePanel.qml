import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Style 1.0
import QtQuick.Controls.Styles 1.4
//import Qt.labs.gestures 1.0
import com.branson.eventlogging 1.0
import com.branson.menu 1.0
import com.branson.helperenums 1.0

Item {
    id: item1
    //    width: 1280
    //    height: 750
    property CustomNumPad cNumpad
    readonly property string qmltextDiscardChangesMsg : qsTr("Do you want to discard unsaved recipe changes?")
    readonly property string qmltextOverrideRecipeMsg : qsTr("Do you want to override the recipe number?")
    readonly property string qmltextSuspectRejectMsg : qsTr("Suspect/Reject limits are not valid")
    readonly property string qmltextUnSavedRecipeValidateMsg : qsTr("Unsaved recipe can't be validated. Save the recipe and try again")
    readonly property string qmltextUnSavedRecipeInValidateMsg : qsTr("Unsaved recipe can't be Invalidated. Save the recipe and try again")
    readonly property string qmlTextStackRecipeFail: qsTr("Failed to save stack recipe data. Check for network connection or recipe parameters check")
    readonly property string qmltextInvalidValueCorrectMsg : qsTr("Invalid value is entered, Enter correct value again")
    readonly property string qmlTextHistory: qsTr("HISTORY")
    readonly property string qmlTextRecipeNumber: qsTr("RecipeNumber")
    readonly property string qmlTextCopyRecipeNo: qsTr("COPY RECIPE NUMBER")
    readonly property string qmlTextStringRecipe: qsTr("RECIPE")
    readonly property string qmlTextRecipeValidated: "Recipe is validated"
    readonly property string qmlTextRecipeInValidated: "Recipe is invalidated"
    readonly property string qmlTextRecipeSaved: "Recipe is modified"
    readonly property string qmltextCommtReqMsg:qsTr("Comment is required")
    property string qmlRecipeName: qsTr("RECIPES")
    property string qmltextRecipeName: "recipename"
    property string recipeNumberInPanel:"1"

    property string qmllabname: qsTr("LAB")
    property string qmlParameter: qsTr("PARAMETER")
    property string emptyString: ""
    property bool menuStatus: false;
    property string recipeNameassign:"NewRecipe"
    property int clickedRecipeIndex: 0
    property var tableHeaderH: 0
    property var tableRowH: 0
    property var windowWidth: 0
    property var windowHeight: 0
    property bool suspectRejectStatus: false
    property bool isaddrecipeclick: false
    property bool isCopyRecipeStatus: false
    property int newclickindexaftercopy: 0
    property int menuCopyclickDisabled: 0
    property int statusOfClicked: 0
    property bool isEditrecipeclick: false
    property bool isNewRecipeCreated: false
    property var oldrecipeName: ""
    property bool isNewRecipeFlag:true
    property bool isMenuModelLoaded: false
    property int focused: 0
    property int validateStatus: 0
    property bool isActiveReq: true
    property bool recipenameReq: false
    property MessagePopup mPopup
    property DialogPopup dialogPopup
    /**
    *@breif: Plot different kinds of graph based on the index
    **/
    function plotGraph()
    {
        if(weldResultHistory.graphPageIndex == 0)
        {
            loaderGraphElement.item.resetPlots()
            loaderGraphElement.item.plotGraph()
        }
        else if(graphPageIndex == 1)
        {
            //loaderGraphElement.item.plotTrendsGraph(g_SelectedRecipeNum)
            loaderGraphElement.item.tableRowSelection(weldHistoryListView.currentIndex)
        }
        else if(graphPageIndex == 2)
        {
            loaderGraphElement.item.tableRowSelection(weldHistoryListView.currentIndex)
        }
    }

    Component.onCompleted:
    {
        texteditrecipeId.focus = false
        recipenumberId.focus = true
        texteditrecipeId.text = recipe.getRecipeName()
        recipenumberId.text = recipe.getRecipeNumber()
        config.setActivePage(3)
    }

    /**
    *@breif: Save the recipe
    **/
    function saveValue()
    {
        recipeNumberInPanel = recipenumberId.text
        var textInput = cNumpad.keyInputText
        var dwRecipeNumber = parseInt(textInput)
        if(dwRecipeNumber<=0 || dwRecipeNumber>1000)
        {
            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueCorrectMsg)
            return
        }
        recipe.loadRecipeCards()
        var ValidateRecipeNumber=recipe.validateRecipeNumber(textInput)

        if(!ValidateRecipeNumber)
        {
            messagePopup.open(messagePopup.warning_title, qmltextOverrideRecipeMsg, messagePopup.ok_and_cancel_button)
            messagePopup.onOkay.connect(function onOkay()
            {
                copyRecipe(recipeNumberInPanel,textInput,!isNewRecipeFlag)
                cNumpad.close()
                messagePopup.close()
                messagePopup.onOkay.disconnect(onOkay)
                // update the screen
                recipe.updateRecipeMenuList(window.userLevelId)
                comboboxRecipeActionID.model = recipeButtonModel
                comboboxRecipeActionID.update()
            })
            messagePopup.onCancel.connect(function onCancel(){
                messagePopup.close()
                messagePopup.onCancel.disconnect(onCancel)
            })
        }
        else
        {
            copyRecipe(recipeNumberInPanel,textInput,isNewRecipeFlag)
            cNumpad.close()
            messagePopup.close()
        }
    }

    /**
    *@breif: Set the recipe number during recipe copy
    **/
    function setRecipeNumberDuringCopy(newRecipenumber)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)

        cNumpad.open("COPY RECIPE NUMBER",qmlTextCopyRecipeNo, newRecipenumber, emptyString)
        cNumpad.save.connect(saveValue)
        cNumpad.cancel.connect(function pressCancel()
        {
            cNumpad.close()
            cNumpad.cancel.disconnect(pressCancel)
        })
    }

    /**
    *@breif: Set the recipe number during recipe creation
    **/
    function setRecipeNumberForNewRecipe(newRecipenumber)
    {
        recipeNumberInPanel = recipenumberId.text
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)

        cNumpad.open("RECIPE NUMBER",qsTr("RECIPE NUMBER"), newRecipenumber, emptyString)
        cNumpad.save.connect(function saveNewRecipeNumber()
        {
            var textInput = cNumpad.keyInputText
            var dwRecipeNumber = parseInt(textInput)
            if(dwRecipeNumber<=0 || dwRecipeNumber>1000)
            {
                displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueCorrectMsg)
                return
            }
            recipe.loadRecipeCards()
            var ValidateRecipeNumber=recipe.validateRecipeNumber(textInput)

            if(!ValidateRecipeNumber)
            {
                messagePopup.open(messagePopup.warning_title, qmltextOverrideRecipeMsg, messagePopup.ok_and_cancel_button)
                messagePopup.onOkay.connect(function onOkay()
                {
                    recipe.setParamaterChangedFlag(0)
                    recipenumberId.text = textInput
                    recipe.exisistingRecipeEdited(recipeNumberInPanel,textInput)
                    cNumpad.close()
                    messagePopup.close()
                    messagePopup.onOkay.disconnect(onOkay)
                })
                messagePopup.onCancel.connect(function onCancel(){
                    messagePopup.close()
                    messagePopup.onCancel.disconnect(onCancel)
                })
            }
            else
            {
                recipenumberId.text = textInput
                recipe.setParamaterChangedFlag(0)
                recipe.exisistingRecipeEdited(recipeNumberInPanel,textInput)
                cNumpad.close()
                messagePopup.close()
            }
        })
        cNumpad.cancel.connect(function pressCancel()
        {
            cNumpad.close()
            cNumpad.cancel.disconnect(pressCancel)
        })
    }

    /**
    *@breif: Copy the existing recipe
    **/
    function copyRecipe(sourceRecipeNumber,newRecipeNumber,newRecipeFlag)
    {
        recipe.copySelectedRecipe(sourceRecipeNumber,newRecipeNumber,newRecipeFlag)
        Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_COPIED)
        if(recipenameReq)
            loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNameassign":recipeNameassign,"recipeNumberInPanel":recipeNumberInPanel,"isaddrecipeclick":1,"isEditrecipeclick":1,"menuCopyclickDisabled":0})
        else
            loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"recipeNumberInPanel":recipeNumberInPanel,"isaddrecipeclick":1,"isEditrecipeclick":1,"menuCopyclickDisabled":0})
        titlebar.text=qmlRecipeName
        titlebarchild.visible=true
        titlebarchildtext.text=qmllabname
        titlbaronclicked="RecipeListControl.qml"
    }

    /**
    *@breif: To create new recipe
    **/
    function creteNewRecipe()
    {
        recipe.createNewRecipe()
        recipe.updateRecipeMenuList(window.userLevelId)
        loaderelement.setSource("RecipePanel.qml",{"width":parent.width,"height":parent.height,"isaddrecipeclick":1})
        titlebar.text=qmlRecipeName
        titlebarchild.visible=true
        titlebarchildtext.text=qmllabname
        titlbaronclicked="RecipeListControl.qml"
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
                if(message === qmlTextRecipeValidated || message === qmlTextRecipeInValidated)
                {
                    validateStatus = recipe.setSelectedRecipeToValidate(recipenumberId.text)
                    isMenuModelLoaded = false
                    recipe.updateRecipeMenuList(window.userLevelId)

                    if(validateStatus == 1)
                    {
                        Eventlogger.eventSend(EventLogging.UI_EVENT_VALIDATED_CHANGE, comment)
                    }
                    else
                    {
                        Eventlogger.eventSend(EventLogging.UI_EVENT_VALIDATED_CHANGE, comment)
                    }

                    isMenuModelLoaded = true
                }
                else if(message === qmlTextRecipeSaved)
                {
                    recipe.setParamaterChangedFlag(0)
                    var strComment = recipe.storeRecipeToDB()
                    isMenuModelLoaded = false
                    recipe.updateRecipeMenuList(window.userLevelId)
                    isMenuModelLoaded = true
                    StackRecipeObj.saveUserData();
                    Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_SAVED, strComment+comment)
                }
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

    function updateRecipePanelScreen()
    {
        //update recipe panel values
        recipeAsteriskID.visible = recipe.isRecipeModified

    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateRecipePanelScreen()

            if(1 == typeOfChangeIndex)
            {
                messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.ok_button)
                messagePopup.displayPopupClose = 1
                messagePopup.onOkay.connect(function onOkay()
                {
                    recipe.loadRecipeCards()
                    loaderelement.setSource("RecipeListControl.qml")
                    leftMenuData_obj.resetParent(qmlRecipeName)
                    titlebar.text=qmlRecipeName
                    titlebarchild.visible= false
                    messagePopup.close()
                    messagePopup.onOkay.disconnect(onOkay)
                })
            }
            else if (0 === messagePopup.displayPopupClose)
                messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.without_button)

        }
    }

    CustomGestureArea
    {
        id: swipeGesture
        anchors.fill: parent
        onSwipe: {
            switch (direction)
            {
            case "left":
                if(carouselDots.selected)
                {
                    carouselDots.selected=false
                    rtol.restart()
                }
                break
            case "right":
                if(!carouselDots.selected)
                {
                    carouselDots.selected=true
                    ltor.restart()
                }
                break
            }
        }
    }

    Rectangle {
        id: rectangle
        x: 0
        width: parent.width
        height: (50/750)*parent.height
        color: "#ffffff"

        Text {
            id:recipenumberId
            anchors.left: rectangle.left
            anchors.leftMargin: 40
            anchors.top: rectangle.top
            anchors.topMargin: 0
            anchors.bottom: rectangle.bottom
            anchors.bottomMargin: 0
            color: "#757575"
            text: recipeNumberInPanel
            font.family: Style.regular.name
            font.pixelSize: Style.style5
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                id: recipenumberIdMA
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    if(isaddrecipeclick || isNewRecipeCreated)
                    {
                        setRecipeNumberForNewRecipe(recipenumberId.text)
                    }
                }
            }
        }
        Text {
            id: recipeColonId
            text: " : "
            anchors.left: recipenumberId.right
            anchors.leftMargin: 2
            anchors.top: rectangle.top
            anchors.topMargin: 0
            anchors.bottom: rectangle.bottom
            anchors.bottomMargin: 0
            color: "#757575"
            font.family: Style.regular.name
            font.pixelSize: Style.style5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            id: recipeAsteriskID
            text: " * "
            anchors.right: recipenumberId.left
            anchors.rightMargin: 1
            anchors.top: rectangle.top
            anchors.topMargin: 0
            anchors.bottom: rectangle.bottom
            anchors.bottomMargin: 0
            color: "#757575"
            font.family: Style.regular.name
            font.pixelSize: Style.style5
            verticalAlignment: Text.AlignVCenter
            visible: recipe.isRecipeModified
            onVisibleChanged: {
                recipe.updateRecipeMenuList(window.userLevelId)
            }
        }
        TextInput
        {
            id: texteditrecipeId
            height: parent.height
            width: parent.width/4
            color: "#757575"
            text: recipeNameassign
            font.family: Style.regular.name
            font.pixelSize: Style.style5
            verticalAlignment: Text.AlignVCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: recipeColonId.right
            anchors.leftMargin: 0
            selectByMouse: true
            maximumLength: 15
            enabled: (window.userLevelId === HelperEnums.OPERATOR? false : true)
            validator: RegExpValidator
            {
                regExp: /^[a-zA-Z0-9`!@#$%&*()_+={}:;<>.?-]*$/
            }
            onTextChanged:
            {
                recipe.setParamaterChangedFlag(1)
                recipeNameassign = texteditrecipeId.text
                enteredRecipeName = texteditrecipeId.text
                if(focused == 1)
                {
                    if(enteredRecipeName != "")
                        recipe.setStructureValue(qmltextRecipeName,recipeNameassign,true)
                }
                else
                    recipe.setStructureValue(qmltextRecipeName,recipeNameassign,false)
            }
            onFocusChanged:
            {
                if(activeFocusOnPress)
                {
                    focused = 1
                }
            }
        }

        CustomDropDown
        {
            id: comboboxRecipeActionID
            anchors.right: parent.right
            anchors.rightMargin: 100
            width: (11/100)*parent.width
            droptext: qmlTextStringRecipe
            model: recipeButtonModel
            enabled: isPermissionAllowed(Features.NEW_SAVE_COPY_VALIDATED_UNVALIDATED_RECIPE) ? true : false

            Component.onCompleted:
            {
                currentIndex = -1
                isMenuModelLoaded = true;
            }

            onActivated:
            {
                if(!isMenuModelLoaded)
                    return
                if(enteredRecipeName == "")
                {
                    recipe.updateStatusText(qmlTextRecipeNameInvalid)
                    return
                }
                switch(currentIndex)
                {
                    /** creating new recipe */
                case 0:
                    if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                    {
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
                                creteNewRecipe()
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
                            creteNewRecipe()
                        }
                    }
                    break;
                    /* save the recipe */
                case 1:
                    if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) || (parseInt(recipe.getStructureValueByName("global setup")) == 1))
                    {
                        recipe.setParamaterChangedFlag(0)
                        var strComment = recipe.storeRecipeToDB()
                        isMenuModelLoaded = false
                        recipe.updateRecipeMenuList(window.userLevelId)
                        isMenuModelLoaded = true
                        StackRecipeObj.saveUserData();
                        Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_SAVED, strComment)
                    }
                    break;

                    /* copy the recipe */
                case 2:
                    if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                    {
                        var newRecipeNumber;
                        var flagid = recipe.getRecipeModifiedState()
                        if(flagid)
                        {
                            var component1 = Qt.createComponent("MessagePopup.qml")
                            mPopup = component1.createObject(window)
                            mPopup.open(messagePopup.info_title,qmltextDiscardChangesMsg, messagePopup.ok_and_cancel_button)
                            mPopup.onOkay.connect(function onOkay()
                            {
                                mPopup.close()
                                mPopup.onOkay.disconnect(onOkay)
                                mPopup.destroy()
                                newRecipeNumber=recipe.getNewRecipeNumber()
                                recipenameReq = false
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
                            recipenameReq = true
                            setRecipeNumberDuringCopy(newRecipeNumber)
                        }
                        menuStatus = false
                    }
                    break;

                    /* validate the recipe */
                case 3:
                    if(recipe.isRuntimeFeatureEnabledForCFR())
                    {
                        if(currentText == qsTr("Validate"))
                        {
                            if(recipe.isRecipeModified)
                            {
                                recipe.updateStatusText(qmltextUnSavedRecipeValidateMsg)
                                return
                            }
                            createDialogpopup(qmlTextRecipeValidated)
                        }
                        else if(currentText == qsTr("Invalidate"))
                        {
                            if(recipe.isRecipeModified)
                            {
                                recipe.updateStatusText(qmltextUnSavedRecipeInValidateMsg)
                                return
                            }
                            createDialogpopup(qmlTextRecipeInValidated)
                        }
                    }
                    else
                    {
                        if(recipe.isRecipeModified)
                        {
                            if(currentText == qsTr("Validate"))
                                recipe.updateStatusText(qmltextUnSavedRecipeValidateMsg)
                            else if(currentText == qsTr("Invalidate"))
                            {
                                recipe.updateStatusText(qmltextUnSavedRecipeInValidateMsg)
                                //console.debug("Invalidate.............");
                            }
                            return
                        }
                        validateStatus = recipe.setSelectedRecipeToValidate(recipenumberId.text)
                        isMenuModelLoaded = false
                        recipe.updateRecipeMenuList(window.userLevelId)
                        if(validateStatus == 1)
                        {
                            Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_VALIDATED)
                        }
                        else
                            Eventlogger.eventSend(EventLogging.UI_EVENT_RECIPE_INVALIDATED)
                    }
                    isMenuModelLoaded = true
                    menuStatus = false
                    break;
                default:
                    break;
                }
                currentIndex = -1
            }
        }

        CarouselDots
        {
            id: carouselDots
            x: parent.width/2
            y: parent.height/2-height/2
            width: Math.sqrt((32/1920)*parent.width*(32/52)*parent.height)
            height: Math.sqrt((32/1920)*parent.width*(32/52)*parent.height)
            Component.onCompleted: {
                selected = true
            }
            marLeft.onClicked: {
                if(!selected)
                {
                    carouselDots.selected=true
                    ltor.restart()
                    bar.currentIndex=0
                }
                Qt.inputMethod.hide();
            }
            marRight.onClicked: {
                if(selected)
                {
                    carouselDots.selected=false
                    rtol.restart()
                    bar.currentIndex=1
                }
                Qt.inputMethod.hide();
            }
        }
    }

    TabBar
    {
        id: bar
        z:1
        x: labPanel.x+labPanel.width
        width: (18/100)*parent.width
        height: (40/750)*parent.height
        anchors.top: rectangle.bottom
        anchors.topMargin: 0
        onCurrentIndexChanged:
        {
            if(currentIndex==1)
            {
                weldResultHistory.selectedRecipeNumber = recipeNumberInPanel
                if(!weldResultHistory.isHistoryPressed)
                {
                    loadNormalGraph()
                    weldResultHistory.isHistoryPressed = true
                    if(weldResultHistory.graphPageIndex == 0)
                        loaderGraphElement.item.weldGraphLoadTimer.start()
                    weldResultHistory.weldGraphPlotTimer.start()
                }
                if(carouselDots.selected)
                {
                    carouselDots.selected=false
                    rtol.restart()
                }
            }
        }
        TabButton {
            height: parent.height
            contentItem: Text {
                text:qmlParameter
                font.family: Style.semibold.name
                font.pixelSize: Style.style2
                opacity: enabled ? 1.0 : 1
                color:  bar.currentIndex===0 ? qmlPrimaryColor : "#757575"
                anchors.leftMargin: (160/480)*parent.width
                anchors.left: parent.left
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height/*parent.height*/
                opacity: enabled ? 1 : 1
                color:"#f8f9fa"
                border.width: 0
                radius: 0
                Rectangle{
                    x:(20/130)*parent.width
                    y:parent.height-(2/40)*parent.height
                    width:parent.width-x
                    height:2
                    color:bar.currentIndex===0?qmlPrimaryColor:"#00ffffff"
                }
            }
        }
        TabButton {
            height: parent.height
            contentItem: Text {
                text: qmlTextHistory
                font.family: Style.semibold.name
                font.pixelSize: Style.style2
                opacity: enabled ? 1.0 : 1
                color:  bar.currentIndex===1 ? qmlPrimaryColor : "#757575"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height
                opacity: enabled ? 1 : 1
                color:"#f8f9fa"
                border.width: 0
                radius: 0
                Rectangle{
                    y:parent.height-(2/40)*parent.height
                    width:parent.width-(20/130)*parent.width
                    height:2
                    color:bar.currentIndex===1?qmlPrimaryColor:"#00ffffff"
                }
            }
        }
    }
    StackLayout {
        id: lab_stacklayout
        x: labPanel.x+labPanel.width
        z:1
        width:(18/100)*parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: bar.bottom
        anchors.topMargin: 0
        currentIndex: bar.currentIndex

        Item
        {
            RecipeParameter
            {
                id: recipeParameter
                x: 0
                width:(230/1280)*item1.width
                height:parent.height
                recipeIndex: clickedRecipeIndex
                dialogPosition: 0
                isaddrecipeitemclicked:isaddrecipeclick?isaddrecipeclick:(menuCopyclickDisabled?(isEditrecipeclick):(!isEditrecipeclick))
            }
        }
        Item
        {
            RecipeResultHistory
            {
                id: weldResultHistory
                x: 0
                y: 0
                width:(230/1280)*item1.width
                height:parent.height
                selectedRecipeNumber: recipeNumberInPanel
            }
        }
    }
    Loader
    {
        id:loaderGraphElement

        x:bar.x+bar.width
        width: (82/100)*parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangle4.bottom
    }
    function loadNormalGraph()
    {
        weldResultHistory.graphPageIndex = 0
        loaderGraphElement.setSource("GraphRightParameter.qml",{"width":parent.width,"height":parent.height})
    }

    function loadTrendsGraph()
    {
        weldResultHistory.graphPageIndex = 1
        loaderGraphElement.setSource("TrendsGraph.qml",{"width":parent.width,"height":parent.height})
    }

    function loadTableGraph()
    {
        weldResultHistory.graphPageIndex = 2
        loaderGraphElement.setSource("AnalyticsResultsTable.qml",
                                     {   "width":parent.width-20,
                                         "height":parent.height-20,
                                         "tableHeaderH": weldResultHistory.listRectHeight,
                                         "tableRowH": weldResultHistory.graphListHeight})
        //recipe.updateTableGraphModel()
    }

    Rectangle {
        id: rectangle4
        width: (82/100)*parent.width
        height: (42/750)*parent.height
        color: "#F8F9FA"
        anchors.top: rectangle.bottom
        anchors.topMargin: 0
        anchors.leftMargin: (18/100)*parent.width
        anchors.left: parent.left
        border.color: "#f8f9fa"

        DropShadow{
            source: rectangle5
            anchors.fill: rectangle5
            horizontalOffset: 1
            verticalOffset: 1
            color: "#000000"
            opacity: 0.2
            radius: 3
            visible: false
        }
        Rectangle {
            id: rectangle5
            width: (60/1020)*parent.width
            height: (35/42)*parent.height
            color: "#e9ecef"
            border.color: "#e9ecef"
            radius: 0
            anchors.bottomMargin: (4/42)*parent.height
            anchors.bottom: parent.bottom
            anchors.topMargin: (3/42)*parent.height
            anchors.top: parent.top
            anchors.rightMargin: (8/1020)*parent.width
            anchors.right: parent.right
            anchors.leftMargin: (976/1020)*parent.width
            anchors.left: parent.left
            visible: false
            layer.enabled: true
            layer.effect: DropShadow
            {
                //transparentBorder:true
                horizontalOffset:0.5
                verticalOffset:0.5
                opacity:0.2
                radius:3
                color:"#000000"
                samples:7
            }

            Image {
                id: filterimage
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                anchors.fill: parent
                source: "qrc:/images/image/filter.svg"
                visible: false
            }
        }
    }

    LabPanel {
        id: labPanel
        x: 0
        y: bar.y
        width: parent.width*1568/1920
        height: (670/780)*parent.height
        recipeIndex1: clickedRecipeIndex
        NumberAnimation on x {
            id:ltor
            from:bar.currentIndex===0?0:labPanel.parent.width*(-(1568/1920))
            to:0
            duration:250
        }
        NumberAnimation on x {
            id:rtol
            from:0
            to:(-(1568/1920))*labPanel.parent.width
            duration:250
        }
    }
}
