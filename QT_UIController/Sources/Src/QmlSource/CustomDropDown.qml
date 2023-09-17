import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0

ComboBox {
    id: dropDownId
    property int imageWidth: 30
    property int imageHeight: 30
    property int minWidth: 140
    property int minHeight: 40
    property bool menuStatus: false
    property int radiusWidth: 3
    property color lineColor: qmlPrimaryColor
    property color comboboxColor: "#ECECEC"
    property color textColor : "#212529"
    property color shadowColor: "#80000000"
    property string droptext: dropDownId.text
    property alias comboBoxmodel: dropDownId.model
    model:comboBoxmodel
    implicitWidth: minWidth
    implicitHeight: minHeight
    contentItem: Text {
        anchors.left:  dropDownId.left
        anchors.leftMargin: 20
        verticalAlignment: Text.AlignVCenter
        color: textColor
        text: droptext
        font{
            family: Style.regular.name
            pixelSize: Style.style2
        }
    }
    delegate: ItemDelegate
    {
        highlighted:false
        width: dropDownId.width
        text: modelData
    }

    Component.onCompleted:
    {
        if (!dropDownId.enabled)
        {
            dropDownId.comboboxColor = "#757575";
            dropDownId.textColor = "#FFFFFF";
            dropDownId.lineColor = "#FFFFFF";
        }
    }

    popup: Popup
    {
        y: dropDownId.height
        width: dropDownId.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: dropDownId.popup.visible ? dropDownId.delegateModel : null
        }

            background: Rectangle {
                 id:innerBG
                color: comboboxColor
               // radius: 2
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

    Rectangle {
        width: 1.5
        height: minHeight
        color: lineColor
        anchors.right: dropDownId.right
        anchors.rightMargin:imageWidth
    }

    Connections {
        // target: control
        onPressedChanged: dropDownId.changeImageUp()
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
        if (dropDownId.enabled)
        {
            /*Image icon update based on System Type*/
            img.source = (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/drop-down-arrow_ASX.svg" :
                                                                "qrc:/images/image/drop-down-arrow_blue.svg"
        }
        else
        {
            img.source = "qrc:/images/image/drop-down-arrow_white.svg"
        }
    }

    /**
       *@breif: Change image
       **/
    function changeIndex(currentIndex)
    {
        dropDownId.changeImageDown()
    }
    indicator:  Image {
        id: img
        // x: dropDownId.width - width - dropDownId.rightPadding
        anchors.left:  dropDownId.left
        anchors.leftMargin: dropDownId.width-imageWidth
        anchors.top:dropDownId.top
        anchors.topMargin: 5
        // y: dropDownId.topPadding + (dropDownId.availableHeight - height) / 2
        width: imageWidth
        height: imageHeight
        fillMode: Image.PreserveAspectFit
        source: dropDownId.changeImageDown()

    }
    background: Rectangle
    {
        id: dropDownRect
        implicitWidth: minWidth
        implicitHeight: minHeight
        color: comboboxColor
        radius: radiusWidth
        layer.enabled: dropDownId.pressed ? false : true
        layer.effect: DropShadow
        {
            horizontalOffset: dropDownId.pressed ? 0 : 1
            verticalOffset: dropDownId.pressed ? 0 : 1
            color:  shadowColor
            opacity: 0.2
            samples: 10
        }
    }
}


