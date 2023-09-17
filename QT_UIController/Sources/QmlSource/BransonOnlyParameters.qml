import QtQuick 2.0
import QtQuick.Controls 2.2
import com.branson.menu 1.0

Item
{
    id:mainComponent
    property int modelIndex:0
    property int recipeIndex: recipeIndex

    Rectangle {
        anchors.fill: parent
        color: "#F8F9FA"
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
            clip: true
            cacheBuffer : 0
            model:bransonOnlyModel
            delegate: WeldModeItem
            {
                paramText:model.modelData.ParameterName
                paramTextLocale:model.modelData.ParameterLocaleName
                paramValue:model.modelData.ParameterValue
                paramUnit:model.modelData.ParameterUnit
                height:  labelSize
                width:  grid.cellWidth-80
                control_mode_mouseArea.onClicked:
                {
                    if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                    {
                        steppingNumPad.parameterText = model.modelData.ParameterName
                        steppingNumPad.parameterLocaleText = model.modelData.ParameterLocaleName
                        steppingNumPad.unitText = model.modelData.ParameterUnit
                        steppingNumPad.keyInputText = model.modelData.ParameterValue
                        steppingNumPad.minValue = model.modelData.ParameterMinValue
                        steppingNumPad.maxValue = model.modelData.ParameterMaxValue
                        numPadStatus = 1
                        modelIndex = index
                        steppingNumPad.dialogueModuleIndex=modelIndex
                        steppingNumPad.dialoguerecipeIndex=recipeIndex
                        steppingNumPad.checkParameter( model.modelData.ParameterName )
                        steppingNumPad.open()
                        enableMainwindowOpacity.start()
                        steppingNumPad.bransonOnlyFlag = true
                    }
                }
            }
            ScrollBar.vertical: ScrollBar{}
        }
    }
}

