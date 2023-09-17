import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Style 1.0
import com.branson.menu 1.0
import com.branson.eventlogging 1.0

Item {
    id: globalSettings

    readonly property string qmltextPasswrdExpiryShouldNotBeEmpty: qsTr("Password expiry days should not be empty.")
    readonly property string qmltextIdleLogOutTimeShouldNotBeEmpty: qsTr("Ideal logoff time should not be empty.")
    readonly property string qmltextInvalidValue: qsTr("Invalid value, reverting to old values")
    readonly property string qmltextSaveSuccess: qsTr( "Global settings are saved successfully")
    readonly property string qmltextEnterPasswordExpiry: qsTr("Enter Password Expiry in days")
    readonly property string qmltextPasswrdExpiration: qsTr("Password Expiration")
    readonly property string qmltextDays: qsTr("day(s)")
    readonly property string qmltextEnterIdealTimeOut: qsTr("Enter Ideal Logout Time in Minuts")
    readonly property string qmltextIdleTimeOut: qsTr("Idle Time Logout")
    readonly property string qmltextMinutes: qsTr("minute(s)")
    readonly property string qmltextSave:  qsTr("SAVE")

    property alias globalSettings: globalSettings
    property var windowHeight :1080
    property var windowWidth :1920
    property var columnSpace :20
    property var itemPosition: 20
    property var columnHeightLeft: 350
    property var columnHeightRight: 680
    property var gridElementHeight:60
    property var gridElementWidth:830
    property var columnWidth:870
    property var linePositionY: 0
    property real vinternalRectheight:1080-85
    property var comboBoxWidth: 286
    property var comboBoxHeight: 40
    property var comboBoxTextPixelSize: 16
    property var sysconfigMainWindowPositionX : 20
    property CustomNumPad cNumpad

    /**
    *@breif: To initialize global settings
    **/
    function globalSettingsInit()
    {
        leftPanelRect.height = systemConfigMenuItem.height + systemConfigMenuItem2.height + 2*columnSpace
        passwordExpiryLineEdit.text = qmlGlobalSettingsObject.passwordExpiryInDays
        idealLogofTimeLineEdit.text = qmlGlobalSettingsObject.idealLogoffTime
    }

    /**
    *@breif: Update the current changes
    **/
    function onSave()
    {

        var errorFlag = false
        if (passwordExpiryLineEdit.text === "")
        {
            recipe.updateStatusText(qmltextPasswrdExpiryShouldNotBeEmpty)
            errorFlag = true
        }
        else if (idealLogofTimeLineEdit.text === "")
        {
            recipe.updateStatusText(qmltextIdleLogOutTimeShouldNotBeEmpty)
            errorFlag = true
        }

        if (!errorFlag)
        {
            customFilter.setTimeOut(parseInt(idealLogofTimeLineEdit.text)*60000);
            if (parseInt(passwordExpiryLineEdit.text) <= 0 || parseInt(passwordExpiryLineEdit.text) > 365)
            {
                recipe.updateStatusText(qmltextInvalidValue)
                passwordExpiryLineEdit.text = qmlGlobalSettingsObject.passwordExpiryInDays
            }
            else if (parseInt(idealLogofTimeLineEdit.text) <= 0 || parseInt(idealLogofTimeLineEdit.text) > 60)
            {
                recipe.updateStatusText(qmltextInvalidValue)
                idealLogofTimeLineEdit.text = qmlGlobalSettingsObject.idealLogoffTime
            }
            else
            {
                qmlGlobalSettingsObject.updateGlobalSettings(passwordExpiryLineEdit.text, idealLogofTimeLineEdit.text);
                recipe.updateStatusText(qmltextSaveSuccess)
            }
        }
    }


    DropShadow{
        anchors.fill: leftPanelRect
        horizontalOffset: 3
        verticalOffset: 3
        color: "#000000"
        opacity: 0.2
        source: leftPanelRect
    }
    Rectangle{
        id:leftPanelRect
        x: (sysconfigMainWindowPositionX/windowWidth)*parent.width

        width: parent.width
        height: (columnHeightLeft/windowHeight)*parent.height
        color:"#ECECEC"
        DropShadow{
            anchors.fill: column
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: column
        }
        Column {
            id: column
            x: parent.width/2-width/2
            y: (itemPosition/columnHeightLeft)*parent.height
            width: parent.width
            height: parent.height
            spacing: (columnSpace/columnHeightLeft)*height

            SystemConfigMenuItem {
                x:parent.width/2-width/2

                id: systemConfigMenuItem
                width: (gridElementWidth/columnWidth)*parent.width
                height: gridElementHeight

                CustomTextField{
                    id: passwordExpiryLineEdit
                    x: 0.5172413793103448*parent.width
                    y: parent.height/2-height/2
                    width: (comboBoxWidth/gridElementWidth)*((gridElementWidth/columnWidth)*parent.width)
                    height: (comboBoxHeight/gridElementHeight)*gridElementHeight
                    placehodertext: qmltextEnterPasswordExpiry
                    onClick:
                    {                       
                        var component = Qt.createComponent("CustomNumPad.qml")
                        cNumpad = component.createObject(window)
                        cNumpad.open(passwordString.text, passwordString.text,passwordExpiryLineEdit.text, "")
                        /* When click on OKAY button in the Numpad */
                        cNumpad.onSave.connect(function okay()
                        {
                            passwordExpiryLineEdit.text = cNumpad.keyInputText
                            cNumpad.close()
                            cNumpad.onSave.disconnect(okay)
                            cNumpad.destroy()
                        })
                        /* When click on CANCEL button in the Numpad */
                        cNumpad.onCancel.connect(function cancel()
                        {
                            cNumpad.close()
                            cNumpad.onCancel.disconnect(cancel)
                            cNumpad.destroy()
                        })
                    }
                    Component.onCompleted:
                    {
                        setBackgroundColor("#FFFFFF")
                    }
                }
                Text{
                    id: passwordString
                    x: (7/290)*parent.width
                    y: 0.2857142857142857*parent.height
                    width: 200
                    height: .42*parent.height
                    text: qmltextPasswrdExpiration
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
                Text
                {
                    id:days
                    text:qmltextDays
                    anchors.left: passwordExpiryLineEdit.right
                    anchors.leftMargin: 10
                    y: 0.2857142857142857*parent.height
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
            }

            SystemConfigMenuItem {
                x:parent.width/2-width/2
                id: systemConfigMenuItem2
                width: (gridElementWidth/columnWidth)*parent.width
                height: gridElementHeight

                CustomTextField {
                    id: idealLogofTimeLineEdit
                    x: 0.5172413793103448*parent.width
                    y: parent.height/2-height/2
                    width: (comboBoxWidth/gridElementWidth)*((gridElementWidth/columnWidth)*parent.width)
                    height: (comboBoxHeight/gridElementHeight)*gridElementHeight
                    placehodertext: qmltextEnterIdealTimeOut
                    validator: IntValidator { bottom:1; top: 90}
                    onClick:
                    {
                        var component = Qt.createComponent("CustomNumPad.qml")
                        cNumpad = component.createObject(window)
                        cNumpad.open(idealLogoutString.text,idealLogoutString.text,idealLogofTimeLineEdit.text, "")
                        cNumpad.onSave.connect(function okay()
                        {
                            idealLogofTimeLineEdit.text = cNumpad.keyInputText
                            cNumpad.close()
                            cNumpad.onSave.disconnect(okay)
                            cNumpad.destroy()
                        })
                        cNumpad.onCancel.connect(function cancel()
                        {
                            cNumpad.close()
                            cNumpad.onCancel.disconnect(cancel)
                            cNumpad.destroy()
                        })
                    }
                    Component.onCompleted:
                    {
                        setBackgroundColor("#FFFFFF")
                    }
                }

                Text{
                    id: idealLogoutString
                    x: (7/290)*parent.width;
                    y: 0.2857142857142857*parent.height
                    width: .355*parent.width
                    height: .42*parent.height
                    text:qmltextIdleTimeOut
                    elide: Text.ElideMiddle
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
                Text
                {
                    id:minutes
                    text: qmltextMinutes
                    anchors.left: idealLogofTimeLineEdit.right
                    anchors.leftMargin: 10
                    y: 0.2857142857142857*parent.height
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
            }
          }
    }

    PrimaryButton {
        id: customButton1
        anchors.rightMargin: 230
        anchors.right: parent.right
        anchors.top: leftPanelRect.bottom
        anchors.topMargin: 40
        text: qmltextSave
        onClicked: {
            onSave()
        }
    }
}
