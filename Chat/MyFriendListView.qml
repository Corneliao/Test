import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.client
import QtQml
import com.global

Item {

    signal updateFinished
    Component.onCompleted: {
        Client.selectUserFirend(Global.myJsonData.account)
        Global.friendModelData = friend_model
    }

    function createAddFriendWindow() {
        let cont = Qt.createComponent("AddFriendContainer.qml")
        if (cont.status === Component.Ready) {
            let obj = cont.createObject(freindListView)
        }
    }

    function addFriend(jsonData) {
        friend_script.sendMessage({
                                      "type": "addFriendData",
                                      "value": jsonData,
                                      "model": friend_model
                                  })
    }

    WorkerScript {
        id: friend_script
        source: "qrc:/js/WorkerScript.js"
        onMessage: messageObject => {
                       if (messageObject.state === "finished") {
                           freindListView.updateFinished()

                           var accounts = []
                           for (var i = 0; i < friend_model.count; i++) {
                               var data = friend_model.get(i).friendData
                               if (data.type === "user") {
                                   var account = data.account
                                   accounts.push(account)
                               }
                           }
                           Client.getChatMessage(accounts)
                       }
                   }
    }

    Connections {
        target: Client
        function onUpdateFriendListSignal(json) {
            friend_script.sendMessage({
                                          "type": "updateFriend",
                                          "value": json,
                                          "model": friend_model
                                      })
        }
        function onFriendsData(array) {
            friend_script.sendMessage({
                                          "type": "friendData",
                                          "value": array,
                                          "model": friend_model
                                      })
        }
        function onReceivedGroupInvitedSignal(groupInfo) {
            friend_script.sendMessage({
                                          "type": "receivedGroupInvited",
                                          "value": groupInfo,
                                          "model": friend_model
                                      })
        }
        function onDeleteFriendSucceed(account) {
            for (var i = 0; i < friend_model.count; i++) {
                var data = friend_model.get(i).friendData
                if (data.type === "user") {
                    if (account === data.account) {
                        friend_model.remove(i)
                        console.log("已删除好友")
                        break
                    }
                }
            }
        }
    }

    CustomMenu {
        id: add_popup
    }

    CustomPopup {
        id: friend_popup
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            spacing: 10
            Layout.preferredWidth: parent.width
            Label {
                text: "Friend"
                font.pixelSize: 17
                font.bold: true
                color: Qt.color("white")
            }
            Loader {
                Layout.fillWidth: true
            }

            Image {
                source: Qt.url("image://asyncLocal/:/res/ico/add.png")
                Layout.preferredHeight: 23
                Layout.preferredWidth: 23
                sourceSize: Qt.size(23, 23)
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                    onClicked: {
                        add_popup.popup()
                    }
                }
            }

            Image {
                source: Qt.url("image://asyncLocal/:/res/ico/notice.png")
                Layout.preferredHeight: 23
                Layout.preferredWidth: 23
                sourceSize: Qt.size(23, 23)
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                    onClicked: {
                        mainWindow.noticeComponent.open()
                    }
                }
            }
        }

        // Rectangle {
        //     Layout.preferredHeight: 40
        //     Layout.fillWidth: true
        //     color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 40 / 255)
        //     radius: 10
        //     TextField {
        //         placeholderText: "Search"
        //         placeholderTextColor: Qt.rgba(255 / 255, 255 / 255, 255 / 255,
        //                                       200 / 255)
        //         width: parent.width - 5
        //         height: parent.height - 2
        //         color: Qt.rgba(255 / 255, 255 / 255, 255 / 255, 200 / 255)
        //         background: Rectangle {
        //             color: "transparent"
        //         }
        //         anchors.verticalCenter: parent.verticalCenter
        //     }
        // }
        Loader {
            Layout.preferredHeight: 15
        }

        ListView {
            id: friend_list_view
            clip: true
            model: friend_model
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 8
            currentIndex: -1

            delegate: Rectangle {
                id: friend_item
                required property var friendData
                required property int index
                width: ListView.view.width
                height: 80
                color: ListView.isCurrentItem ? Qt.rgba(0 / 255,
                                                        0 / 255, 0 / 255,
                                                        20 / 255) : Qt.color(
                                                    "transparent")
                radius: 10

                Image {
                    id: user_head
                    source: friendData.type
                            === "user" ? Qt.url(
                                             "image://async/http://127.0.0.1:9005/userhead/"
                                             + friend_item.friendData.account + ".jpg") : Qt.url(
                                             "image://async/http://127.0.0.1:9005/userhead/GroupHead.png")
                    width: 50
                    height: 50
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 8
                    }
                    sourceSize: Qt.size(50, 50)
                }
                Label {
                    font.pixelSize: 13
                    font.bold: true
                    color: Qt.color("white")
                    text: friendData.type === "user" ? friend_item.friendData.name : friend_item.friendData.groupName
                    anchors {
                        left: user_head.right
                        leftMargin: 15
                        verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onEntered: {
                        friend_item.color = Qt.rgba(0 / 255, 0 / 255,
                                                    0 / 255, 20 / 255)
                    }
                    onExited: {
                        if (friend_list_view.currentIndex !== friend_item.index)
                            friend_item.color = Qt.color("transparent")
                    }
                    onClicked: mouse => {
                                   if (mouse.button === Qt.LeftButton) {

                                       friend_list_view.currentIndex = friend_item.index
                                       friend_item.color = Qt.rgba(
                                           0 / 255, 0 / 255, 0 / 255, 20 / 255)
                                       friendContainer.infoContainer.showInfo(
                                           friend_item.friendData,
                                           friend_item.friendData.type)
                                   } else if (mouse.button === Qt.RightButton) {
                                       if (friend_item.friendData.type === "user") {
                                           friend_popup.popup()
                                           friend_popup.account = friend_item.friendData.account
                                       }
                                   }
                               }
                }
            }
            onCurrentIndexChanged: {
                for (var i = 0; i < friend_list_view.contentItem.children.length; i++) {
                    if (contentItem.children[i] instanceof Rectangle) {
                        if (contentItem.children[i].index !== friend_list_view.currentIndex) {
                            contentItem.children[i].color = "transparent"
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: friend_model
    }
}
