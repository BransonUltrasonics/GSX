import QtQuick 2.0
import QtQuick.Controls 2.2
import com.branson.menu 1.0

Item {
    property int modelIndex:0
    property int recipeIndex
    GridView {

        id: grid
        x: 0
        y: 50
        width: parent.width
        height: parent.height - 50
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection

        cellHeight: grid.height/5
        cellWidth: grid.width/2
        clip:true
        model: modelAfterBurstParams
        property bool refreshSwitch: false
        delegate: WeldModeItem
        {
            paramText:model.modelData.ParameterName
            paramTextLocale:model.modelData.ParameterLocaleName
            paramValue:model.modelData.ParameterValue
            paramUnit:model.modelData.ParameterUnit
            isSwitch:model.modelData.ParameterSwitch
            checkStatus:model.modelData.ParameterSwitchState
            height:  labelSize
            width:  grid.cellWidth-80
            Component.onCompleted:
            {
                toggleSwitch.enabled = isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX)
            }
            toggleSwitch.onPressedChanged:
            {
                grid.refreshSwitch = true
            }
            toggleSwitch.onCheckedChanged:
            {
                var currentIndex = index
                if(grid.refreshSwitch == true)
                {
                    recipe.setParamaterChangedFlag(1)
                    grid.refreshSwitch = false
                    if(toggleSwitchChecked)
                        recipe.setStructureValue(paramText,1)
                    else
                        recipe.setStructureValue(paramText,0)
                }
            }
            control_mode_mouseArea.onClicked:
            {
                if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                {
                    if(model.modelData.ParameterName != "Afterburst")
                    {
                        steppingNumPad.parameterText = model.modelData.ParameterName
                        steppingNumPad.parameterLocaleText = model.modelData.ParameterLocaleName
                        steppingNumPad.unitText = model.modelData.ParameterUnit
                        steppingNumPad.keyInputText = model.modelData.ParameterValue
                        steppingNumPad.minValue = model.modelData.ParameterMinValue
                        steppingNumPad.maxValue = model.modelData.ParameterMaxValue
                        modelIndex=index;
                        steppingNumPad.checkParameter( model.modelData.ParameterName )
                        numPadStatus = 1
                        steppingNumPad.open()
                        enableMainwindowOpacity.start()
                        steppingNumPad.dialogueModuleIndex=modelIndex
                        steppingNumPad.dialoguerecipeIndex=recipeIndex
                        steppingNumPad.editrecipeflag= false
                        steppingNumPad.addrecipeflag= false
                        steppingNumPad.paramAtoZflag= false
                        steppingNumPad.editHornRecipeFlag= false
                        steppingNumPad.ultrasonicAdvancedFlag=false
                        steppingNumPad.afterBurst = true
                        steppingNumPad.preTrigger = false
                    }
                }
            }
        }

        ScrollBar.vertical: ScrollBar
        {
            parent: grid
            anchors.top: grid.top
            anchors.bottom: grid.bottom
        }
    }
}
