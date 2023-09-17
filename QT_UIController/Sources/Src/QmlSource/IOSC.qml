import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2 as New
import QtQuick.Controls.Styles 1.4
import Style 1.0

Item {
    //    width:1920
    //    height:1080
    id:rootIOSC
    property var rootWidth : 1920
    property var rootHeight : 1080
    property var columnHeight: 1000
    property var columnWidth: 1840
    property var topMargin_: 20
    property var bottomMargin_: 20
    property var leftMargin_: 20
    property var rightMargin_: 20
    property var backgroundColor: "#80ececec"
    property var backgroundOpacity: 1
    property var digitalInputsText : "DIGITAL INPUTS"
    property var digitalOutputsText :"DIGITAL OUTPUTS"
    property var analogInputsText : "ANALOG INPUTS"
    property var analogOutputsText :"ANALOG OUTPUTS"
    property var restoreDefaultText: "RESTORE DEFAULTS"
    property var saveText: "SAVE"
    property var titleTextInputPinSeriesTopLeft: "J116"
    property var digitalRectPanelHeightRatio: .6
    width:parent.width
    height:parent.height
    ListModel{
        id:ioDiagnosticsDigitalInputsModelSC
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}
        ListElement{}

    }
    Rectangle
    {
        anchors
        {
            left:parent.left
            top:parent.top
            right:parent.right
            bottom: parent.bottom
           // topMargin: topMargin_
            bottomMargin: bottomMargin_
           // leftMargin: leftMargin_
            rightMargin: rightMargin_
        }

        Rectangle
        {

            anchors
            {
                left:parent.left
                top:parent.top
                right:parent.horizontalCenter
                bottom: parent.bottom
               // topMargin: topMargin_
                bottomMargin: bottomMargin_
               // leftMargin: leftMargin_
                rightMargin: rightMargin_
            }

            ColumnLayout
            {   anchors
                {
                    left:parent.left
                    top:parent.top
                    right:parent.right
                    bottom: parent.bottom
                    topMargin: topMargin_
                    bottomMargin: bottomMargin_
                    leftMargin: leftMargin_
                    rightMargin: rightMargin_
                }
                spacing: 40
                Rectangle
                {

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height*digitalRectPanelHeightRatio
                    ColumnLayout
                    {anchors.fill: parent
                        spacing: 10
                        Rectangle
                        {
                            id:leftDigitalTitleTextPanel
                            Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            // color:backgroundColor
                            Text
                            {
                                id:text_Digital_input
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:digitalInputsText
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }

                        }//titleTextTopLeft
                        Rectangle
                        {
                            Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            // color:backgroundColor
                            Text
                            {
                                id:titleTextPinSeriesTopLeft
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:titleTextInputPinSeriesTopLeft
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }
                        }//titleTextPinSeriesTopLeft
                        Rectangle
                        {id:leftTopViewRect
                            Layout.preferredWidth:  parent.width
                            Layout.fillHeight:  true
                            color:backgroundColor
                            GridView{
                                id:topLeftView
                                anchors
                                {
                                    left:leftTopViewRect.left
                                    top:leftTopViewRect.top
                                    right:parent.right
                                    bottom: parent.bottom
                                    topMargin: topMargin_
                                    bottomMargin: bottomMargin_
                                    leftMargin: leftMargin_
                                    rightMargin: rightMargin_
                                }

                                model:ioDiagnosticsDigitalInputsModelSC
                                clip:true
                                cellWidth:(width)/2
                                cellHeight: 60
                                delegate: Item{
                                   width:(topLeftView.width-(20*1))/2

                                    IODiagnosticsDigitalInputs{
                                        width:parent.width
                                    }

                                }

                            }//topLeftView
                        }//leftTopViewRect
                    }//columnLayoutTopLeft
                }//rectTopLeft
                Rectangle
                {
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height*(1-digitalRectPanelHeightRatio)
                    // color:backgroundColor
                    ColumnLayout
                    {anchors.fill: parent
                        spacing: 10
                        Rectangle
                        {
                            id:leftAnalogTitleTextPanel
                            Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            //color:backgroundColor

                            Text
                            {
                                id:text_Analog_input
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:analogInputsText
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }

                        }//titleTextBottomLeft
                        Rectangle{     Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            //color:backgroundColor
                            Text
                            {
                                id:titleTextPinSeriesBottomLeft
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:titleTextInputPinSeriesTopLeft
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }
                        }//titleTextPinSeriesBottomLeft
                        Rectangle
                        {
                            Layout.preferredWidth:  parent.width
                            Layout.fillHeight:  true
                            color:backgroundColor
                        }//bottomLeftView
                    }//columnLayoutBottomLeft
                }//rectBottomLeft

            }//columnLayoutLeft
        }//rectLeft
        Rectangle
        {

            anchors
            {
                left:parent.horizontalCenter
                top:parent.top
                right:parent.right
                bottom: parent.bottom
                topMargin: topMargin_
                bottomMargin: bottomMargin_
                leftMargin: leftMargin_
                //rightMargin: rightMargin_
            }

            ColumnLayout
            {   anchors
                {
                    left:parent.left
                    top:parent.top
                    right:parent.right
                    bottom: parent.bottom
                    //topMargin: topMargin_
                    bottomMargin: bottomMargin_
                    leftMargin: leftMargin_
                   // rightMargin: rightMargin_
                }
                spacing: 40
                Rectangle
                {

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height*digitalRectPanelHeightRatio
                    ColumnLayout
                    {anchors.fill: parent
                        spacing: 10
                        Rectangle
                        {
                            id:rightDigitalTitleTextPanel
                            Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            // color:backgroundColor
                            Text
                            {
                                id:text_Digital_output
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:digitalOutputsText
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }

                        }//titleTextTopRight
                        Rectangle{     Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            // color:backgroundColor
                            Text
                            {
                                id:titleTextPinSeriesTopRight
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:titleTextInputPinSeriesTopLeft
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }
                        }//titleTextPinSeriesTopRight
                        Rectangle
                        {id:rightTopViewRect
                            Layout.preferredWidth:  parent.width
                            Layout.fillHeight:  true
                            color:backgroundColor
                            GridView{
                                id:topRightView
                                anchors
                                {
                                    left:rightTopViewRect.left
                                    top:rightTopViewRect.top
                                    right:parent.right
                                    bottom: parent.bottom
                                    topMargin: topMargin_
                                    bottomMargin: bottomMargin_
                                    leftMargin: leftMargin_
                                    rightMargin: rightMargin_
                                }

                                model:ioDiagnosticsDigitalInputsModelSC
                                clip:true
                                cellWidth:(width)/2
                                cellHeight: 60
                                delegate: Item{
                                   width:(topRightView.width-(20*1))/2

                                    IODiagnosticsDigitalOutputs{
                                        width:parent.width
                                    }

                                }

                            }//topLeftView
                        }//topRightView
                    }//columnLayoutTopRight
                }//rectTopLeft
                Rectangle
                {Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height*(1-digitalRectPanelHeightRatio)
                    // color:backgroundColor
                    ColumnLayout
                    {anchors.fill: parent
                        spacing: 10
                        Rectangle
                        {
                            id:rightAnalogTitleTextPanel
                            Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            //color:backgroundColor
                            anchors.left:parent.left
                            anchors.leftMargin: 20
                            Text
                            {
                                id:text_Analog_output
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:analogOutputsText
                                anchors.left:parent.left
                                anchors.leftMargin: 20

                            }

                        }//titleTextBottomRight
                        Rectangle{     Layout.preferredWidth:  parent.width
                            Layout.preferredHeight: 20
                            anchors.left:parent.left
                            anchors.leftMargin: 20
                            //color:backgroundColor
                            Text
                            {
                                id:titleTextPinSeriesBottomRight
                                font.pixelSize: Style.style6
                                font.family: Style.semibold.name
                                Layout.preferredHeight: parent.height
                                Layout.fillWidth: true
                                color:"#212529"
                                text:titleTextInputPinSeriesTopLeft
                                anchors.left:parent.left
                                anchors.leftMargin: 20


                            }
                        }//titleTextPinSeriesBottomRight
                        Rectangle
                        {
                            Layout.preferredWidth:  parent.width
                            Layout.fillHeight:  true
                            color:backgroundColor
                        }//bottomRightView
                    }//columnLayoutBottomRight
                }//rectBottomRight

            }//columnLayoutRight
        }//rectRight

    }//rootRectSC

}//rootIOSC
