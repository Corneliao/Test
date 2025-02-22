import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: friendContainer
    property var infoContainer: info_container

    signal createMessageSignal(var jsonData)

    RowLayout {
        anchors.fill: parent
        MyFriendListView {
            id: freindListView
            Layout.preferredWidth: 250
            Layout.fillHeight: true
        }
        UserInfoContainer {
            id: info_container
            Layout.fillHeight: true
            Layout.fillWidth: true
            onSendMessage: data => {
                               friendContainer.createMessageSignal(data)
                           }
        }
    }
    Component.onCompleted: {
        mainWindow.noticeComponent.agreeFriendRequest.connect(
                    freindListView.addFriend)
    }
}
