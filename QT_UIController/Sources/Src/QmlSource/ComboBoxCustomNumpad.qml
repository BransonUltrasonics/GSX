import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0

ComboBox
{
    readonly property string qmltextParameterName:"ParameterName"
    readonly property string qmlTextdenied: qsTr("Absolute Distance mode selection is denied until part contact is done.")

    id: control
    property int modeIndex: 0
    property bool isDefultValuesReq: true
    property string bgColor: "#BEC1C3"
    property double partcontactval: 0
    property int fontSize: Style.style1

    function setBackgroundColor( fontColor )
    {
        bgColor = fontColor
    }
    textRole: qmltextParameterName
    model: modelSteppingMode

    contentItem:Text
    {
        id: comboBox_Language_DisplayText
        text: control.currentText
        color: "#898D96"
        verticalAlignment: Text.AlignVCenter
        font.family: Style.regular.name
        font.pixelSize: fontSize
    }
    indicator:Image {
        /*Image icon update based on System Type*/
        source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                       "qrc:/images/image/drop-down-arrow_blue.svg"
        x:0.85*parent.width
        y:parent.height/2-height/2
        width:.70*parent.height
        height:width
    }
    background: Rectangle
    {
        anchors.fill:parent
        color: bgColor
        border.color: qmlPrimaryColor
    }
    property Component __dropDownStyle: MenuStyle {
        __maxPopupHeight: 600
        __menuItemType: "comboboxitem"

        frame: Rectangle {              // background
            color: "#E9ECEF"
            border.width: 1
            border.color: "#E9ECEF"
            radius: 1
        }

        itemDelegate.label:
            Text {
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: fontSize
            font.family: Style.regular.name
            color: styleData.selected ? "#ffffff" : "#898D96"
            text: styleData.text
        }

        itemDelegate.background: Rectangle {  // selection of an item
            radius: 2
            color: styleData.selected ? qmlPrimaryColor : "transparent"
        }

        __scrollerStyle: ScrollViewStyle { }
    }

    onCurrentIndexChanged:
    {
        if(currentIndex == modeIndex)
            return

        var dwStepAtIndex = dropdownCustom.textAt(dropdownCustom.currentIndex)
        var expectedpartcontact = recipe.getStructureValue(203)
        partcontactval = parseFloat(expectedpartcontact)
        if((dwStepAtIndex == qsTr("Step@Absolute")) &&(partcontactval <= 0) )
        {
            displayQuitPopup(messagePopup.warning_title, qmlTextdenied)
            currentIndex = modeIndex
            return
        }
        modeIndex = currentIndex
        if(isDefultValuesReq)
        {
            steppingNumpopup.setDefaultValues(-1)
            steppingNumpopup.setStructureWithDefaultValues()
        }
        if( dwStepAtIndex == qsTr("Step@Ext. Signal") )
        {
            steppingNumpopup.deleteStepsAtEXSignal()
        }
        else
        {
            steppingNumpopup.enableStepAtChanged()
        }
        steppingNumPad.changeStepAt()
    }
    Component.onCompleted:
    {
    }
}
