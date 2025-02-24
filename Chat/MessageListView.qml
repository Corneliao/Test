import QtQuick
import com.global
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQml

Item {
    id: history_message

    signal currentIndexChanged(int index)

    function createMessage(jsonData, message, type) {
        historyMessage_script.sendMessage({
                                              "type": "increateMessage",
                                              "value": jsonData,
                                              "model": message_model,
                                              "message": message,
                                              "itemType": type
                                          })
    }

    function setCurrentIndex(account) {
        for (var i = 0; i < message_model.count; i++) {
            let account_ = message_model.get(i).messageData.account
            if (account_ === account) {
                message_list_view.currentIndex = i
                var item = message_list_view.currentItem
                item.color = Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)
                break
            }
        }
    }

    function modifyMessageData(account, message, messageType) {
        for (var i = 0; i < message_model.count; i++) {
            var data = message_model.get(i).messageData
            if (data.account === account) {
                data.message = message
                message_model.set(i, {
                                      "messageData": data
                                  })
            }
        }
    }

    function increateUnreadMessageCount(account, message) {
        var viewCurrentIndex = message_list_view.currentIndex

        historyMessage_script.sendMessage({
                                              "type": "updateMessage",
                                              "value": account,
                                              "model": message_model,
                                              "message": message,
                                              "viewIndex": viewCurrentIndex
                                          })
    }

    WorkerScript {
        id: historyMessage_script
        source: "qrc:/js/WorkerScript.js"
        onMessage: messageObject => {
                       if (messageObject.type === "increateMessageFinished") {
                           message_list_view.currentIndex = message_list_view.count - 1
                       }
                       if (messageObject.type === "haveUnreadMessage") {
                           sendMessage({
                                           "type": "countUnreadCount",
                                           "value": "",
                                           "model": message_model
                                       })
                       }
                       if (messageObject.type === "totolUnreadCount") {
                           Global.unreadMessageCount = messageObject.count.toString()
                       }
                   }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 8

        Label {
            text: "Message"
            font.bold: true
            Layout.alignment: Qt.AlignLeft
            font.pixelSize: 17
            color: "white"
        }

        Rectangle {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 40 / 255)
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
            id: message_list_view
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: message_model
            spacing: 8
            clip: true
            currentIndex: -1

            ScrollIndicator.vertical: ScrollIndicator {}
            delegate: Rectangle {
                id: message_item
                required property var messageData
                required property var index
                required property var unreadCount
                required property string itemType

                property color bkColor: Qt.rgba(0 / 255, 0 / 255,
                                                0 / 255, 20 / 255)
                width: ListView.view.width
                height: 80
                radius: 15
                color: ListView.isCurrentItem ? Qt.rgba(0 / 255,
                                                        0 / 255, 0 / 255,
                                                        20 / 255) : Qt.color(
                                                    "transparent")
                Image {
                    id: user_head
                    source: itemType
                            === "user" ? Qt.url(
                                             "image://async/http://127.0.0.1:9005/userhead/"
                                             + message_item.messageData.account + ".jpg") : Qt.url(
                                             "image://async/http://127.0.0.1:9005/userhead/GroupHead.png")
                    width: 50
                    height: 50
                    sourceSize: Qt.size(55, 55)
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 8
                    }
                }

                Label {
                    id: user_name
                    text: itemType === "user" ? message_item.messageData.name : message_item.messageData.groupName
                    font.pixelSize: 13
                    font.bold: true
                    color: "white"
                    anchors {
                        top: user_head.top
                        left: user_head.right
                        leftMargin: 5
                        topMargin: 8
                    }
                }
                Label {
                    id: user_message
                    text: itemType === "user" ? messageData.message : ""
                    font.pixelSize: 11
                    color: Qt.rgba(255 / 255, 255 / 255, 255 / 255, 200 / 255)
                    anchors {
                        left: user_name.left
                        bottom: parent.bottom
                        bottomMargin: 20
                    }
                }

                Rectangle {
                    id: unread_bubble
                    visible: message_item.unreadCount === 0 ? false : true
                    width: 18
                    height: 18
                    radius: 18 / 2
                    color: Qt.color("#b14347")
                    Label {
                        anchors.centerIn: parent
                        font.pixelSize: 10
                        color: Qt.color("white")
                        text: message_item.unreadCount
                    }
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        verticalCenter: user_message.verticalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        message_item.color = message_item.bkColor
                    }
                    onClicked: {
                        message_item.ListView.view.currentIndex = message_item.index
                        message_item.color = message_item.bkColor
                        historyMessage_script.sendMessage({
                                                              "type": "clearUnreadCount",
                                                              "value": message_item.index,
                                                              "model": message_model
                                                          })
                    }
                    onExited: {
                        if (message_item.ListView.view.currentIndex !== message_item.index) {
                            message_item.color = "transparent"
                        }
                    }
                }
            }

            onCurrentIndexChanged: {
                history_message.currentIndexChanged(
                            message_list_view.currentIndex)
                for (var i = 0; i < message_list_view.contentItem.children.length; i++) {
                    if (contentItem.children[i] instanceof Rectangle) {
                        if (contentItem.children[i].index !== message_list_view.currentIndex) {
                            contentItem.children[i].color = "transparent"
                        }
                    }
                }
            }
        }

        ListModel {
            id: message_model
        }

        Component.onCompleted: {
            Global.messageModel = message_model
        }
    }
}
