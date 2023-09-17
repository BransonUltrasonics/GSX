import QtQuick 2.0
import QtQuick.Controls 2.2
import Style 1.0
import com.branson.menu 1.0

Item {
    readonly property string qmltextSetup : "Setup"
    readonly property string qmltextGroundDetect : "Ground Detect"
    readonly property string qmltextSetUpLOwHighLimits : qsTr("SETUP LOW & HIGH LIMITS")
    readonly property string qmlTextInvalidSetup: qsTr("Setup/Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidControl: qsTr("Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidParamSetup: qsTr("Parameter Values are outside the Setup limits! Modify suitably")
    readonly property string qmltextEneterValidLimitsMsg: qsTr("Please enter valid limits to continue")

    property real gridHeight:870
    property real rectWidth:480.6
    property real parentwidth:1350
    property int modelIndex: 0

    GridView {
        id: grid
        width: parent.width
        height: parent.height - 50
        y: 50
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        cellHeight: grid.height/5
        cellWidth: grid.width/2
        clip:true
        cacheBuffer: 0
        model: modelSetupLimits
        property bool refreshSwitch: false
        delegate: LimitsSetupItem{
            paramText:model.modelData.ParameterName
            paramTextLocale:model.modelData.ParameterLocaleName
            individualCheck:model.modelData.SetUpEnabled
            paramMinValue:model.modelData.SetUpMinValue
            paramMaxValue:model.modelData.SetUpMaxValue
            paramUnit:model.modelData.SetupUnit
            isweldEnable: model.modelData.SetUpWeld
            isSwitch: model.modelData.ParameterSwitch
            checkStatus: model.modelData.ParameterSwitchState
            isRec: model.modelData.ParameterRec
            weldmodeStr:model.modelData.WeldMode
            setupParameterVisible:model.modelData.SetUpParameterVisible
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
                    grid.refreshSwitch = false
                    var bRetVal = 0
                    if(toggleSwitchChecked)
                        bRetVal = recipe.midPointValidationForAllParams(false,false,true)

                    if(bRetVal == 2)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
                    }
                    else if(bRetVal == 3)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmlTextInvalidSetup)
                    }
                    else if(bRetVal == 6)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmlTextInvalidParamSetup)
                    }

                    recipe.setParamaterChangedFlag(3)
                    if(toggleSwitchChecked)
                        recipe.setStructureValue(paramText,1)
                    else
                        recipe.setStructureValue(paramText,0)
                }
            }
            setup_Limit_mouseArea.onClicked:
            {
                if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                {
                    if((paramText != qmltextSetup) || (model.modelData.ParameterName == qmltextGroundDetect))
                    {
                        numPadStatus = 3
                        modelIndex = index
                        setupNumpad.setLimitValues(paramMinValue,paramMaxValue,individualCheck)
                        setupNumpad.setupLimitheader = paramTextLocale /*+ " : " + qmltextSetUpLOwHighLimits*/
                        if(model.modelData.ParameterName == "Weld Mode")
                            setupNumpad.mode = model.modelData.WeldMode
                        else
                            setupNumpad.mode = model.modelData.ParameterName
                        setupNumpad.setupOnOffRec = true
                        setupNumpad.open()
                        setupNumpad.mpText = false
                        setupNumpad.wpText = false
                        enableMainwindowOpacity.start()
                    }
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            parent: grid
            anchors.top: grid.top
            anchors.left: grid.right
            anchors.bottom: grid.bottom
        }
    }
}
