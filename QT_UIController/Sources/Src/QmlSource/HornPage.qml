import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import QtCharts 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import CustomValidator 1.0
import Style 1.0
//import QtLocation 5.3

Item {

    id: itemHornScan
    visible: true
    width:  loaderelement.width
    height: loaderelement.height
    property alias threewayCarouselDots: threeway.visible
    property double scaleRatio: 1
    property int tabWindowMargin: 10
    property int dotSpacing: 5
    property int assetSize: 32
    property int layoutSpacing: 10
    property int histItemCount: 1

    property int ampInd: 0
    property int curInd: 1
    property int phaseInd: 2
    property int impedanceInd: 3
    property int freqInd: 4

    property bool ampflag: true
    property bool currentflag: true
    property bool phaseflag: true
    property bool impedanceflag: true

    property double fontSizeScaleFactor : window.height / 800

    Component.onCompleted:
    {
        titlebarchild.visible = false
    }


    /**
    *@breif: Forcefully generate an abort button clicked event to stop the horn scan operation
    **/
    function abortScanFromPopup()
    {
        hornScanFeatureID.abortScanFromPopup()
    }

    HornScanFeature{
        id:hornScanFeatureID
        width:parent.width
        height:parent.height
    }

    NumberAnimation on x
    {
        id:leftW
        to:0
        duration:250
    }
    NumberAnimation on x
    {
        id:middleW
        to:-itemHornScan.width
        duration:250
    }
    NumberAnimation on x
    {
        id:rightW
        to:-2*itemHornScan.width
        duration:250
    }


    FourCarouselDots
    {
        id:threeway
        width:30
        height: 30
        x:parent.width/2
        anchors.top: parent.top
        anchors.topMargin: 10
        Component.onCompleted:
        {
            selected= 1
        }


        mar1.onClicked:
        {
            selected=1
//            historyDataObj.hornIndSelected = 0
//            hornRecipeObj.init()
//            historyDataObj.refreshHistory()
            loaderelement.setSource("HornPage.qml");
            titlebarchild.visible = false
        }
        mar2.onClicked:
        {
            if(false == mainwindow.hornOpInProgress)
            {
                selected=2
                seekRecipeObj.init()
                seekhistoryDataObj.seekIndSelected = 0
                seekhistoryDataObj.refreshSeekHistory()
                loaderelement.setSource("SeekScanFeature.qml");
                titlebarchild.visible = false
            }
            else
            {
                hornScanPopup.hornPopupOpen();
            }

        }
        mar3.onClicked:
        {
            if(false == mainwindow.hornOpInProgress)
            {
                ultrasonictest.getTestModeAdvancedDataFromDB()
                loaderelement.setSource("Test.qml");
                ultrasonictest.setCheckBoxModelData()
                selected=3;
                titlebarchild.visible = false
            }
            else
            {
                hornScanPopup.hornPopupOpen();
            }
        }
    }
}
