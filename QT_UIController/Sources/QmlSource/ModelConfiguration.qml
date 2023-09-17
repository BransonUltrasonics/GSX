import QtQuick 2.0



ListModel{

    ListElement{ name:qsTr("General"); state: false}
    ListElement{ name:qsTr("User Management"); state: true}
    ListElement{ name:qsTr("User Authority"); state: false}
    ListElement{ name:qsTr("Change Password"); state: true}
    ListElement{ name:qsTr("Alarm Management"); state: true}
    ListElement{ name:qsTr("IP Configuration"); state: true}
}

//ListModel{

//    ListElement{name:qsTr("General")}
//    ListElement{name:qsTr("User Management")}
//    ListElement{name:qsTr("User Authority")}
//    ListElement{name:qsTr("Change Password")}
//    ListElement{name:qsTr("Alarm Management")}
//    ListElement{name:qsTr("IP Configuration")}
//}



