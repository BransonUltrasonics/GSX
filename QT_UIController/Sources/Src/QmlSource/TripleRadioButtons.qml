import QtQuick 2.0

Item {
    readonly property string qmlTextLow: qsTr("Low")
    readonly property string qmlTextMed: qsTr("Med")
    readonly property string qmlTextHigh: qsTr("High")
    property alias customRadioBtnLow: customRadioBtnLow
    property alias customRadioBtnMed: customRadioBtnMed
    property alias customRadioBtnHigh: customRadioBtnHigh
    property bool customRadioBtnLowStatus: false
    property bool customRadioBtnMedStatus: false
    property bool customRadioBtnHighStatus: false
    CustomRadioButton
    {
        id:customRadioBtnLow
        radiobuttontext:qmlTextLow
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        img: "qrc:/images/image/darkgrey.png"
        checked: customRadioBtnLowStatus
    }
    CustomRadioButton
    {
        id:customRadioBtnMed
        radiobuttontext:qmlTextMed
        anchors.left: customRadioBtnLow.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        img: "qrc:/images/image/darkgrey.png"
        checked: customRadioBtnMedStatus
    }
    CustomRadioButton
    {
        id:customRadioBtnHigh
        radiobuttontext:qmlTextHigh
        anchors.left:customRadioBtnMed.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        img: "qrc:/images/image/darkgrey.png"
        checked: customRadioBtnHighStatus
    }
}
