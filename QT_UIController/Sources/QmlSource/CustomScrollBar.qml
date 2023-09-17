import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: scrollbar
    width: (handleSize + 2 * (backScrollbar.border.width))
    visible: (flickable.visibleArea.heightRatio < 1.0)
    property color shadowColor: "#000000"
    readonly property color bgColor: qmlPrimaryColor
    anchors {
        top: flickable.top
        right: flickable.right
        bottom: flickable.bottom
        margins: 1
    }

    property Flickable flickable               : null
    property int       handleSize              : 12

    /**
       *@breif: To change content while scrolling down
       **/
    function scrollDown () {
        flickable.contentY = Math.min (flickable.contentY + (flickable.height / 4), flickable.contentHeight - flickable.height)
    }

    /**
       *@breif: To change content while scrolling up
       **/
    function scrollUp () {
        flickable.contentY = Math.max (flickable.contentY - (flickable.height / 4), 0)
    }

    Binding {
        target: handle
        property: "y"
        value: (flickable.contentY * clicker.drag.maximumY / (flickable.contentHeight - flickable.height))

        when: (!clicker.drag.active)
    }
    Binding {
        target: flickable
        property: "contentY"
        value: (handle.y * (flickable.contentHeight - flickable.height) / clicker.drag.maximumY)
        when: (clicker.drag.active || clicker.pressed)
    }

    Rectangle {
        id: backScrollbar
        radius: 10;
        antialiasing: true
        color: bgColor
        opacity: 0.7

        width: 10
        anchors {
            //fill: parent;
            top: parent.top
            right: parent.right
            bottom:parent.bottom
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { }
        }
    }

    Item {
        id: groove;
        clip: true;
        anchors {
            fill: parent;
            topMargin: (backScrollbar.border.width+1)
            leftMargin: (backScrollbar.border.width+1)
            rightMargin: (backScrollbar.border.width+1)
            bottomMargin: (backScrollbar.border.width+1)
        }

        MouseArea {
            id: clicker;
            drag {
                target: handle
                minimumY: 0
                maximumY: (groove.height - handle.height)
                axis: Drag.YAxis
            }
            anchors { fill: parent }
            onClicked: { flickable.contentY = (mouse.y / groove.height * (flickable.contentHeight - flickable.height)) }
            onEntered: {
                                cursorShape=Qt.PointingHandCursor
                            }
        }
    }

    Item {
        id: handle;
        height: 20
        width: 20
        anchors
        {
            horizontalCenter: backScrollbar.horizontalCenter
        }

        Rectangle {
            id: backHandle
            radius: 25
            color: bgColor
            opacity: 1.0
            //color: (clicker.pressed ? "red" : "green");
            //  opacity: (flickable.moving ? 0.65 : 0.35);
            anchors { fill: parent }
            layer.enabled: true
            layer.effect: DropShadow {
                horizontalOffset:  1
                verticalOffset:  1
                color:  "#20000000"
                opacity: 0.8
                samples: 10
            }
            Behavior on opacity { NumberAnimation { duration: 100 } }
            //cursorShape=Qt.PointingHandCursor
        }
    }
}


