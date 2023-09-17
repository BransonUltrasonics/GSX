import QtQuick 2.0
import QtQuick.Controls 2.2
import com.branson.menu 1.0

Item {
    id:weldmodeselectID

    property alias weldModeGrid:weldModeGrid
    property int recipemodifyflag: 0
    signal modifiedRecipeforWeldmodeSelection(var flagdata);
    property alias weldmodeselectID: weldmodeselectID
    property double partcontactval: 0
    readonly property string qmlTextdenied: qsTr("Absolute Distance mode selection is denied until part contact is done.")

    Rectangle
    {
        anchors.fill: parent
        color: "#F8F9FA"
        GridView
        {
            id: weldModeGrid
            x: 50
            y: 50
            width: parent.width-50
            height: parent.height-50
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            Component.onCompleted:
            {
                weldModeGrid.currentIndex=recipe.getSelectedIndex()
            }
            onModelChanged:
            {
                weldModeGrid.currentIndex=recipe.getSelectedIndex()
            }

            Connections{
                target: recipe
                onRefreshScreenPopUpSignal: {
                    weldModeGrid.currentIndex=recipe.getSelectedIndex()
                }
            }

            cellHeight: height/5
            cellWidth: width/3
            clip:true
            model:modelRecipeWeldMode
            delegate: WeldModeItem
            {
                paramText:model.modelData.ParameterName
                paramTextLocale:model.modelData.ParameterLocaleName
                height:  labelSize
                width:  weldModeGrid.cellWidth-80
                control_mode_mouseArea.onClicked:
                {
                    if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                    {
                        if (weldModeGrid.currentIndex !== index)
                        {
                            var expectedpartcontact = recipe.getStructureValue(203)
                            partcontactval = parseFloat(expectedpartcontact)
                            if((model.modelData.ParameterName == "Absolute Distance")&&(partcontactval <= 0))
                            {
                                displayQuitPopup(messagePopup.warning_title, qmlTextdenied)
                            }
                            else
                            {
                                recipe.setParamaterChangedFlag(1)
                                weldModeGrid.currentIndex=index
                                recipe.changeWeldMode(paramText)
                            }
                        }
                    }
                }
                Connections
                {
                    target: weldModeGrid
                    onCurrentItemChanged:
                    {
                        if(index==weldModeGrid.currentIndex)
                        {
                            paramBoarderWidth=2
                        }
                        else
                        {
                            paramBoarderWidth=0
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar
            {
                parent: weldModeGrid
            }
        }
    }
}

