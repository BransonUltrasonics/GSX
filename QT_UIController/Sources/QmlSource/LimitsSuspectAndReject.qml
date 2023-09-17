import QtQuick 2.0
import QtQuick.Controls 2.2
import Style 1.0
import QtGraphicalEffects 1.0
import com.branson.menu 1.0

Item
{
    readonly property string qmltextSuspectRejectLimits : qsTr(" : SUSPECT & REJECT LIMITS")
    readonly property string qmlTextInvalidSetup: qsTr("Setup/Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidControl: qsTr("Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidParam: qsTr("Parameter Values are outside the Suspect/Reject limits! Modify suitably")
    readonly property string qmltextEneterValidLimitsMsg: qsTr("Please enter valid limits to continue")

    id: limitsSuspectRejectItem
    property real gridHeight:870
    property real rectWidth:480.6
    property real parentwidth:1350
    property int modelIndex: 0
    readonly property string unitTextColor: "#898D96"
    readonly property string bgColor: "#F8F9FA"
    readonly property string titleBGColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"
    readonly property string highlightColor: "#757575"
    readonly property string lowColor: "#D9E8FA"
    readonly property string highColor: qmlSecondaryColor
    property int recipemofifyFlag: 0

    signal modifiedRecipeforLimit(var flagdata)

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
        property bool refreshSwitch: false
        model:modelSuspectRejectLimits
        delegate: LimitsSuspectAndRejectItem{
            paramText:model.modelData.ParameterName
            paramTextLocale:model.modelData.ParameterLocaleName
            suspectLow:model.modelData.SuspectLow
            suspectHigh: model.modelData.SuspectHigh
            rejectLow: model.modelData.RejectLow
            rejectHigh: model.modelData.RejectHigh
            rejectStatus : model.modelData.RejectStatus
            suspectStatus: model.modelData.SuspectStatus
            isSwitch:model.modelData.ParameterSwitch
            checkStatus: model.modelData.ParameterSwitchState
            suspectLowStatus:model.modelData.SuspectLowStatus
            suspectHighStatus:model.modelData.SuspectHighStatus
            rejectLowStatus: model.modelData.RejectLowStatus
            rejectHighStatus:model.modelData.RejectHighStatus
            height: labelSize
            width:  grid.cellWidth-30
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
                var dwIndex = index
                if(grid.refreshSwitch == true)
                {
                    grid.refreshSwitch = false
                    var bRetVal = 0
                    if(toggleSwitchChecked)
                    {
                        if(model.modelData.ParameterName == "Global Suspect")
                            bRetVal = recipe.midPointValidationForAllParams(true,false,false)
                        if(model.modelData.ParameterName == "Global Reject")
                            bRetVal = recipe.midPointValidationForAllParams(false,true,false)
                    }

                    if(bRetVal == 1 || bRetVal == 4 || bRetVal == 5)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmlTextInvalidParam)
                    }
                    else if(bRetVal == 2)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
                    }
                    else if(bRetVal == 3)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmlTextInvalidSetup)
                    }
                    recipe.setParamaterChangedFlag(2)
                    if(toggleSwitchChecked)
                        recipe.setStructureValue(model.modelData.ParameterName,1)
                    else
                        recipe.setStructureValue(model.modelData.ParameterName,0)
                }
            }
            suspect_Reject_mouseArea.onClicked:
            {
                if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                {
                    if((model.modelData.ParameterName != "Global Suspect") ||
                            (model.modelData.ParameterName != "Global Reject"))
                    {
                        suspectReject.suspectRejectheader = paramTextLocale /*+ qmltextSuspectRejectLimits*/
                        suspectReject.mode = paramText
                        suspectReject.focusedItem = 1
                        suspectReject.setLimitValues(model.modelData.SuspectLow,
                                                     model.modelData.SuspectHigh,
                                                     model.modelData.RejectLow,
                                                     model.modelData.RejectHigh,
                                                     model.modelData.SuspectStatus,
                                                     model.modelData.RejectStatus,
                                                     model.modelData.SuspectLowStatus,
                                                     model.modelData.SuspectHighStatus,
                                                     model.modelData.RejectLowStatus,
                                                     model.modelData.RejectHighStatus)
                        modelIndex = index
                        suspectReject.open()
                        numPadStatus = 2
                        suspectReject.suspectRejectIndex = modelIndex
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


