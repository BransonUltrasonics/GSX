import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0

ComboBox {

    id: comboboxId
    property alias comboboxpopupheight:comboboxpopup.implicitHeight
    property alias userLevel: comboboxId.currentIndex
    property alias userLevelText: comboboxId.currentText
    property alias comboBoxmodel: comboboxId.model
    property alias comboBoxenabled: comboboxId.enabled
    property int imageWidth:30
    property int imageHeight:30
    property int minWidth: 240
    property int minHeight: 40
    property bool menuStatus: false
    property int radiusWidth: 3
    property color comboboxColor: "#E9ECEF"
    property color bgColor: "#ECECEC"
    property color carrotColor: qmlPrimaryColor
    property color textColor : "#757575"
    property color shadowColor: "#80000000"

    property alias dropDownImg:img
    property alias dropDownText:combo_text

    enabled:true
    model: comboBoxmodel
    implicitWidth: minWidth
    implicitHeight: minHeight
    popup: Popup {
        id:comboboxpopup
        y: comboboxId.height
        width: comboboxId.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: comboboxId.popup.visible ? comboboxId.delegateModel : null
            // currentIndex: comboboxId.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            //border.color: shadowColor
            id:innerBG
            color: bgColor
            radius: 2
            layer.enabled: innerBG
            layer.effect: DropShadow {
                horizontalOffset: 1
                verticalOffset: 1
                color:  shadowColor
                opacity: 0.2
                samples: 10
            }
        }
        onAboutToHide:
        {
            changeImageDown()
        }
    }
    onCurrentIndexChanged:changeIndex(currentIndex)

    contentItem: Text {
        id:combo_text
        color: textColor
        text: currentText
        anchors.left:  comboboxId.left
        anchors.leftMargin: 10
        verticalAlignment: Text.AlignVCenter
        font{
            family: Style.semibold.name
            pixelSize: Style.style2
        }
    }

    Connections
    {
        onPressedChanged: comboboxId.changeImageUp()
    }

    /**
       *@breif: On click change image
       **/
    function changeImageUp() {
        /*Image icon update based on System Type*/
        img.source = (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-up-arrow_ASX.svg" :
                                                            "qrc:/images/image/drop-up-arrow.svg"

    }

    /**
       *@breif: On click change image
       **/
    function changeImageDown() {
        /*Image icon update based on System Type*/
        img.source = (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                            "qrc:/images/image/drop-down-arrow_blue.svg"
    }

    /**
       *@breif: Change image
       **/
    function changeIndex(currentIndex)
    {
        comboboxId.changeImageDown()
    }
    indicator:  Image {
        id: img
        anchors.right: comboboxId.right
        anchors.rightMargin: 5
        anchors.top:comboboxId.top
        anchors.topMargin: 10
        anchors.verticalCenter: comboboxId.verticalCenter
        width: imageWidth
        height: imageHeight
        fillMode: Image.PreserveAspectFit
        source: comboboxId.changeImageDown()

    }
    background: Rectangle {
        id: comboBoxRect
        implicitWidth: minWidth
        implicitHeight: minHeight
        color: comboboxColor
        radius: radiusWidth
        layer.enabled: comboboxId.pressed ? false : true
        layer.effect: DropShadow {
            horizontalOffset: comboboxId.pressed ? 0 : 1
            verticalOffset: comboboxId.pressed ? 0 : 1
            color:  shadowColor
            opacity: 0.2
            samples: 10
        }
    }
}
