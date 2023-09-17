import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.eventlogging 1.0


Item {

    readonly property string qmltextACTCALIB_MAIN_TEXT : qsTr("CALIBRATION")
    readonly property string qmltextACTCALIB_INITIAL_TEXT : qsTr("Place external force gauge under the horn then press the Start button to initiate the Calibration routine.")
    readonly property string qmltextACTCALIB_DELAY_TEXT : qsTr("Please wait..")
    readonly property string qmltextACTCALIB_FIRST_READING : qsTr("Enter first force reading")
    readonly property string qmltextACTCALIB_SECOND_READING : qsTr("Enter second force reading")
    readonly property string qmltextACTCALIBLSTARTTEXT : qsTr("Start")
    readonly property string qmltextACTCALIBLBACKTEXT : qsTr("Back")
    readonly property string qmltextACTCALIBLNEXTTEXT : qsTr("Next")
    readonly property string qmltextACTCALIBLFINISHTEXT : qsTr("Finish")
    readonly property string qmltextACTCALIBLABORTTEXT : qsTr("Abort")
    readonly property string qmltextACTCALIB_STATUS_ABORTED:qsTr("Calibration aborted")
    readonly property string qmltextACTCALIB_STATUS_ABORT_FAILED:qsTr("Calibration abort failed")
    readonly property string qmltextACTCALIB_STATUS_START_FAILED:qsTr("Calibration start failed")
    readonly property string qmltextACTCALIB_STATUS_FIRST_READING_SAVEFAILED:qsTr("first force reading save failed")
    readonly property string qmltextACTCALIB_STATUS_FIRST_READING_INVALIDINPUT: qsTr("Invalid first force reading")
    readonly property string qmltextACTCALIB_STATUS_SECOND_READING_SAVEFAILED:qsTr("second force reading save failed")
    readonly property string qmltextACTCALIB_STATUS_SECOND_READING_INVALIDINPUT: qsTr("Invalid second force reading")
    readonly property string qmltextACTCALIB_STATUS_READINGS_SAVED:qsTr("Calibration saved successfully")
    readonly property string qmltextACTCALIB_STATUS_READY_TEXT:qsTr("Ready")
    readonly property string qmltextgrayColor:qsTr("gray")
    readonly property string qmltextLbsUnit:qsTr("lbs")
    readonly property string qmltextNewtonsUnit:qsTr("N")


    property string qmltextACTCALIB_UNIT

    readonly property int  calibstate_Start : 0
    readonly property int  calibstate_Next : 1
    readonly property int  calibstate_Finish : 2
    readonly property int  calibstate_Splash : 3
    readonly property int  calibstate_SplashDelay : 3000
    property int g_currentcalibstate: calibstate_Start
    property color calib_start_btn_textcolor : "#FFFFFF"
    property int g_tempstate : 0
    property CustomNumPad cNumpad
    property string  caliblabeltext: qmltextACTCALIBLSTARTTEXT
    property int unitType

    readonly property string qmltextCommtReqMsg: qsTr("Comment is required")
    property DialogPopup dialogPopup

    Component.onCompleted: {
    	isCalibration = true
        horndownObj.startOrStopWeld(0)
        //actuatorCalibrationObject.sendStart()
        unitType = actuatorCalibrationObject.getUnitsType()
        if(unitType)
            qmltextACTCALIB_UNIT = qmltextLbsUnit
        else
            qmltextACTCALIB_UNIT = qmltextNewtonsUnit
    }
    
    Component.onDestruction:
    {
    	isCalibration = false
        if(!isLogout && !isHornDown)
        {
       		actuatorCalibrationObject.onExit()
            horndownObj.startOrStopWeld(1)
        }
    }

    /**
      *@breif: Hide all controls
      **/
    function hideallcontrols()
    {
        id_ac_calib_intiallabel.visible = false
        id_ac_calib_waitlabel.visible = false
        id_ac_calib_busy.visible = false
        id_ac_calib_firstforcereadinglabel.visible = false
        id_ac_calib_firstforcereadingvalue.visible = false
        id_ac_calib_secondforcereadinglabel.visible = false
        id_ac_calib_secondforcereadingvalue.visible = false
        id_ac_calib_abortbtn.visible = false
        id_ac_calib_start_next_finish_btn.visible = false
        id_ac_calib_firstforcereadinglabel_unit.visible = false
        id_ac_calib_secondforcereadingvalue_unit.visible = false
    }

    /**
      *@breif: Set statusbar text
      **/
    function setstatusbartext(currentstatustext)
    {
        recipe.updateStatusText(currentstatustext)
    }

    /**
      *@breif: To manupulate control states
      **/
    function manupulatecontrolstate(g_currentcalibstate)
    {
        switch(g_currentcalibstate)
        {
        case calibstate_Start:
            id_ac_calib_intiallabel.visible = true
            id_ac_calib_start_next_finish_btn.text = qmltextACTCALIBLSTARTTEXT
            id_ac_calib_start_next_finish_btn.visible = true
            id_ac_calib_abortbtn.visible = false
            break;
        case calibstate_Splash:
            actuatorCalibrationObject.sendStart()
            id_ac_calib_intiallabel.visible = true
            id_ac_calib_waitlabel.visible = true
            id_ac_calib_busy.visible = true
            id_ac_calib_start_next_finish_btn.textColor =qmltextgrayColor
            id_ac_calib_start_next_finish_btn.enabled = false
            id_ac_calib_start_next_finish_btn.visible = true
            id_ac_calib_splashtimer.start()
            break;
        case calibstate_Next:
            id_ac_calib_splashtimer.stop()
            id_ac_calib_start_next_finish_btn.text = qmltextACTCALIBLNEXTTEXT
            id_ac_calib_abortbtn.visible = true
            id_ac_calib_start_next_finish_btn.visible = true
            id_ac_calib_firstforcereadinglabel.visible = true
            id_ac_calib_firstforcereadingvalue.visible = true
            id_ac_calib_start_next_finish_btn.textColor= calib_start_btn_textcolor
            id_ac_calib_start_next_finish_btn.enabled = true
            id_ac_calib_firstforcereadinglabel_unit.visible = true
            break;
        case calibstate_Finish:
            id_ac_calib_secondforcereadinglabel.visible =true
            id_ac_calib_secondforcereadingvalue.visible = true
            id_ac_calib_start_next_finish_btn.text = qmltextACTCALIBLFINISHTEXT
            id_ac_calib_start_next_finish_btn.visible = true
            id_ac_calib_abortbtn.visible = true
            id_ac_calib_secondforcereadingvalue_unit.visible = true
            break;
        }
    }

    /**
      *@breif: To open the numpad
      **/
    function openNumpad(name,value)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        cNumpad.open(name,name, value.text, qmlTextEmpty)
        cNumpad.onSave.connect(function okay()
        {
            var textInput = cNumpad.keyInputText
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


    function createDialogpopup(message)
    {
        var comment = ""
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(window)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === "")
            {
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_CALIBRATION, comment);
            }

            dialogPopup.close()
            dialogPopup.onSave.disconnect(onOkay)
            dialogPopup.destroy()
        })
        dialogPopup.onCancel.connect(function onCancel()
        {
            dialogPopup.close()
            dialogPopup.onCancel.disconnect(onCancel)
            dialogPopup.destroy()
        })
    }

    Timer
    {
        id:id_ac_calib_splashtimer
        interval: calibstate_SplashDelay
        triggeredOnStart: false
        onTriggered:
        {
            hideallcontrols()
            g_currentcalibstate = calibstate_Next
            manupulatecontrolstate(g_currentcalibstate)

        }
    }

    Rectangle
    {

        id: id_ac_calib_titlerect
        width: parent.width
        height: 60
        color: "#E9ECEF"
        anchors.topMargin: 0
        Text
        {
            id: id_ac_calib_title
            text: qmltextACTCALIB_MAIN_TEXT
            color: qmlPrimaryColor
            font.bold: true
            font.pixelSize: Style.style7
            font.family:  Style.semibold.name
            anchors.centerIn: id_ac_calib_titlerect
        }
    }
    Rectangle
    {
        id: id_ac_calib_contentrect
        color:"#E9ECEF"
        width: parent.width-10
        height: parent.height * (40/100)
        anchors.top: id_ac_calib_titlerect.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        border.width: 1

        Column
        {
            id: id_ac_calib_contentcolumn
            spacing: 2

            Rectangle {
                id:id_ac_calib_contentcolumnrect
                width: id_ac_calib_contentrect.width;
                height: id_ac_calib_contentrect.height
                Text
                {
                    id: id_ac_calib_intiallabel
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    anchors.top: parent.top
                    anchors.topMargin: 40
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style5
                    wrapMode: Text.WordWrap
                    text: qmltextACTCALIB_INITIAL_TEXT

                }
                Text
                {
                    id: id_ac_calib_waitlabel
                    anchors.top: id_ac_calib_intiallabel.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style5
                    text: qmltextACTCALIB_DELAY_TEXT

                }
                BusyIndicator {
                    id:id_ac_calib_busy

                    anchors.top: id_ac_calib_intiallabel.bottom
                    anchors.topMargin: 50
                    anchors.horizontalCenter: id_ac_calib_waitlabel.horizontalCenter
                    running: true

                }
                Text {
                    id:id_ac_calib_firstforcereadinglabel
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width/2 - 150
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 30
                    text: qmltextACTCALIB_FIRST_READING
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style5
                }
                CustomTextField {
                    id:id_ac_calib_firstforcereadingvalue
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 20
                    anchors.left: id_ac_calib_firstforcereadinglabel.right
                    anchors.leftMargin: 40
                    validator: IntValidator { bottom:1; top: 9999}
                    height: 40
                    width : 140
                    onClick:
                    {
                        var component = Qt.createComponent("CustomNumPad.qml")
                        cNumpad = component.createObject(window)
                        cNumpad.open(qmltextACTCALIB_FIRST_READING,qmltextACTCALIB_FIRST_READING, id_ac_calib_firstforcereadingvalue.text, "")
                        cNumpad.onSave.connect(function okay()
                        {
                            id_ac_calib_firstforcereadingvalue.text = cNumpad.keyInputText
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
                }
                Text {
                    id:id_ac_calib_firstforcereadinglabel_unit
                    anchors.left: id_ac_calib_firstforcereadingvalue.right
                    anchors.leftMargin: 10
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 30
                    text: qmltextACTCALIB_UNIT
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
                Text {
                    id:id_ac_calib_secondforcereadinglabel
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width/2 - 150
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 30
                    text: qmltextACTCALIB_SECOND_READING
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style5
                }
                CustomTextField {
                    id:id_ac_calib_secondforcereadingvalue
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 20
                    anchors.left: id_ac_calib_firstforcereadinglabel.right
                    anchors.leftMargin: 40
                    height: 40
                    width : 140
                    validator: IntValidator { bottom:1; top: 9999}
                    onClick:
                    {
                        var component = Qt.createComponent("CustomNumPad.qml")
                        cNumpad = component.createObject(window)
                        cNumpad.open(qmltextACTCALIB_SECOND_READING,qmltextACTCALIB_SECOND_READING, id_ac_calib_secondforcereadingvalue.text, "")
                        cNumpad.onSave.connect(function okay()
                        {
                            id_ac_calib_secondforcereadingvalue.text = cNumpad.keyInputText
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
                }
                Text {
                    id:id_ac_calib_secondforcereadingvalue_unit
                    anchors.left: id_ac_calib_secondforcereadingvalue.right
                    anchors.leftMargin: 10
                    anchors.top: id_ac_calib_busy.bottom
                    anchors.topMargin: 30
                    text: qmltextACTCALIB_UNIT
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                }
            }
        }
        Component.onCompleted:
        {
            hideallcontrols()
            manupulatecontrolstate(g_currentcalibstate)
        }
    }

    Rectangle
    {
        id: id_ac_calib_buttonrow
        width: id_ac_calib_contentrect.width
        height: 60
        anchors.top :id_ac_calib_contentrect.bottom
        anchors.topMargin: 80
        anchors.left:  parent.left
        anchors.leftMargin: parent.width/2
        anchors.verticalCenter: parent.verticalCenter
        PrimaryButton
        {
            id: id_ac_calib_abortbtn
            text:qmltextACTCALIBLABORTTEXT
            anchors.left: parent.left
            anchors.leftMargin: 20
            onClicked:
            {
                if(actuatorCalibrationObject.sendAbort())
                {
                    hideallcontrols()
                    g_currentcalibstate = calibstate_Start
                    manupulatecontrolstate(g_currentcalibstate)
                    setstatusbartext(qmltextACTCALIB_STATUS_ABORTED)
                }
                else
                {
                    setstatusbartext(qmltextACTCALIB_STATUS_ABORT_FAILED)
                }
            }
        }

        PrimaryButton
        {
            id: id_ac_calib_start_next_finish_btn
            text: caliblabeltext
            anchors.left: id_ac_calib_abortbtn.right
            anchors.leftMargin: 20
            onClicked:
            {
                if(g_currentcalibstate ==calibstate_Start)
                {
                    //Send BL about start request and get acknowledgement
//                    if(actuatorCalibrationObject.sendStart())
//                    {
                        //Currently user clicked on start button state, so please move to splash state
                        g_tempstate = calibstate_Splash
                        setstatusbartext("")
//                    }
//                    else
//                    {
//                        g_tempstate = g_currentcalibstate
//                        setstatusbartext(qmltextACTCALIB_STATUS_START_FAILED)
//                    }

                }
                if(g_currentcalibstate == calibstate_Next)
                {
                    if( id_ac_calib_firstforcereadingvalue.text.length > 0)
                    {
                        //Current we got first reading we need to get second reading so move to finish state
                        if(actuatorCalibrationObject.sendFirstReading(id_ac_calib_firstforcereadingvalue.text))
                        {
                            g_tempstate = calibstate_Finish
                            setstatusbartext(qmlTextEmpty)
                        }
                        else
                        {
                            g_tempstate = g_currentcalibstate
                            setstatusbartext(qmltextACTCALIB_STATUS_FIRST_READING_SAVEFAILED)
                        }
                    }
                    else
                    {
                        g_tempstate = g_currentcalibstate
                        setstatusbartext(qmltextACTCALIB_STATUS_FIRST_READING_INVALIDINPUT)
                    }

                }
                if(g_currentcalibstate == calibstate_Finish)
                {
                    if(id_ac_calib_secondforcereadingvalue.text.length > 0)
                    {
                        if(actuatorCalibrationObject.sendSecondReading(id_ac_calib_firstforcereadingvalue.text,id_ac_calib_secondforcereadingvalue.text))
                        {
                            createDialogpopup(qmltextACTCALIB_STATUS_READINGS_SAVED)
                            g_tempstate = calibstate_Start
                            setstatusbartext(qmlTextEmpty)
                            setstatusbartext(qmltextACTCALIB_STATUS_READINGS_SAVED)
                        }
                        else
                        {
                            g_tempstate = g_currentcalibstate
                            setstatusbartext(qmltextACTCALIB_STATUS_SECOND_READING_SAVEFAILED)
                        }
                    }
                    else
                    {
                        g_tempstate = g_currentcalibstate
                        setstatusbartext(qmltextACTCALIB_STATUS_SECOND_READING_INVALIDINPUT)
                    }
                }

                hideallcontrols()
                g_currentcalibstate = g_tempstate
                manupulatecontrolstate(g_currentcalibstate)
            }
        }
    }
}
