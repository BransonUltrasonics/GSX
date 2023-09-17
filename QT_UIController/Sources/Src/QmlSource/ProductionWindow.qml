import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.openedscreen 1.0

Item
{
    readonly property string qmltextGood : qsTr("Cycles OK")
    readonly property string qmltextCycles : qsTr("Cycles")
    readonly property string qmltextBatchCount : qsTr("Batch Count: ")
    readonly property string qmltextPartPerMin : qsTr("Parts/Min")
    readonly property string qmltextPeakPower : qsTr("Peak Power")

    id: productionItem
    visible: true
    //anchors.fill: parent
    width: parent.width
    height: parent.height - 200
    property int weldStatus_x: 60
    property int weldStatus_y: 70
    property int weldStatus_BL_x: 40
    property int weldStatus_BL_y: 85
    property int weldStatus_BL_x1: weldStatus_BL_x*3.55
    property string goodPartsCount
    property int activeRecipeEditFlag: 0
    Component.onCompleted:
    {
        config.setActivePage(2)
        production.productionStatus(true)
        updateProductionPowerResults()
        refreshPartsPerMin.start()
        setGoodNumberFontSize(goodCountValue.text.length)
        setBatchCounterFontSize(id_batchCounter.text.length)
        id_batchCount.visible = production.m_batchSetupEnable
        activeRecipeEditFlag = recipe.getModifiedStatus(production.m_RecipeNumber)
    }
    Component.onDestruction:
    {
        production.productionStatus(false)
        refreshPartsPerMin.stop()
    }

    Timer
    {
        id:refreshPartsPerMin
        interval: 5000
        repeat: true
        triggeredOnStart: true
        onTriggered:
        {
            production.refreshPartsPerMin()
        }
    }

    /**
    *@breif: update the latest results
    **/
    function updateResults()
    {
        goodCountValue.text = production.GetGoodNum()
        setGoodNumberFontSize(goodCountValue.text.length)
        id_batchCounter.text = production.GetCycleCountNum()
        setBatchCounterFontSize(id_batchCounter.text.length)
        partPerMinValue.text = production.GetPartPerMinChanged()

        for(var i=0;i<200;i++)
        {
            canvasRepeater.itemAt(i).children[0].requestPaint()
        }
    }

    /**
    *@breif: Update production power results
    **/
    function updateProductionPowerResults()
    {
        control.value = production.m_weldProgressPerc/100
        id_weldProgressValue.text = production.m_peakPower+qsTr(" W")
        id_weldProgressValue.color= production.m_weldProgressPerc <= 50 ? "#805fff" : "#FFFFFF";
    }

    /**
    *@breif: To set batch counter size based on the good value
    **/
    function setBatchCounterFontSize(goodlength)
    {
        if(goodlength == 1 || goodlength == 2 || goodlength ==3)
        {
            id_batchCounter.font.pixelSize = Style.style10
            id_batchCounter.font.family = Style.regular.name
        }
        else if(goodlength == 4 || goodlength == 5)
        {
            id_batchCounter.font.pixelSize = 48//Style.style9
            id_batchCounter.font.family = Style.regular.name
        }
        else if(goodlength == 6)
        {
            id_batchCounter.font.pixelSize = 42//60//Style.style9
            id_batchCounter.font.family = Style.regular.name
        }
        else if(goodlength == 7 || goodlength == 8)
        {
            id_batchCounter.font.pixelSize = 34//48//Style.style9
            id_batchCounter.font.family = Style.regular.name
        }
    }

    /**
    *@breif: To set good number font size based on the length
    **/
    function setGoodNumberFontSize(length)
    {
        if(length == 1 || length == 2 || length ==3)
        {
            goodCountValue.font.pixelSize = Style.style10
            goodCountValue.font.family = Style.regular.name
        }
        else if(length == 4 || length == 5)
        {
            goodCountValue.font.pixelSize = 48//Style.style9
            goodCountValue.font.family = Style.regular.name
        }
        else if(length == 6)
        {
            goodCountValue.font.pixelSize = 42//60//Style.style9
            goodCountValue.font.family = Style.regular.name
        }
        else if(length == 7 || length == 8)
        {
            goodCountValue.font.pixelSize = 34//48//Style.style9
            goodCountValue.font.family = Style.regular.name
        }
    }

    function updateProductionScreen()
    {
    	//update production values
    	production.productionInit()
    	updateProductionPowerResults()
    	updateResults()
    	recipeName.text = (recipe.getModifiedStatus(production.m_RecipeNumber)?"* ":"") + production.m_RecipeNumber + ": " + production.m_RecipeName
        productControl.text = production.m_WeldMode + " : "
        productControlValue.text = production.m_ControlValue + " " + production.m_ControlValueUnit
    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateProductionScreen()
            messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.without_button)
        }
    }

    Rectangle
    {
        id: id_Prod_Main_Rect
        width:productionItem.width - 50
        height:productionItem.height - 136

        Rectangle
        {
            id:recipeDetailPane
            width: id_Prod_Main_Rect.width
            height:60
            color: "#FFFFFF"
            Text
            {
                id: recipeName
                width: (300/parent.width)*parent.width
                height: parent.height
                anchors.left:  parent.left
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Style.style5
                font.family: Style.regular.name
                color: "#757575"
                text:(activeRecipeEditFlag?"* ":"") + production.m_RecipeNumber + ": " + production.m_RecipeName
            }
            CarouselDots
            {
                id: carouselDots
                x: id_Prod_Main_Rect.width/2 - 10
                y: recipeDetailPane.height/2-height/2
                width: Math.sqrt((32/1920)*recipeDetailPane.width*(32/52)*recipeDetailPane.height)
                height: Math.sqrt((32/1920)*recipeDetailPane.width*(32/52)*recipeDetailPane.height)
                marLeft.onClicked:
                {
                }
                marRight.onClicked:
                {
                }
                visible: false
            }
            Text
            {
                id: productControlValue
                y: recipeDetailPane.height/2-height/2
                anchors.right: parent.right
                anchors.rightMargin: 10
                //font.bold: true
                font.pixelSize:  Style.style5
                font.family:  Style.semibold.name
                color: qmlPrimaryColor
                text:production.m_ControlValue + " " + production.m_ControlValueUnit
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            Text
            {
                id: productControl
                anchors.right: productControlValue.left
                y: recipeDetailPane.height/2-height/2
                color: "#757575"
                //font.bold: true
                font.pixelSize: Style.style5
                font.family:  Style.semibold.name
                text: production.m_WeldMode + " : "
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.Right
            }
        }
        Rectangle
        {
            id:weldingStratPane
            width:productionItem.width
            height: parent.height
            color: "#ECECEC"
            y:61
            anchors.top: recipeDetailPane.bottom
            //Good Count Displayer displayer Rectangle
            Rectangle
            {
                id: goodCountDisplayer
                width: weldingStratPane.width/3 //(427/1280)*parent.width
                height:weldingStratPane.height
                color: "#ECECEC"
                radius: 1
                Column
                {
                    x: goodCountDisplayer.width/4
                    y: weldingStratPane.height/3
                    Rectangle
                    {
                        id: goodCntRect
                        width: 240
                        height:40
                        color: qmlPrimaryColor
                        radius: 1
                        Text
                        {
                            id:goodCountHeading
                            width: parent.width
                            height:parent.height
                            text: qmltextGood
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            leftPadding: 25
                            color: "#FFFFFF"
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle
                    {
                        id: goodCntValRect
                        width: 240
                        height:200
                        color: "#F8F9FA"
                        radius: 1
                        anchors.top: goodCntRect.Bottom
                        Text
                        {
                            id: goodCountValue
                            width:parent.width
                            height:parent.height
                            //font.bold: true
                            //font.pixelSize: Style.style10
                            //font.family: Style.regular.name
                            color: "#757575"
                            text: production.m_goodNum
                            horizontalAlignment: Text.Center
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
            Rectangle
            {
                id: weldResultDisplayer
                width: weldingStratPane.width/3
                height:weldingStratPane.height
                color: "#ECECEC"
                anchors.left: goodCountDisplayer.right
                property int p_cur: 42
                Rectangle {
                    id: id_line
                    y:10
                    width: weldResultDisplayer.width
                    height: 2
                    LinearGradient
                    {
                        anchors.fill: parent
                        start: Qt.point(0, 0)
                        end: Qt.point(parent.width,0)
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#ebebec" }
                            GradientStop { position: 0.44; color: "#7f7f7f" }
                            GradientStop { position: 1.0; color: "#ebebec" }
                        }
                    }

                }

                Rectangle{
                    id: rect2
                    width: weldResultDisplayer.width
                    anchors.topMargin: 0
                    height: 100
                    color: "transparent"
                    Item {
                        anchors.fill: parent
                        Canvas {
                            id: id_canvas
                            anchors.fill: parent
                            contextType: "2d"
                            antialiasing: true

                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.lineWidth = 1;
                                ctx.strokeStyle = "#7f7f7f"
                                ctx.beginPath()
                                ctx.moveTo(weldStatus_BL_x, weldStatus_BL_y)
                                ctx.lineTo(weldStatus_BL_x*3.75,weldStatus_BL_y)
                                ctx.lineTo(weldStatus_BL_x1+25,weldStatus_BL_y+12)
                                ctx.lineTo(weldStatus_BL_x1+150,weldStatus_BL_y+12)
                                ctx.lineTo(weldStatus_BL_x1+163,weldStatus_BL_y)
                                ctx.lineTo(weldStatus_BL_x*10.5,weldStatus_BL_y)
                                ctx.stroke();
                            }
                        }
                    }

                }
                Text {
                    id: id_weldStatus
                    x: parent.width/2-width/2
                    y:1
                    text:production.m_WeldStatus
                    font.pixelSize: Style.style9
                    font.family: Style.semibold.name
                    color:  qmlPrimaryColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                }
                Rectangle
                {
                    id: id_rec1
                    property int p_Blocks: 200
                    property int p_curFPS: 0
                    x:weldStatus_x - 9
                    anchors.top: id_batchCount.bottom
                    anchors.topMargin: 0
                    height: Math.min(weldResultDisplayer.width, weldResultDisplayer.height) * 0.8
                    width: height
                    transformOrigin: Item.Center
                    rotation: 270
                    color: "transparent"

                    Canvas {
                        id: id_FirstCircle
                        anchors.fill: parent
                        contextType: "2d"
                        antialiasing: true
                        renderStrategy: Canvas.Threaded
                        renderTarget: Canvas.FramebufferObject
                        onPaint: {
                            // CIRCLE PART
                            var ctx = getContext('2d');
                            ctx.beginPath();
                            ctx.strokeStyle = "#757575";
                            ctx.lineWidth = 1;
                            ctx.arc(id_rec1.height / 2, id_rec1.height / 2, id_rec1.height /(2.35*0.94), 0,Math.PI * 2, false);
                            ctx.stroke();

                            ctx.beginPath();
                            ctx.strokeStyle = "#E9ECEF";
                            ctx.lineWidth = 20;
                            ctx.arc(id_rec1.height / 2, id_rec1.height / 2, id_rec1.height / 2.25*0.95, 0, Math.PI * 2, false);
                            ctx.stroke();

                            ctx.beginPath();
                            ctx.strokeStyle = "#5c72d5";
                            ctx.lineWidth = 12;
                            ctx.arc(id_rec1.height / 2, id_rec1.height / 2,  id_rec1.height / 2.25*0.85 , 0, Math.PI * 2, false);
                            ctx.stroke();
                        }
                    }
                    Repeater
                    {
                        id:canvasRepeater
                        model: id_rec1.p_Blocks

                        Item {
                            id:itemCanvas
                            anchors.fill: id_FirstCircle
                            Canvas {
                                property int p_cur: weldResultDisplayer.p_cur
                                id: id_canvas_progress
                                anchors.fill: parent
                                contextType: "2d"
                                antialiasing: true
                                renderStrategy: Canvas.Threaded
                                renderTarget: Canvas.FramebufferObject
                                onPaint: {
                                    // CIRCLE PART
                                    var context = getContext('2d');
                                    if(production.m_goodByGoalPerc < index)
                                    {
                                        context.strokeStyle = "#898D96";
                                    }
                                    else
                                    {
                                        context.strokeStyle = "#508F05";
                                    }

                                    context.lineWidth = 12;
                                    context.beginPath();
                                    context.arc(id_rec1.height / 2, id_rec1.height / 2, id_rec1.height / 3, index * Math.PI * 2 * (360 / id_rec1.p_Blocks / 360) + Math.PI * 2 * 0.003, index * Math.PI * 2 * (360 / id_rec1.p_Blocks / 360) + Math.PI * 2 * (360 / id_rec1.p_Blocks / 360), false);
                                    context.stroke();

                                    if(index == (id_rec1.p_Blocks - 1)){
                                        id_rec1.p_curFPS++
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id: innerRecText
                    width: parent.width*(10/100)
                    height: parent.height*(10/100)
                    anchors.horizontalCenter: id_rec1.horizontalCenter
                    anchors.verticalCenter: id_rec1.verticalCenter
                    color: "transparent"
                    rotation: 0
                    Text {
                        id: id_batchCounter
                        height: 100
                        anchors.verticalCenter: innerRecText.verticalCenter
                        anchors.horizontalCenter: innerRecText.horizontalCenter
                        text: production.m_cycleCount
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#8BC24A"
                    }
                    Text
                    {
                        id: cycle
                        anchors.top: id_batchCounter.bottom
                        anchors.topMargin: 0
                        anchors.horizontalCenter: innerRecText.horizontalCenter
                        font.pixelSize: Style.style6
                        font.family: Style.semibold.name
                        color: "#898D96"
                        text:qsTr("Cycle Count")
                        wrapMode: Text.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text {
                    id: id_batchCount
                    anchors.left: parent.left
                    anchors.leftMargin: 150
                    anchors.top:rect2.bottom
                    anchors.topMargin: 10
                    text: qmltextBatchCount + production.m_batchCount
                    font.pixelSize: Style.style6
                    font.family: Style.semibold.name
                    color: "#898D96"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    visible: false
                }
            }
            Rectangle
            {
                id: partPerMinDisplayer
                width: weldingStratPane.width/3
                height:weldingStratPane.height
                color: "#ECECEC"
                anchors.left: weldResultDisplayer.right
                radius: 1
                Column{

                    x: parent.width/4
                    y: parent.height/3
                    Rectangle
                    {
                        id: partPerMinRect
                        width: 240
                        height:40
                        color: qmlPrimaryColor
                        radius: 1
                        Text
                        {
                            id:partPerMinHeading
                            width: parent.width
                            height:parent.height
                            text: qmltextPartPerMin
                            color: "#FFFFFF"
                            font.pixelSize: Style.style1
                            font.family: Style.semibold.name
                            leftPadding: 25
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle
                    {
                        id: partPerMinValRect
                        x:partPerMinRect.x
                        width: 240
                        height:200
                        color: "#F8F9FA"
                        anchors.top: partPerMinRect.Bottom
                        radius: 1
                        Text
                        {
                            id: partPerMinValue
                            width:parent.width
                            height:parent.height
                            font.pixelSize: Style.style10
                            font.family: Style.regular.name
                            color: "#757575"
                            text: production.m_partsPerMin
                            horizontalAlignment: Text.Center
                            verticalAlignment: Text.AlignVCenter

                        }
                    }
                }
            }
        }
//        Rectangle
//        {
//            id:weldingStratBottomRec
//            width:productionItem.width
//            height: 65
//            anchors.top:weldingStratPane.bottom
////            anchors.topMargin: 0
//            color: "#ECECEC"
//        }
        Rectangle
        {
            id: id_WeldProgressBarRect
            width: productionItem.width
            height: 75
            anchors.top:weldingStratPane.bottom
            anchors.topMargin: 0//65
            color: "#D7DBDE"
            layer.enabled: true
            layer.effect: DropShadow{
                anchors.fill: id_WeldProgressBarRect
                horizontalOffset: 1
                verticalOffset: 1
                color: "#80000000"
                opacity: 0.2
                source: id_WeldProgressBarRect
                radius: 3
            }
            Text
            {
                id:id_WeldProgressBarLbl
                x:10
                y:5
                height: id_WeldProgressBarRect.height/2 - 10
                text :qmltextPeakPower
                font.pixelSize: Style.style3
                font.family: Style.semibold.name
                color: "#757575"
            }

            Rectangle
            {
                id: id_WeldProgressBar
                x:10
                y:27
                width: id_WeldProgressBarRect.width - 25
                height: id_WeldProgressBarRect.height/3
                ProgressBar {
                    id: control
                    width: id_WeldProgressBar.width
                    height: id_WeldProgressBar.height
                    value: production.m_weldProgressPerc/100 // aS ProgressBar EXCEPT value in fraction in range of 0.0-1.0, hence /100 is done.
                    padding: 2
                    background: Rectangle {
                        implicitWidth: parent.width
                        implicitHeight: parent.height
                        color: "#e6e6e6"
                        radius: 3
                    }

                    contentItem: Item {
                        implicitWidth: parent.width
                        implicitHeight: parent.height

                        Rectangle {
                            width: control.visualPosition * parent.width
                            height: parent.height
                            radius: 2
                            LinearGradient
                            {
                                anchors.fill: parent
                                start: Qt.point(0, 0)
                                end: Qt.point(parent.width,0)
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#805fff" }
                                    GradientStop { position: 1.0; color: "#69a1ec" }
                                }
                            }
                        }
                    }
                }
            }
            Text
            {
                id: id_weldProgressValue
                x: id_WeldProgressBar.width/2
                y: 29
                text:production.m_peakPower+qsTr(" W")
                font.pixelSize: Style.style3
                font.family: Style.semibold.name
                color: production.m_weldProgressPerc <= 50 ? "#805fff" : "#FFFFFF";
            }
        }
    }
}
