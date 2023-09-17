import QtQuick 2.5
import QtQuick.Controls 2.2
import com.branson.menu 1.0

Item
{
    Component.onCompleted:
    {
        StackRecipeObj.init();
    }
    GridView
    {
        id: grid
        x: 50
        y: 50
        width: parent.width-50
        height: parent.height-50
        cellHeight: height/5
        cellWidth: width/3
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        model:stackRecipeParameterModel
        delegate: WeldModeItem
        {
            paramText:model.modelData.parameterName
            paramTextLocale:model.modelData.parameterName
            paramValue:model.modelData.parameterValue
            paramUnit:model.modelData.parameterUnit
            height:  labelSize
            width:  grid.cellWidth-80

            control_mode_mouseArea.onClicked:
            {
                if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                {
                    steppingNumPad.parameterText = model.modelData.parameterName
                    steppingNumPad.parameterLocaleText = model.modelData.parameterName
                    steppingNumPad.unitText = model.modelData.parameterUnit
                    steppingNumPad.keyInputText = model.modelData.parameterValue
                    steppingNumPad.minValue = ""//model.modelData.parameterMinValue
                    steppingNumPad.maxValue = ""//model.modelData.parameterMaxValue
                    steppingNumPad.checkParameter( model.modelData.parameterName )
                    numPadStatus = 1
                    steppingNumPad.open()
                    enableMainwindowOpacity.start();
                    steppingNumPad.dialoguerecipeIndex=index;
                    steppingNumPad.editrecipeflag= false
                    steppingNumPad.addrecipeflag= false
                    steppingNumPad.paramAtoZflag= false
                    steppingNumPad.editHornRecipeFlag= false
                    steppingNumPad.editSeekRecipeFlag= false
                    steppingNumPad.editStackRecipeFlag= true
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
