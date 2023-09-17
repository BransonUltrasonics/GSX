import QtQuick 2.0

Item {
    property alias dynamicModel: testListView.model
    property int setCurrentIndex: 0
    property int  itemWidth: 150
    property int  itemHeight: 40

    property alias customListviewCurrentIndex: testListView.currentIndex
    signal  customCurrentIndexChanged(var cIndex)

    ListView{
        id: testListView
        width: parent.width
        height: parent.height
        currentIndex:-1
        interactive: false
        model: dynamicModel
        spacing: 1

        onCurrentIndexChanged: {
            customCurrentIndexChanged(testListView.currentIndex)
        }

        delegate: CustomListItem
        {
            id: listItem
            width: itemWidth; height: itemHeight
        }

        Component.onCompleted: {
            testListView.currentIndex = setCurrentIndex
        }
    }
}
