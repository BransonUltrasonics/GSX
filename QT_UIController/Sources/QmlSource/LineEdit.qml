import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import Style 1.0
/**
  * Implementaion based on the Next Gen HMI Pattern Library
  * Ref: Next Gen HMI Pattern Library
  * Text Field:
  * Color:
  * Background: HEX #FFFFFF
  * Label: HEX #868e96
  * Value: HEX #68A4EB
  * Stroke: HEX #68A4EB
  * Style:
  * Dropshadow: None
  * Stroke: 1px
  * Text:
  * Label: Style 2 (ALL CAPS)
  * Value: Style 1
  * Behavior:
  * Text fields with text values should be left aligned while text fields with number values should be right aligned.
  * Single value text fields should be Width: 150px
  * For paragraphs, set the text field dimensions to the expected size of the paragraph. Do not make text fields too large or too small.
  */


TextField {
    property alias text: input.text
    property alias input: input
    property alias readOnlyText: input.readOnly
    property alias placehodertext: input.placeholderText
    property alias echoMode: input.echoMode
    property alias horizontalAlignment: input.horizontalAlignment
    property alias textFieldObj: input
    property string bgColor_: "#FFFFFF"
    property string  strFieldName: ""
    property string textFieldColor: qmlPrimaryColor


    /**
    *@breif: To set background color
    **/
    function setBackgroundColor(fontColor)
    {
        bgColor_ = fontColor
    }    

    /**
    *@breif: To set text color
    **/
    function setTextColor(fontColor)
    {
        textFieldColor = fontColor
    }

    /**
    *@breif: To set field name
    **/
    function setFieldName(strInputName)
    {
        strFieldName=strInputName;
    }

    id: input
    font.pixelSize: Style.style3
    font.family: Style.regular.name
    font.bold: false
    width: parent.width
    height: parent.height
    maximumLength: 32
    placeholderText : ""
    echoMode:TextInput.Normal
    readOnly: false

    style: TextFieldStyle {
        id: textStyleId
        textColor: textFieldColor
        background: Rectangle {
            id:backGroundId
            radius: 2
            color: bgColor_
            border.color: qmlPrimaryColor
            border.width: 1
        }
    }
}
