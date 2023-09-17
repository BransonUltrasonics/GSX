import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0
import QtCharts 2.2

Item {
    id: item_ProdOverview
    width:parent.width
    height: parent.height - mainStatusbarHeight
    x:0
    y:0
    property string firstDayOfWeek:"2018-04-09";
    property string lastDayOfWeek:"2018-04-15";
    property string averageRectTitle:qsTr("Daily Average");
    readonly property string qmltextSunday: qsTr("Sunday")
    readonly property string qmltextMonday: qsTr("Monday")
    readonly property string qmltextTuseday: qsTr("Tuseday")
    readonly property string qmltextWednesday: qsTr("Wednesday")
    readonly property string qmltextThursday: qsTr("Thursday")
    readonly property string qmltextFriday: qsTr("Friday")
    readonly property string qmltextSaturday: qsTr("Saturday")

    property alias weeklyDate: id_DateDisplayerText.text
    //    ListModel {
    //        id: productionModel
    //        ListElement { propertyname: qsTr("Weld") ;SunVal:"642";MonVal:"659";TueVal:"658";WedVal:"639";ThrVal:"656";FriVal:"643";SatVal:"631";AvgVal:"643";unit: qsTr("Reject Limits");barColor:"#8BC24A";}
    //        ListElement { propertyname: qsTr("Rate") ;SunVal:"9";MonVal:"10";TueVal:"12";WedVal:"8";ThrVal:"10";FriVal:"8";SatVal:"7";AvgVal:"9";unit: "Part/Min";barColor:qmlPrimaryColor;}
    //        ListElement { propertyname: qsTr("Alarms") ;SunVal:"10";MonVal:"8";TueVal:"12";WedVal:"10";ThrVal:"16";FriVal:"18";SatVal:"10";AvgVal:"13";unit: "Errors";barColor:"#F59F00";}
    //    }
    BusyIndicator {
        id:busyindicatiorId
        anchors.verticalCenter:parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        running: true

    }
    Component.onCompleted: {
        firstDayOfWeek = analyticsproduction.startDateofCurrentWeek()
        lastDayOfWeek = analyticsproduction.endDateofCurrentWeek()
        getProductionOverView(firstDayOfWeek,lastDayOfWeek)

    }
    function getProductionOverView(fromDate,toDate) {
        analyticsproduction.setAnalyticsDateProductionList(fromDate,toDate)
        busyindicatiorId.visible = false
    }

    Rectangle
    {
        id:id_ProductionOverviewRect
        y:15
        width: item_ProdOverview.width
        height: item_ProdOverview.height
        color: "#ECECEC"

        ListView
        {
            id: idListView_ProdOverView
            anchors.fill: parent
            model:productionModel
            delegate:id_ProdOverComponent
            spacing: 10
            y:45
            height: item_ProdOverview.height
            boundsBehavior: Flickable.StopAtBounds
           interactive:false
        }
        Component
        {
            id: id_ProdOverComponent

            Rectangle
            {
                id:id_ProdOverComponentRect
                width: item_ProdOverview.width
                height:item_ProdOverview.height/3
                color:"#E9ECEF"
                y:20
                Rectangle{
                    id: id_PropertyNameRect
                    width:(120/1280)*parent.width
                    height: parent.height -10
                    y:5
                    color: "#E9ECEF"
                    Text
                    {
                        id: id_PropertyNameText
                        width: parent.width
                        height: parent.height
                        text: model.modelData.productionName
                        font.pixelSize: Style.style2
                        font.family: Style.regular.name//Style.regular.name
                        color: "#757575"
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Rectangle
                {
                    id: id_ProdOverChartView
                    width:(950/1280)*parent.width
                    height: parent.height - 10
                    color: "#ECECEC"
                    y:5
                    anchors.left: id_PropertyNameRect.right
                    ChartView {
                        anchors.fill: parent
                        antialiasing: true

                        BarSeries {
                            id: mySeries
                            labelsVisible: true
                            labelsPosition: AbstractBarSeries.LabelsInsideEnd
                            axisX: BarCategoryAxis { categories: [qmltextSunday,qmltextMonday, qmltextTuseday, qmltextWednesday, qmltextThursday, qmltextFriday, qmltextSaturday] }
                            BarSet {color: model.modelData.productionBarColor; values: [model.modelData.sunVal,model.modelData.monVal,model.modelData.tueVal,model.modelData.wedVal,model.modelData.thrVal,model.modelData.friVal,model.modelData.satVal] }

                        }
                        legend.visible: false

                    }
                }
                Rectangle
                {
                    id: id_weldAvgDisplayer
                    width: (195/1280)*parent.width
                    height:parent.height -10
                    color: "#ECECEC"
                    anchors.left: id_ProdOverChartView.right
                    border.color: "#BEC1C3"
                    radius: 2
                    y:5
                    Rectangle
                    {
                        id: id_weldAvgTitleRect
                        width: id_weldAvgDisplayer.width
                        height: 45
                        color: qmlSecondaryColor
                        radius: 2
                        Text
                        {
                            id:id_weldAvgHeading
                            width: parent.width
                            height:parent.height
                            text: averageRectTitle
                            color: "#FFFFFF"
                            font.pixelSize: Style.style2
                            font.family:Style.regular.name
                            verticalAlignment: Text.AlignVCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                        }

                    }
                    Rectangle
                    {
                        id: id_AvgValRect
                        width: id_weldAvgDisplayer.width
                        height:parent.height - id_weldAvgTitleRect.height
                        color: "#F8F9FA"
                        radius: 2
                        anchors.top: id_weldAvgTitleRect.bottom
                        Text
                        {
                            id: id_AvgValue
                            width:parent.width
                            anchors.top: id_weldAvgTitleRect.Bottom
                            anchors.centerIn: parent
                            font.pixelSize: Style.style5
                            font.family: Style.semibold.name
                            color: "#757575"
                            text: model.modelData.avgVal
                            horizontalAlignment: Text.Center
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text
                        {
                            id: id_AvgVALUnit
                            width:parent.width
                            font.pixelSize: Style.style1
                            font.family: Style.regular.name
                            color: "#757575"
                            text: model.modelData.productionUnit
                            anchors.top: id_AvgValue.bottom
                            horizontalAlignment: Text.Center
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }

    }
    Rectangle
    {
        id: id_ProdOverDateRect
        width: item_ProdOverview.width
        height: 75
        color:"#E9ECEF"
        anchors.top: id_ProductionOverviewRect.bottom
        anchors.topMargin: 10
        Rectangle{
            id:id_spacer1
            width:(id_ProdOverDateRect.width-434)/2
            height: id_ProdOverDateRect.height -50
            y:20
            color: "#E9ECEF"
        }
        Button{
            id: id_DecrementDateBtn
            width: 32
            height: 50
            y:20
            text: "<"
            anchors.left: id_spacer1.right
            onClicked: {
                busyindicatiorId.visible = true
                lastDayOfWeek = analyticsproduction.clickPreviousWeek(lastDayOfWeek)
                firstDayOfWeek = analyticsproduction.startDateofWeek(lastDayOfWeek)
                getProductionOverView(firstDayOfWeek,lastDayOfWeek)
            }

        }
        Rectangle{
            id:id_DateDisplayerTextRect
            width: 370
            height:id_ProdOverDateRect.height -50
            y:20
            color: "#E9ECEF"
            anchors.left: id_DecrementDateBtn.right
            Text{
                id:id_DateDisplayerText
                width: parent.width
                height:parent.height
                text: firstDayOfWeek+" - "+lastDayOfWeek
                font.pixelSize: 32// Style.style5
                font.family: Style.regular.name//Style.regular.name
                color: "#757575"
            }
        }
        Button{
            id: id_IncrementDateBtn
            width: 32
            height: 50
            y:20
            anchors.left: id_DateDisplayerTextRect.right
            text: ">"
            onClicked: {
                busyindicatiorId.visible = true
                firstDayOfWeek = analyticsproduction.clickNextWeek(firstDayOfWeek)
                lastDayOfWeek = analyticsproduction.endDateofWeek(firstDayOfWeek)
                getProductionOverView(firstDayOfWeek,lastDayOfWeek)
            }

        }
        Rectangle{
            id:id_spacer2
            width:(id_ProdOverDateRect.width-434)/2
            height: id_ProdOverDateRect.height -50
            y:20
            anchors.left: id_IncrementDateBtn.right
            color: "#E9ECEF"
        }
    }
}
