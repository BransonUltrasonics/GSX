import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.menu 1.0

Item
{
    property string dialogText : ""
    property string modelValue: "0"
    property int modelIndex:0
    property int recipeIndex: recipeIndex
    property var  dialogPosition : 0
    property bool isaddrecipeitemclicked: false

    id:recipeitem

    property var sprite:0;
    Component.onCompleted: {
        suspectReject.recipeIndex = recipeIndex
    }
    DropShadow{
        source: rectangle1
        anchors.fill: rectangle1
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        radius: 3
    }
    Rectangle {
        id: rectangle1
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#f8f9fa"
        border.width: 2
        border.color: "#f8f9fa"

        GridView {
            id: gridviewrecipeparameter
            width: (240/260)*parent.width/*Adjustment in width and leftmargin to overcome overlapping*/
            height: (560/750)*parent.height
            anchors.topMargin: (10/750)*parent.height
            anchors.top: parent.top
            anchors.leftMargin: (0.5/260)*parent.width
            anchors.left: parent.left
            interactive: false
            cellWidth: width
            cellHeight: (1/8)*height
            focus: true
            model: appModel
            delegate: Item
            {
                readonly property string qmltextReactivity : "REACTIVITY"
                readonly property string qmltextForceLevel : "FORCE LEVEL"

                Component.onCompleted: {

                    if(qmltextReactivity === model.modelData.ParameterName || qmltextForceLevel === model.modelData.ParameterName){
                        rectanglegridparameter.visible = model.modelData.ParameterVisible ? true:false
                        rectanglegridparameterDrp.visible = model.modelData.ParameterVisible ? true:false
                    }
                    else
                        rectanglegridparameter.visible = true

                }

                width: parent.width; height: (35/360)*parent.height
                DropShadow{
                    id:rectanglegridparameterDrp
                    source: rectanglegridparameter
                    anchors.fill: rectanglegridparameter
                    horizontalOffset: 1
                    verticalOffset: 1
                    color: "#000000"
                    opacity: 0.2
                    radius: 3
                }
                Rectangle
                {
                    id: rectanglegridparameter
                    anchors.left: parent.left
                    width: parent.width
                    height: parent.height
                    color: "#E9ECEF"
                    Text {
                        id:delegateName
                        y: rectanglegridparameter.height/2-height/2
                        text: model.modelData.ParameterLocaleName
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style2
                        width:rectanglegridparameter.width*(70/100)
                        elide: Text.ElideRight
                        color: "#757575"
                        anchors.left: rectanglegridparameter.left
                        anchors.leftMargin: rectanglegridparameter.width*(3/100)
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        id:valueid
                        width:rectanglegridparameter.width*(12/100)
                        y: rectanglegridparameter.height/2-height/2
                        color: qmlPrimaryColor
                        text: model.modelData.ParameterValue
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style1
                        anchors.left: delegateName.right
                        anchors.leftMargin:rectanglegridparameter.width*(2/100)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignRight
                    }
                    Text {
                        id:delegateUnit
                        y: rectanglegridparameter.height/2-height/2
                        color: "#757575"
                        width: rectanglegridparameter.width*(6/100)
                        text: model.modelData.ParameterUnit
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style1
                        anchors.left: valueid.right
                        anchors.leftMargin: rectanglegridparameter.width*(1/100)
                    }
                    MouseArea {
                        anchors.fill: rectanglegridparameter
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX) || (parseInt(recipe.getStructureValueByName("global setup")) == 1))
                            {
                                var userLevel = login.getUserLevel();
                                if (userLevel == "Operator")
                                {
                                  if((model.modelData.ParameterName=="Time")||(model.modelData.ParameterName=="Energy")
                                    ||(model.modelData.ParameterName=="Peak power")||(model.modelData.ParameterName=="Scrub Time")
                                    ||(model.modelData.ParameterName=="Absolute Distance")||(model.modelData.ParameterName=="Collapse Distance")
                                    || (model.modelData.ParameterName=="Density"))
                                  {
                                    if (parseInt(recipe.getStructureValueByName("setup weld mode status"))==0)
                                     {   return;}
                                     else
                                    {
                                        model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup weld mode high")
                                        model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup weld mode low")
                                    }
                               }
                              else if (model.modelData.ParameterName=="WELD AMPLITUDE")
                              {
                                    if (parseInt(recipe.getStructureValueByName("setup weld amplitude status"))==0)
                                     {    return;}
                                     else
                                     {
                                       modelData.ParameterMaxValue= recipe.getStructureValueByName("setup weld amplitude high")
                                       model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup weld amplitude low")
                                     }

                              }
                              else  if (model.modelData.ParameterName=="WELD FORCE")
                              {

                                    if (parseInt(recipe.getStructureValueByName("setup weld force status"))==0)
                                     {  return;}

                                  }
                                  else if (model.modelData.ParameterName=="TRIGGER FORCE")
                                  {

                                    if (parseInt(recipe.getStructureValueByName("setup trigger force status"))==0)
                                     {  return;}
                                     else
                                     {
                                        model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup trigger force high")
                                        model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup trigger force low")
                                    }
                                }
                                else if (model.modelData.ParameterName=="HOLD FORCE")
                                {

                                    if (parseInt(recipe.getStructureValueByName("setup hold force status"))==0)
                                    {   return;}
                                    else
                                     {
                                       model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup hold force high")
                                       model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup hold force low")
                                     }
                                }
                                else if (model.modelData.ParameterName=="HOLD TIME")
                                {

                                    if (parseInt(recipe.getStructureValueByName("setup hold time status"))==0)
                                    {   return;}
                                    else
                                    {
                                         model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup hold time high")
                                         model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup hold time low")
                                    }
                                }
                                else if (model.modelData.ParameterName=="FORCE RAMP TIME")
                                {
                                  return;
                                }
                                  else if (model.modelData.ParameterName=="REACTIVITY")
                                  {

                                      if (parseInt(recipe.getStructureValueByName("setup reactivity status"))==0)
                                      {   return;}
                                      else
                                      {
                                           model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup reactivity high")
                                           model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup reactivity low")
                                      }
                                  }
                                  else if (model.modelData.ParameterName=="FORCE LEVEL")
                                  {

                                      if (parseInt(recipe.getStructureValueByName("setup force level status"))==0)
                                      {   return;}
                                      else
                                      {
                                           model.modelData.ParameterMaxValue= recipe.getStructureValueByName("setup force level high")
                                           model.modelData.ParameterMinValue= recipe.getStructureValueByName("setup force level low")
                                      }
                                  }



                            }

                               if(isaddrecipeitemclicked) //&& (recipe.getStructureValueByName(steppingNumPad.parameterText)) == 40)
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
                                    steppingNumPad.editrecipeflag= false
                                    steppingNumPad.addrecipeflag= true
                                    steppingNumPad.paramAtoZflag= false
                                    steppingNumPad.editHornRecipeFlag= false
                                    steppingNumPad.afterBurst = false
                                }
                                else
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
                                    steppingNumPad.editrecipeflag= true
                                    steppingNumPad.addrecipeflag= false
                                    steppingNumPad.paramAtoZflag= false
                                    steppingNumPad.editHornRecipeFlag= false
                                    steppingNumPad.ultrasonicAdvancedFlag=false
                                    steppingNumPad.afterBurst = false
                                    steppingNumPad.preTrigger = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
