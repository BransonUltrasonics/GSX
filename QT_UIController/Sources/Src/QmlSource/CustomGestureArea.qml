import QtQuick 2.0

MouseArea {
    property int swipeTreshold: 32
    property bool ready: false

    readonly property string dirUp:     "up"
    readonly property string dirDown:   "down"
    readonly property string dirLeft:   "left"
    readonly property string dirRight:  "right"

    signal move(int x, int y)
    signal swipe(string direction, int distance)

    property point originPosition

    onPressed: {
        drag.axis = Drag.XAndYAxis
        originPosition = Qt.point(mouse.x, mouse.y)
    }

    onPositionChanged: {
        switch (drag.axis) {
        case Drag.XAndYAxis:
            if (Math.abs(mouse.x - originPosition.x) > swipeTreshold)
                drag.axis = Drag.XAxis
            else if (Math.abs(mouse.y - originPosition.y) > swipeTreshold)
                drag.axis = Drag.YAxis
            break

        case Drag.XAxis: move(mouse.x - originPosition.x, 0) ; break
        case Drag.YAxis: move(0, mouse.y - originPosition.y) ; break
        }
    }

    onReleased: {
        switch (drag.axis) {
        case Drag.XAndYAxis: canceled(mouse) ; break
        case Drag.XAxis: swipe(mouse.x - originPosition.x < 0 ? dirLeft : dirRight, Math.abs(mouse.x-originPosition.x)) ; break
        case Drag.YAxis: swipe(mouse.y - originPosition.y < 0 ? dirUp : dirDown, Math.abs(mouse.y-originPosition.y)) ; break
        }
    }
}