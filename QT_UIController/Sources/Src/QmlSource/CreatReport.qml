import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Styles 1.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import com.branson.runtimefeatures 1.0

Item {
    readonly property string qmltextSingleReportButton: qsTr("SINGLE REPORT")
    readonly property string qmltextContinousReport: qsTr("CONTINUOUS REPORT")
    id:dataid

    RowLayout {
        id:buttonsid
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 100
        SecondaryButton{
            id:generatebutton
            text: qmltextContinousReport
            enabled: false
            onClicked: {
                //                loaderelement.setSource("SingleReport.qml",{"width":(400/1920)*window.width,"height":window.height})
                //                titlebarchild.visible=true
                //                titlebarchildtext.text=qsTr("CONTINUOUS REPORT")
                //                titlbaronclicked="System.qml"
            }
        }
        PrimaryButton{
            id:continuousReport
            text: qmltextSingleReportButton
            onClicked: {
                loaderelement.setSource("SingleReport.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebarchild.visible=true
                titlebarchildtext.text=qmltextSingleReportButton
                titlbaronclicked="System.qml"
            }
        }
    }
}
