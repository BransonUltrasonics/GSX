import QtQuick 2.0
import Style 1.0

Item
{
    property string listRecColor: "#ECECEC"
    property string listRecTextColor: "#212529"
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"

    readonly property int indexChangePasswordTab : 4

    Rectangle{
        id: itemRec
        width: parent.width
        height: parent.height
        color: listRecColor
        enabled: model.state

        Text {
            id: name
            text: model.name
            color: listRecTextColor
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            verticalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
            font.family: Style.regular.name
            font.pixelSize: Style.style5
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if((indexChangePasswordTab == index) && (login.m_username == login.WSLoggedUserName))
                {
                    messagePopup.open(messagePopup.warning_title,qmltextWebServicesLoggedIn,messagePopup.without_button)
                }
                else
                {
                testListView.currentIndex=index
                switchColor()
                }
            }
        }

        Component.onCompleted: {
            if (!itemRec.enabled)
            {
                itemRec.color = "#757575"
                name.color = "#BEC1C3"
            }
        }
    }

    Connections
    {
        target:testListView
        onCurrentIndexChanged: {
            switchColor()
        }
    }

    /**
       *@breif: To switch color based on the selected index
       **/
    function switchColor()
    {
        if(index==testListView.currentIndex)
        {
            listRecTextColor="#ffffff"
            listRecColor=pressedColor
        }
        if(index!=testListView.currentIndex)
        {
            listRecTextColor="#000000"
            listRecColor=unpressedColor
        }
    }
}


