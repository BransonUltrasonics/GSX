import QtQuick 2.0
import QtQuick.Controls 2.2
import com.branson.menu 1.0
Item
{
    readonly property string qmltextCoolingValve : qsTr("Extra Cooling")
    readonly property string qmltextAfterBurst : qsTr("Afterburst")
    readonly property string qmltextPreTrigger : qsTr("Pretrigger")

    property int modelIndex:0
    property int recipeIndex: recipeIndex
    property int recipemofifyFlag: 0

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
            model: modelParamAtoZ
            property bool refreshSwitch: false
            delegate: WeldModeItem
            {
                paramText:model.modelData.ParameterName
                paramTextLocale:model.modelData.ParameterLocaleName
                paramValue:model.modelData.ParameterValue
                paramUnit:model.modelData.ParameterUnit
                isSwitch:model.modelData.ParameterSwitch
                isDoubleSwitch: model.modelData.ParameterDoubleSwitch
                checkStatus:model.modelData.ParameterSwitchState
                isTripleButton: model.modelData.ParameterTripleSwitch
                doubleLeftText:model.modelData.DoubleSwitchLeftText
                doubleRightText:model.modelData.DoubleSwitchRightText
                height:  labelSize
                width:  grid.cellWidth-80
                Component.onCompleted:
                {
                    toggleSwitch.enabled = isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX)
                    toggleDoubleSwitch.enabled = isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX)
                    tripleRadioButtons.enabled = isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX)
                    if(model.modelData.ParameterName == "Weld Force Control")
                    {
                        if(paramValue == "1")
                        {
                            tripleRadioButtons.customRadioBtnLowStatus = true
                            tripleRadioButtons.customRadioBtnMedStatus = false
                            tripleRadioButtons.customRadioBtnHighStatus = false
                        }
                        if(paramValue == "2")
                        {
                            tripleRadioButtons.customRadioBtnLowStatus = false
                            tripleRadioButtons.customRadioBtnMedStatus = true
                            tripleRadioButtons.customRadioBtnHighStatus = false
                        }
                        if(paramValue == "3")
                        {
                            tripleRadioButtons.customRadioBtnLowStatus = false
                            tripleRadioButtons.customRadioBtnMedStatus = false
                            tripleRadioButtons.customRadioBtnHighStatus = true
                        }
                    }
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
                        if((model.modelData.ParameterName != "Extra Cooling") ||
                                (model.modelData.ParameterName != "Afterburst") ||
                                (model.modelData.ParameterName != "Pretrigger"))
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
                            steppingNumPad.paramAtoZflag= true
                            steppingNumPad.editHornRecipeFlag= false
                            steppingNumPad.ultrasonicAdvancedFlag=false
                            steppingNumPad.afterBurst = false
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
}
