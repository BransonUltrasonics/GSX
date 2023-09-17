import QtQuick 2.0
import Style 1.0
import com.branson.scstate 1.0
import com.branson.openedscreen 1.0

Rectangle {
    id: splashScreenId
    width: window.width
    height: window.height
    color: "#D0CECE"
    readonly property string qmltextNoConnection:"No Connection"
    readonly property string qmltextNoServerConn:qsTr("Internal communication failure. Attempting to recover")
    readonly property string qmltextReadSysConfig:qsTr("Reading System configuration")
    readonly property string qmltextReadFeatureConfig:qsTr("Reading feature settings")

    property int idealTimeShowSplash: 1000   //2 sec
    property int count: 0
    property int reqIdfromSc: 0
    property int scStatus: 0
    property int noOfAlarms: 0
    property bool retVal : false

    Image {
        id: splashScreenImageId
        width: parent.width
        height: parent.height*(90/100)
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.top:parent.top
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/image/GSX Wallpaper.bmp"
    }

    Timer {
        id: splashTimerId
        interval: idealTimeShowSplash
        running: true
        onTriggered:
        {
            login.setAuthorityCheckStatus(1)
            // What ever the system state User should allow login

            if(dbVersion == "")
                dbVersion = config.CheckDBVersion()
            if(dbVersion == qmltextNoConnection)// && (!sysconfig.getAuthorityCheck()))
            {
                splashWaitingInfoID.text=qmltextNoServerConn
                idealTimeShowSplash = 500
                splashTimerId.restart()
            }
            else
            {
                splashWaitingInfoID.text=qmltextReadFeatureConfig
                retVal = config.readRuntimeFlagInformation()
                if(retVal == true)
                {
                    splashWaitingInfoID.text=qmltextReadSysConfig
                    retVal = config.readSysConfigData()
                    if(retVal == true)
                    {
                        loadmain.setSource(Qt.resolvedUrl("Login.qml"))
                    }
                    else
                    {
                        idealTimeShowSplash = 500
                        splashTimerId.restart()
                    }
                }
                else
                {
                    idealTimeShowSplash = 500
                    splashTimerId.restart()
                }
            }
        }
    }

    Text {
        id: splashWaitingInfoID
        text: qsTr("Waiting for System Ready")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: parent.width*(1/100)
        font.bold: true
        font.pixelSize: Style.style6
        font.family: Style.semibold.name
        color: "#757575"
    }
    Text {
        id: splashWaitingInfoDotsID
        text: qsTr("")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25
        anchors.left:splashWaitingInfoID.right
        anchors.leftMargin: 2
        font.bold: true
        font.pixelSize: Style.style6
        font.family: Style.semibold.name
        color: "#757575"
    }
    Timer {
        id: id_dotBlinking_visible
        interval: 500
        running: true
        onTriggered: {
            switch(count)
            {
            case 1:
                splashWaitingInfoDotsID.text="."
                break;
            case 2:
                splashWaitingInfoDotsID.text=".."
                break;
            case 3:
                splashWaitingInfoDotsID.text="..."
                break;
            case 4:
                splashWaitingInfoDotsID.text=""
                count = 0;
                break;
            }
            count++;
            id_dotBlinking_visible.restart()
        }
    }

    Component.onCompleted: {
        config.setActivePage(7)
    }
}
