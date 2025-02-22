import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.client
import QtQml
import com.global

Item {

    Component.onCompleted: {
        Client.selectUserFirend(Global.myJsonData.account)
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
    }

    CustomMenu {
        id: add_popup
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

        Rectangle {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)
            radius: 10
            TextField {
                placeholderText: "Search"
                placeholderTextColor: Qt.rgba(255 / 255, 255 / 255, 255 / 255,
                                              200 / 255)
                width: parent.width - 5
                height: parent.height - 2
                color: Qt.rgba(255 / 255, 255 / 255, 255 / 255, 200 / 255)
                background: Rectangle {
                    color: "transparent"
                }
                anchors.verticalCenter: parent.verticalCenter
            }
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
                    source: Qt.url(
                                "image://async/http://127.0.0.1:9005/userhead/"
                                + friend_item.friendData.account + ".jpg")
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
                    text: friend_item.friendData.name
                    anchors {
                        left: user_head.right
                        leftMargin: 15
                        verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        friend_item.color = Qt.rgba(0 / 255, 0 / 255,
                                                    0 / 255, 20 / 255)
                    }
                    onExited: {
                        if (friend_list_view.currentIndex !== friend_item.index)
                            friend_item.color = Qt.color("transparent")
                    }
                    onClicked: {
                        friend_list_view.currentIndex = friend_item.index
                        friend_item.color = Qt.rgba(0 / 255, 0 / 255,
                                                    0 / 255, 20 / 255)
                        friendContainer.infoContainer.showInfo(
                                    friend_item.friendData)
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
