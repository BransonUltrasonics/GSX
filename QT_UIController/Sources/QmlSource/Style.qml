pragma Singleton // We indicate that this QML Type is a singleton
import QtQuick 2.0

Item {
    readonly property int  style1:14
    readonly property int  style2:14

    readonly property int  style3:16
    readonly property int  style4:16

    readonly property int  style5:20
    readonly property int  style6:20

    readonly property int  style7:34
    readonly property int  style8:44
    readonly property int  style9:72
    readonly property int  style10:115

    readonly property int  style0:12

    property FontLoader light: FontLoader{
        source: "qrc:/Fonts/OpenSans-Light.ttf"
    }

    property FontLoader regular: FontLoader{
        source: "qrc:/Fonts/OpenSans-Regular.ttf"
    }

    property FontLoader semibold: FontLoader{
        source: "qrc:/Fonts/OpenSans-Semibold.ttf"
    }


    property string grayColor: "#ECECEC"       //the color after pressing the button
    property string blueColor: qmlPrimaryColor       //the color after releasing the button
    property string whiteColor: "#FFFFFF"      //the white font color
    property string blackColor: "#212529"       //the black font color





}
