import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Styles 1.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import com.branson.WSrestrictionscreen 1.0

Item {
    readonly property string qmltextCreateReport: qsTr("Create a Report")
    readonly property string qmltextSingleReportButton: qsTr("SINGLE REPORT")
    readonly property string qmltextSingleReport: qsTr("Single Report")
    property string unpressedColor: "#ECECEC"
    property string pressedColor: qmlPrimaryColor
    readonly property string gqmltextWeldDataMsg :qsTr("Weld data extraction is in progress, Please wait until completion")
    readonly property string qmltextWebServicesLoggedIn :qsTr("Please logout from the Web Services before accessing this screen")


    Component.onCompleted:
    {
       if(config.getFeatureFlagStatus(RuntimeFeatures.UI_WEB_SERVICES) != 1)
        {
            id_reportgrid.model=modelTwo
        }
    }

    Component.onDestruction:
    {
        if(false == isActSetupActive)
            login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
    }

    id:root
    ListModel
    {
        id:modelOne
        ListElement{name:qsTr("CREATE A REPORT")}
        ListElement{name:qsTr("SECURITY")}
    }
    ListModel
    {
        id:modelTwo
        ListElement{name:qsTr("CREATE A REPORT")}
    }

    ListView
    {
        id: id_reportgrid
        width: parent.width
        height: parent.height
        currentIndex:0
        clip:true
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        onCurrentItemChanged:
        {
            id_reportgrid.itemAt(currentItem.x,currentItem.y).paramColor="#000000"
        }
        onCurrentIndexChanged: {
            //switchTitleBarText()
        }
        spacing: 1
        model:modelOne//ListModel{id:ada}
        delegate:ConfigurationItem
        {

            Component.onCompleted:
            {
               if ((index == 1) && (!isPermissionAllowed(Features.SECURITY)))
                {
                    tab_griditem.color = "#757575"
                    name.color = "#BEC1C3"
                    tab_griditem_mousearea.enabled = false
                }
            }

            id:id_reportgrid_list
            paramText:name
            height:  id_reportgrid.height/10
            width:  id_reportgrid.width/5

            tab_griditem_mousearea.onClicked:
            {
                if(g_reportgenerationinprogress)
                {
                    messagePopup.open(messagePopup.warning_title,gqmltextWeldDataMsg+",",messagePopup.without_button)
                    return
                }
                else if ((1 == index) && (true == login.IsWebServicesLogged))
                {
                    messagePopup.open(messagePopup.warning_title,qmltextWebServicesLoggedIn,messagePopup.without_button)
                    return
                }
                else
                    id_reportgrid.currentIndex=index

                //If Security screen is active, webservices login is denied.
                if (1 == index)
                    login.setWSRestrictedScreens(WSRestrictionScreen.GENERAL_RESTRICTED)
                else
                    login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
            }

            Connections
            {
                target:id_reportgrid
                onCurrentItemChanged:
                {
                    switchColor()
                }
            }

            /**
            *@breif: To switch the color
            **/
            function switchColor()
            {
                if(index==id_reportgrid.currentIndex)
                {
                    paramTextColor="#ffffff"
                    paramColor=pressedColor
                }
                if(index!=id_reportgrid.currentIndex)
                {
                    paramTextColor="#000000"
                    paramColor=unpressedColor
                }
            }
        }
    }

    StackLayout
    {
        id: repview
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: id_reportgrid.currentIndex
        Item {
            id: id_reportgrid_tab1
            ReportGen
            {
                width:(window.width /5) * 4
                anchors.topMargin: 50
                height:((window.height /10 ) *9)
            }
        }
        Item {
            id: tab1
            Security
            {
                width:(window.width /5) * 4
                x:(window.width/5) + 10
                anchors.topMargin: 50
                height:((window.height /10 ) *9)
            }
        }

    }
}
