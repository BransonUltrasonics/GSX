import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick 2.7
import Style 1.0
import com.branson.wizardsteps 1.0

GroupBox
{
    property alias lineRecWidth: lineRec.width
    id: control
    title: items[(currentWizardStep - DataWizardStep.DATAWIZARD_STEP_TYPE)]
    background: Rectangle
    {
        id:gbackground
    }
    Rectangle
    {
        //anchors.top:gbackground.top
        id:lineRec
        width: reptype.width
        height:1
        color: "black"
    }

    label: Label
    {
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        //color: "#21be2b"
        elide: Text.ElideRight
        font.bold: true
        font.pixelSize: Style.style5
        font.family:  Style.semibold.name
    }
    // Title with Bar End
}
