import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.eventlogging 1.0
import com.branson.menu 1.0

Item {
    readonly property string qmltextFailBatchCountMsg:qsTr("Modifying the batch count cause batch counter reset")
    readonly property string qmltextSaveMsg:qsTr("Production setup details are saved successfully")

    property bool isCycleCountUpdated
    property bool isProductionInstructionUpdated
    property bool isRecipeDescriptionUpdated

    property int recipeNamePanelHeight:50
    property int rootHeight:995
    property int root_panels_height: 945
    property int rightPanelWidth : 900
    property int rightPanelHeight: 720
    property int rightPanelX: 980
    property int rightPanelY: 80
    property int recipeNameWidth:160
    property int recipeNameHeight:40
    property int recipeDescriptionLabelWidth: 200
    property int recipeDescriptionLabelHeight: 50
    property int recipeDescriptionValueLabelY: 390
    property int recipeDescriptionValueY: recipeDescriptionValueLabelY+70
    property int recipeNameX:leftPanelX+leftElementsStartX
    property int saveButtonX: rightPanelX+rightPanelWidth/2+15//220
    property int saveButtonY: 820
    property string temp_CycleCount: ""
    property string temp_ProductionInstruction: ""
    property string temp_RecipeDescription: ""
    property alias text_cyclecount: text_cyclecount
    property int textAreaProductionSetUpWidth:760
    property int textAreaProductionSetUpHeight:200

    property int leftPanelWidth : 900
    property int leftPanelHeight: 720
    property int leftPanelX: 40
    property int leftPanelY: 80
    property int leftElementsStartX : 80

    property int productionInstructionLabelWidth: 240
    property int productionInstructionLabelHeight: 50
    property int productionInstructionLabelY: 55
    property int productionInstructionValueY: productionInstructionLabelY+70


    property int batchCountLabelY: 35
    property int buttonCancel_Save_Width: 180

    property int cycleCountLabelWidth: 150
    property int cycleCountLabelHeight: 50
    property int cycleCountLabelY: 420
    property int cycleCountRectY: cycleCountLabelY+60
    property int cycleCountRectWidth:760
    property int cycleCountRectHeight : 120
    property int cancelButtonX: rightPanelX+rightPanelWidth/2-buttonCancel_Save_Width-15
    property int cancelButtonY: 820
    property int cycleCountValueWidth:100
    property int cycleCountValueHeight : 40
    property int windowWidth:1920
    property int retVal: 0
    property bool isCycleCountResetted: false
    property MessagePopup mPopup
    /*
     @brief: Production information set has been saved into database
    */
    function saveProductionSetUpInfo()
    {
        isCycleCountUpdated = false
        isProductionInstructionUpdated = false
        isRecipeDescriptionUpdated = false

        retVal = recipeSetupInfo.checkBatchCount()
        if(retVal == 1)
        {
            saveValuesToDB()
        }
        else if(retVal == 2)
        {
            var component = Qt.createComponent("MessagePopup.qml")
            mPopup = component.createObject(window)
            mPopup.open(mPopup.info_title,qmltextFailBatchCountMsg, mPopup.ok_and_cancel_button)
            mPopup.onOkay.connect(function okay()
            {
                saveValuesToDB()
                recipeSetupInfo.sendResetBatchCounterReqToSC()
                mPopup.close()
                mPopup.onOkay.disconnect(okay)
                mPopup.destroy()
            })
            mPopup.onCancel.connect(function cancel()
            {
                recipeSetupInfo.replaceBatchCountWithOldVal()
                saveValuesToDB()
                mPopup.close()
                mPopup.onCancel.disconnect(cancel)
                mPopup.destroy()
            })
        }
        if(isCycleCountResetted == true)
        {
            Eventlogger.eventSend(EventLogging.UI_EVENT_CYCLE_COUNT_RESETTED);
            isCycleCountResetted = false
        }
    }
    /*
      @brief: save all values to database
      */
    function saveValuesToDB()
    {
        recipeSetupInfo.setCycleCount(temp_CycleCount)
        recipeSetupInfo.setProductionInstruction(temp_ProductionInstruction)
        recipeSetupInfo.setRecipeDescription(temp_RecipeDescription)
        recipeSetupInfo.savetoDB();
        recipe.updateStatusText(qmltextSaveMsg)
        if(titlbaronclicked == "Dashboard.qml")
            dashboard.init();
        else
            recipe.loadRecipeCards()
        loaderelement.setSource(titlbaronclicked);
        titlebarchild.visible=false
    }

    Rectangle {
        id: rectangle_recipe_name
        width: parent.width
        height: (recipeNamePanelHeight/rootHeight)*parent.height
        color: "#ffffff"
        border.color: "#ececec"
        border.width: 1
        Text {
            id: recipe_name
            x: (recipeNameX/windowWidth)*parent.width
            y: parent.height/2-height/2
            width: (recipeNameWidth/windowWidth)*parent.width
            height: (recipeNameHeight/recipeNamePanelHeight)*parent.height
            text:dashboard.m_RecipeNumber+" : "+dashboard.m_RecipeName
            font.bold:true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment:Text.AlignLeft
            font.family: Style.semibold.name
            font.pixelSize: Style.style5
            color:"#757575"
            Component.onCompleted:
            {
                text= recipeSetupInfo.getRecipeNumber()+ " : "+recipeSetupInfo.getRecipeName()
            }
        }
    }//Recipe Name

    Rectangle {
        id: rectangle
        y: (recipeNamePanelHeight/rootHeight)*parent.height
        width: parent.width
        height: (root_panels_height/rootHeight)*parent.height
        color: "#ffffff"
        DropShadow{
            anchors.fill: leftPanel
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: leftPanel
        }
        Rectangle {
            id: leftPanel
            x: (leftPanelX/windowWidth)*parent.width
            y: (leftPanelY/root_panels_height)*parent.height
            width: (leftPanelWidth/windowWidth)*parent.width
            height: (leftPanelHeight/root_panels_height)*parent.height
            color:"#80ECECEC"
            border.width: 0
            SecondaryButton
            {
                id: batch_setup_btn
                x: (leftElementsStartX/leftPanelWidth)*parent.width
                y:(batchCountLabelY/leftPanelWidth)*parent.width
                text: qsTr("Batch Setup")
                onClicked:
                {
                    recipeSetupInfo.setCycleCount(temp_CycleCount)
                    recipeSetupInfo.setProductionInstruction(temp_ProductionInstruction)
                    recipeSetupInfo.setRecipeDescription(temp_RecipeDescription)
                    loaderelement.setSource("BatchSetup.qml",{"width":1920,"height":995})
                    titlebarchildtext.text = qsTr("BATCH SETUP")
                }
            }// Batch Setup Button
            Text {
                id: text_cycle_count
                x: (leftElementsStartX/leftPanelWidth)*parent.width
                y: (cycleCountLabelY/leftPanelHeight)*parent.height
                width: (cycleCountLabelWidth/leftPanelWidth)*parent.width
                height: (cycleCountLabelHeight/leftPanelHeight)*parent.height
                text: qsTr("Cycle Count")
                textFormat: Text.PlainText
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                color:"#868e96"
            }//Text Cycle Count

            Rectangle {
                id: rectangle_cyclecount
                x: (leftElementsStartX/leftPanelWidth)*parent.width
                y: (cycleCountRectY/leftPanelHeight)*parent.height
                width: (cycleCountRectWidth/leftPanelWidth)*parent.width
                height: (cycleCountRectHeight/leftPanelHeight)*parent.height
                color: "#ECECEC"
                radius:(1/height)*parent.height
                Text {
                    id: text_cyclecount
                    x: parent.width/2-width/2
                    y: parent.height/2-height/2
                    width: (cycleCountValueWidth/leftPanelWidth)*parent.width
                    height: (cycleCountValueHeight/cycleCountRectHeight)*parent.height
                    Component.onCompleted:
                    {
                        text = recipeSetupInfo.getCycleCount()
                        temp_CycleCount = text
                        isCycleCountUpdated = true
                    }
                    onTextChanged:
                    {
                        if(isCycleCountUpdated)
                        {
                            temp_CycleCount=text
                        }
                    }
                    color:qmlPrimaryColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: Style.regular.name
                    font.pixelSize: Style.style3
                }
            }// Data Cycle Count

            SecondaryButton {
                id: prod_reset
                x: rectangle_cyclecount.x+rectangle_cyclecount.width-width
                y: (rectangle_cyclecount.y+rectangle_cyclecount.height+parent.height)/2-height/2
                text: qsTr("RESET")
                enabled: isPermissionAllowed(Features.RESET_WELD_COUNTER) ? true : false
                buttonColor:isPermissionAllowed(Features.RESET_WELD_COUNTER) ? "#ECECEC" : "#757575"
                textColor:isPermissionAllowed(Features.RESET_WELD_COUNTER) ? "#212529" : "#ffffff"
                onClicked: {
                    temp_CycleCount=0;
                    isCycleCountResetted = true
                    text_cyclecount.text=0
                }
            }
        }
        Flickable{
            id: rightPanelFlick
            width: parent.width
            height: parent.height
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            interactive: Qt.inputMethod.visible ? true : false
            bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
            clip: true
            DropShadow{
                anchors.fill: rightPanel
                horizontalOffset: 3
                verticalOffset: 3
                color: "#000000"
                opacity: 0.2
                source: rightPanel
            }
            Rectangle {
                id: rightPanel
                x: (rightPanelX/windowWidth)*parent.width
                y: (rightPanelY/root_panels_height)*parent.height
                width: (rightPanelWidth/windowWidth)*parent.width
                height: (rightPanelHeight/root_panels_height)*parent.height
                color:"#80ECECEC"
                Text {
                    id: text_production_instruction
                    x: parent.width/2-width/2
                    y: (productionInstructionLabelY/rightPanelWidth)*parent.width
                    width: (productionInstructionLabelWidth/rightPanelWidth)*parent.width
                    height: (productionInstructionLabelHeight/rightPanelHeight)*parent.height
                    text: qsTr("Production Instruction")
                    font.bold: true
                    color:"#868e96"
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }
                Rectangle{
                    id: rec
                    x: parent.width/2-width/2
                    y: (productionInstructionValueY/rightPanelHeight)*parent.height
                    width: (textAreaProductionSetUpWidth/rightPanelWidth)*parent.width
                    height: (textAreaProductionSetUpHeight/rightPanelHeight)*parent.height
                    border.color: qmlPrimaryColor
                    border.width: 1
                    Flickable {
                        id: flickable1
                        anchors.fill: parent
                        TextArea.flickable: TextArea {
                            id: textArea_production_instruction
                            color:qmlPrimaryColor
                            focus: false
                            background:Rectangle {
                                anchors.fill: parent
                                border.color:qmlPrimaryColor
                            }
                            wrapMode: Text.WordWrap
                            Component.onCompleted: {
                                text = recipeSetupInfo.getProductionInstruction()
                                temp_ProductionInstruction=text
                                isProductionInstructionUpdated=true
                            }
                            onTextChanged: {
                                if(isProductionInstructionUpdated){
                                    temp_ProductionInstruction=text
                                }

                            }
                            font.family: Style.regular.name
                            font.pixelSize: Style.style3
                            onFocusChanged:
                            {
                                var isfocus = textArea_production_instruction.focus
                                if(isfocus)
                                    rightPanelFlick.contentY = rightPanel.height * (20/100)
                            }
                        }
                        ScrollBar.vertical: ScrollBar { }
                    }
                }
                Text {
                    id: text_recipe_description
                    x: parent.width/2-width/2
                    y: (recipeDescriptionValueLabelY/rightPanelHeight)*parent.height
                    width: (recipeDescriptionLabelWidth/rightPanelWidth)*parent.width
                    height: (recipeDescriptionLabelHeight/rightPanelHeight)*parent.height
                    text: qsTr("Recipe Description")
                    color:"#868e96"
                    font.bold: true
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }
                Rectangle{
                    x: parent.width/2-width/2
                    y: (recipeDescriptionValueY/rightPanelHeight)*parent.height
                    width: (textAreaProductionSetUpWidth/rightPanelWidth)*parent.width
                    height: (textAreaProductionSetUpHeight/rightPanelHeight)*parent.height
                    border.color: qmlPrimaryColor
                    border.width: 1
                    Flickable {
                        id: flickable2
                        anchors.fill: parent
                        TextArea.flickable:TextArea {
                            id: textArea_recipe_description
                            focus: false
                            color:qmlPrimaryColor
                            wrapMode: Text.WordWrap
                            font.family: Style.regular.name
                            font.pixelSize: Style.style3
                            background:Rectangle {
                                anchors.fill: parent
                                border.color:qmlPrimaryColor
                            }
                            Component.onCompleted: {
                                text = recipeSetupInfo.getRecipeDescription()
                                temp_RecipeDescription = text
                                isRecipeDescriptionUpdated=true
                            }

                            onTextChanged: {
                                if(isRecipeDescriptionUpdated){
                                    temp_RecipeDescription = text
                                }
                            }
                            onFocusChanged:
                            {
                                var isfocus = textArea_recipe_description.focus
                                if(isfocus)
                                    rightPanelFlick.contentY = rightPanel.height * (60/100)
                            }
                        }
                        ScrollBar.vertical: ScrollBar { }
                    }
                }
            }
        }
        RowLayout{
            id:downButtonsId
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.right: parent.right
            anchors.rightMargin: 120
            spacing: 30
            SecondaryButton {
                id: prod_cancel
                text: qsTr("CANCEL")
                onClicked:
                {
                    loaderelement.setSource(titlbaronclicked);
                    titlebarchild.visible=false
                    recipe.loadRecipeCards()
                }
            }
            PrimaryButton {
                id: prod_save
                text: qsTr("SAVE")
                onClicked:
                {
                    saveProductionSetUpInfo()
                }
            }
        }
    }
}

