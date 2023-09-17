import QtQuick 2.0
import Style 1.0
Item{

    /*Qml macro definition*/
    property string qmlOperatingFrequency     : qsTr("PS Frequency (Hz)")
    property string qmlOperatingPower         : qsTr("PS Power Level (W)")
    property string qmlParameterFrequency     : qsTr("Frequency")
    property string qmlParameterMemory        : qsTr("Memory")
    property string qmlParameterAmplitude     : qsTr("Amplitude")
    property string qmlParameterPower         : qsTr("Power")
    property real   qmlEnumFreuencyType_20KHz : 0
    property real   qmlEnumFreuencyType_30KHz : 1
    property real   qmlEnumFreuencyType_40KHz : 2

    width:parent.width
    height:(22/184)*parent.height

Rectangle{
    color:"#00ffffff"
    width:parent.width
    height:parent.height

    Text{
        id:text_progressbar
        y:progressbar.height/2-height/2
        anchors.left: parent.left
        anchors.leftMargin: 0
        width:(140/950)*parent.width
        height:(20/30)*parent.height
        text:model.modelData.ParamName
        font.pixelSize: Style.style1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color:"#868e96"
        font.family:Style.semibold.name
    }

    UltrasonicTestProgressbar{
        id:progressbar
        height:parent.height
        anchors.left: text_progressbar.right
        anchors.leftMargin: (10/930)*parent.width
        anchors.right: text_progress.left
        anchors.rightMargin: (20/930)*parent.width
        progressBarControl.minimumValue: 0
        progressBarControl.maximumValue:
            /*To select progress's maximum value based on parameters and operating frequencies*/
        {
            if((model.modelData.ParamName === qmlParameterFrequency) ||
                    (model.modelData.ParamName === qmlParameterMemory))
            {
                switch (sysconfig.getCurrentNameOfComboBox(qmlOperatingFrequency))
                {

                case qmlEnumFreuencyType_20KHz:
                    20000
                    break

                case qmlEnumFreuencyType_30KHz:
                    30000
                    break

                case qmlEnumFreuencyType_40KHz:
                    40000
                    break

                default:
                    break
                }
            }

            else if(model.modelData.ParamName === qmlParameterAmplitude)
            {
                100
            }

            else if(model.modelData.ParamName === qmlParameterPower)
            {
                /*To set progress bar's max value based on power level*/
                sysconfig.getNameOfTextEdit(qmlOperatingPower)
            }
        }

        progressBarControl.value:  model.modelData.ParamValue
    }

    Text{
        id:text_progress
        y:progressbar.height/2-height/2
        anchors.right: text_progress_unit.left
        anchors.rightMargin: (10/930)*parent.width
        width:(60/950)*parent.width
        height:(20/30)*parent.height
        text:model.modelData.ParamValue
        font.pixelSize: Style.style1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color:"#868e96"
        font.family: Style.regular.name
    }
    Text{
        id:text_progress_unit
        y:progressbar.height/2-height/2
        anchors.right: parent.right
        anchors.rightMargin: (0/930)*parent.width
        width:(40/950)*parent.width
        height:(20/30)*parent.height
        text:model.modelData.ParamUnit
        font.pixelSize: Style.style1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color:"#868e96"
        font.family: Style.regular.name
    }
}
}
