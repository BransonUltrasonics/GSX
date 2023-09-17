import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import com.branson.menu 1.0
import com.branson.WSrestrictionscreen 1.0

Item {
    implicitWidth: 400
    implicitHeight: 995
    property real gridHeight:1080
    property real parentHeight:1080
    property real rectHeight: 80
    property real rectWidth:400
    property real parentwidth:1920
    property real limitsmenupressureX: 0
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"

    Component.onCompleted: {
        login.setWSRestrictedScreens(WSRestrictionScreen.GENERAL_RESTRICTED)
    }

    Component.onDestruction: {
        if(false == isActSetupActive)
            login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
    }

    id:root

    function switchTitleBarText()
    {

    }

    ListView
    {
        id: id_calibgrid
        width: parent.width
        height: parent.height
        currentIndex:0
        clip:true
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        onCurrentItemChanged:
        {
            id_calibgrid.itemAt(currentItem.x,currentItem.y).paramColor="#000000"
        }
        onCurrentIndexChanged:
        {
            switchTitleBarText()
        }
        spacing: 1
        model:ModelCalibration{id:ada}
        delegate:ConfigurationItem
        {
            id:id_calibgrid_tab0
            paramText:name
            height:  id_calibgrid.height/10
            width:  id_calibgrid.width/5

            tab_griditem_mousearea.onClicked:
            {
                id_calibgrid.currentIndex=index
            }

            Connections
            {
                target:id_calibgrid
                onCurrentItemChanged:
                {
                    switchColor()
                }
            }

            /**
               *@breif: To switch color when index changed
               **/
            function switchColor()
            {
                if(index==id_calibgrid.currentIndex)
                {
                    paramTextColor="#ffffff"
                    paramColor=pressedColor
                }
                if(index!=id_calibgrid.currentIndex)
                {
                    paramTextColor="#000000"
                    paramColor=unpressedColor
                }
            }
        }
    }

    StackLayout
    {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: id_calibgrid.currentIndex

        Item
        {
            id: tab0
            property real limitpressureX
            limitpressureX:(id_calibgrid.width-(20/parentwidth)*id_calibgrid.width)/4 +(20/1920)*window.width
            Actuatorcalibration
            {
                x:(420/1920)*window.width
                y: id_calibgrid.y
                width:window.width*(1500/1920)
                height:(995/1080)*window.height
            }
        }

    }

    /**
       *@breif: Update change password text field
       **/
    function setTextFieldValue( inputText )
    {
        changePasswordId.setTextFieldValue(inputText)
    }

    /**
       *@breif: Update User Management key text field
       **/
    function setUserMngKeyValue( inputText )
    {
        usrMgrMain.setUserMngKeyValue(inputText)
    }
}
