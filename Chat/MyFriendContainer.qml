import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.global

Item {
    id: friendContainer
    property var infoContainer: info_container

    signal createMessageSignal(var jsonData, string type)

    RowLayout {
        anchors.fill: parent
        MyFriendListView {
            id: freindListView
            Layout.preferredWidth: 250
            Layout.fillHeight: true

            onUpdateFinished: {

                for (var i = 0; i < Global.friendModelData.count; i++) {
                    if (Global.friendModelData.get(
                                i).friendData.type === "user") {
                        friendContainer.createMessageSignal(
                                    Global.friendModelData.get(i).friendData,
                                    Global.friendModelData.get(
                                        i).friendData.type)
                    }
                }
            }
        }
        UserInfoContainer {
            id: info_container
            Layout.fillHeight: true
            Layout.fillWidth: true
            onSendMessage: (data, type) => {
                               friendContainer.createMessageSignal(data, type)
                           }
        }
    }
    Component.onCompleted: {
        mainWindow.noticeComponent.agreeFriendRequest.connect(
                    freindListView.addFriend)
    }
}
