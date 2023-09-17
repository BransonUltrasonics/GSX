import QtQuick 2.0
import QtQuick.Controls 2.1
import com.branson.openedscreen 1.0
import com.branson.helperenums 1.0
import com.branson.deleteRecipeResponse 1.0
import com.branson.eventlogging 1.0

Item{
    property bool isDone:false
    property alias gridparentrect: gridparentrect
    property bool isSpriteDrawn: false
    id:recipelistcontrolID

    property MessagePopup deleteRecipePopup
    property DialogPopup dialogPopup

    property int deleteRecipeNumber
    property int deleteRecipeActive


    readonly property string qmlTextDeleteRecipeMsg: qsTr("Are you sure you want to delete the recipe?")
    readonly property string qmlTextDeleteSuccessful: qsTr("Recipe is deleted successfully")
    readonly property string qmlTextRecipeNumberInvalid: qsTr("Recipe number is invalid")
    readonly property string qmlTextDeleteActiveRecipe: qsTr("Deleting an active recipe is not allowed")
    readonly property string qmlTextDeleteRecipeFailed: qsTr("Failed to delete recipe from Database")
    readonly property string qmlTextRecipeNotInDatabase: qsTr("Recipe is not present in Database")
    readonly property string qmlTextFailToDelete: qsTr("Fail to delete recipe")



    Component.onCompleted:
    {
        config.setActivePage(3)
        var component = Qt.createComponent("MessagePopup.qml")
        deleteRecipePopup = component.createObject(window)
        deleteRecipePopup.onOkay.connect(onOkay)
        deleteRecipePopup.onCancel.connect(onCancel)

        var dialogComponent = Qt.createComponent("DialogPopup.qml")
        dialogPopup = dialogComponent.createObject(mainwindow)
        dialogPopup.onSave.connect(dialogPopupOnSave)
        dialogPopup.onCancel.connect(dialogPopupOnCancel)
    }

    Component.onDestruction:
    {
        //config.setActivePage(OpenedScreenEnums.NONE)
        deleteRecipePopup.onOkay.disconnect(onOkay)
        deleteRecipePopup.onCancel.disconnect(onCancel)
        deleteRecipePopup.destroy()

        dialogPopup.onSave.disconnect(dialogPopupOnSave)
        dialogPopup.onCancel.disconnect(dialogPopupOnCancel)
        dialogPopup.destroy()
    }

    /**
    *@brief: to handle the Done button of delete recipe
    **/
    function onOkay()
    {
        deleteRecipePopup.close()
        showDialogPopupBox()
    }

    /**
    *@brief: to handle the Cancel button of delete recipe
    **/
    function onCancel()
    {
        deleteRecipePopup.close()
    }

    /**
    *@brief: to handle the save button for deleting a recipe
    **/
    function dialogPopupOnSave()
    {
        var resDelete = deleteArecipe()
        showDeleteRecipeResponse(resDelete)
        var deleteComment = dialogPopup.quitMessage
        Eventlogger.eventSend(EventLogging.UI_EVENT_DELETE_RECIPE,deleteComment);
        dialogPopup.close()
        recipe.loadRecipeCards()
    }

    /**
    *@brief: to handle the cancel button for deleting a recipe
    **/
    function dialogPopupOnCancel()
    {
        dialogPopup.close()
    }

    /**
    *@brief: show the result of deleting a recipe at the bottom left corner of the screen.
    **/
    function showDeleteRecipeResponse(responseCode)
    {
        var strResponse = ""
        switch(responseCode)
        {
        case DeleteRecipeResponse.DELETE_RECIPE_SUCCESS:
            strResponse = qmlTextDeleteSuccessful      //"Recipe is deleted successfully"
            break
        case DeleteRecipeResponse.INVALID_RECIPE_NUMBER:
            strResponse = qmlTextRecipeNumberInvalid  //"Recipe number is invalid"
            break
        case DeleteRecipeResponse.DELETE_RECIPE_FAILED_ACTIVE_RECIPE:
            strResponse = qmlTextDeleteActiveRecipe   //"Delete recipe failed because its a ACTIVE RECIPE"
            break
        case DeleteRecipeResponse.DELETE_RECIPE_FAILED:
            strResponse = qmlTextDeleteRecipeFailed   //"Failed to delete recipe from Database"
            break
        case DeleteRecipeResponse.RECIPE_NOT_PRESENT:
            strResponse = qmlTextRecipeNotInDatabase  //"Recipe is not present in Database"
            break
        default:
            strResponse =  qmlTextFailToDelete       //"Fail to delete recipe"
            break
        }
        recipe.updateStatusText(strResponse)
    }



    /**
    *@brief: Delete a recipe
    **/
    function deleteArecipe()
    {
        var resDelete = -2
        //if the recipe is active recipe, it shold not be deleted.
        if(0 === deleteRecipeActive) /* this recipe is not active */
        {
            //send the delete message
            resDelete = recipe.deleteArecipeCard(deleteRecipeNumber)
        }
        else  /* the recipe is active and it cannot be deleted */
        {
            resDelete = DeleteRecipeResponse.DELETE_RECIPE_FAILED_ACTIVE_RECIPE
            recipe.updateStatusText(qmlTextDeleteActiveRecipe)
        }
        return resDelete
    }

    /**
    *@brief: show the message box
    **/
    function showDeleteRecipeMessageBox()
    {
        deleteRecipePopup.open(deleteRecipePopup.warning_title,qmlTextDeleteRecipeMsg, deleteRecipePopup.ok_and_cancel_button)
    }

    /**
    *@brief: show the message box of deleting recipe
    **/
    function showDialogPopupBox()
    {
        dialogPopup.open(messagePopup.info_title, "")
    }

    Rectangle {
        id: gridparentrect
        property alias gridrecipelist: gridrecipelist
        width: parent.width
        height: parent.height - mainStatusbarHeight
        anchors.topMargin: (2/480)*parent.height
        anchors.top: parent.top
        clip: true
        Flickable {
            anchors.fill: parent
            x:20
            y:20
            contentHeight: gridrecipelist.height + 30
            contentWidth: gridrecipelist.width
            ScrollBar.vertical: ScrollBar { }
            GridView
            {
                id: gridrecipelist
                x:20
                y:20
                width: gridparentrect.width
                height: gridparentrect.height
                anchors.leftMargin: 20
                anchors.topMargin: 20
                cellHeight: 350
                cellWidth: 240
                cacheBuffer: 0
                model:recipeModel
                delegate:existingRecipe
            }

            Component
            {
                id:existingRecipe
                NewRecipe
                {
                    id: newRecipedelegate
                    width:220
                    height:330
                    isCreateRecipe:model.modelData.RecipeName===""&&model.modelData.RecipeNumber===""?true:false
                    recipeNumber: model.modelData.RecipeNumber
                    recipeNameAssign:model.modelData.RecipeName
                    recipecompNameassign:model.modelData.CompanyName
                    recipeWeldModeassign:model.modelData.WeldMode
                    recipeWeldModeNameScreen:model.modelData.WeldModeNameScreen
                    recipeWeldModeunitassign:model.modelData.WeldModeUnit
                    recipeWeldModevalueassign:model.modelData.WeldModeValue
                    isActive:model.modelData.isActive?2:0
                    verifiedstatus:model.modelData.isValidate
                    modifiedStatus: model.modelData.isModified
                    clickedIndex:index
                    onSignalClickTrash: {
                        deleteRecipeNumber = recipeNumber
                        deleteRecipeActive = isActive
                        showDeleteRecipeMessageBox()
                    }

                }
            }
        }
    }
}
