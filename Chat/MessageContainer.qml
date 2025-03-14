import QtQuick
import QtQuick.Layouts
import com.client
import QtQml
import com.global

Item {

    property var hostoryMessageView: hostoryMessageListView

    Connections {
        target: Client
        function onReceivedMessageSignal(senderData, message, messageType) {

            //聊天窗口存在则直接更新数据
            for (var i = 0; i < chatwindow_stacklayout.children.length; i++) {
                var item = chatwindow_stacklayout.children[i]
                if (item !== null) {
                    if (senderData.account === item.friendJsonData.account) {
                        item.increateReceiveMessage(senderData, message,
                                                    messageType)

                        return
                    }
                }
            }

            //聊天窗口不存在测则创建聊天窗口
            messageContainer.createChatWindowReceived(senderData, message,
                                                      messageType)
        }

        function onChatMessageSignal(array) {
            for (var i = 0; i < array.length; i++) {
                var account = array[i].friend
                for (var j = 0; j < chatwindow_stacklayout.children.length; j++) {
                    var item = chatwindow_stacklayout.children[j]
                    if (item !== null) {
                        if (account === item.friendJsonData.account) {
                            var messageArray = array[i].History
                            for (var h = 0; h < messageArray.length; h++) {
                                var type = messageArray[h].type
                                var message = messageArray[h].message
                                if (type === "send") {
                                    item.increateSendHistoryMessage(account,
                                                                    message)
                                } else {
                                    item.increateReceiveHistoryMessage(account,
                                                                       message)
                                }
                            }
                        }
                    }
                }
            }
        }

        function onReceivedFileSignal(senderData, fileInfo, messageType) {
            //聊天窗口存在则直接更新数据
            for (var i = 0; i < chatwindow_stacklayout.children.length; i++) {
                var item = chatwindow_stacklayout.children[i]
                if (item !== null) {
                    if (senderData.account === item.friendJsonData.account) {
                        item.increateReceiveFile(senderData, fileInfo,
                                                 messageType)

                        return
                    }
                }
            }

            //聊天窗口不存在测则创建聊天窗口
            messageContainer.createChatWindowReceivedFile(senderData, fileInfo,
                                                          messageType)
        }

        function onReceiveGroupMessage(groupInfo, senderData, message) {
            console.log("收到群聊消息")
            for (var i = 0; i < chatwindow_stacklayout.children.length; i++) {
                var item = chatwindow_stacklayout.children[i]
                if (item !== null) {
                    if (groupInfo.type === "Group"
                            && groupInfo.groupID === item.friendJsonData.groupID) {
                        item.increateReceiveMessage(senderData, message, "Text")
                        return
                    }
                }
            }
            messageContainer.createChatWindowReceivedGroupMessage(groupInfo,
                                                                  senderData,
                                                                  message)
        }
    }

    //当第一次接受到好友消息时创建聊天窗口
    function createChatWindowReceived(senderData, message, messageType) {

        let comp = Qt.createComponent("ChatWindow.qml", chatwindow_stacklayout)
        if (comp.status === Component.Ready) {
            let obj = comp.createObject(chatwindow_stacklayout, {
                                            "friendJsonData": senderData
                                        })
            obj.receivedMessageSignal.connect(increateUnreadMessageCount)
            obj.sendMessageSignal.connect(sendMessageSignal)
            hostoryMessageListView.createMessage(senderData, message, "user")
            obj.increateReceiveMessage(senderData, message, messageType)
        }
    }

    function createChatWindowReceivedFile(senderData, fileInfo, messageType) {
        let comp = Qt.createComponent("ChatWindow.qml", chatwindow_stacklayout)
        if (comp.status === Component.Ready) {
            let obj = comp.createObject(chatwindow_stacklayout, {
                                            "friendJsonData": senderData
                                        })
            obj.receivedMessageSignal.connect(increateUnreadMessageCount)
            obj.sendMessageSignal.connect(sendMessageSignal)
            hostoryMessageListView.createMessage(senderData,
                                                 fileInfo.fileName, "user")
            obj.increateReceiveFile(senderData, fileInfo, messageType)
        }
    }

    function createChatWindowReceivedGroupMessage(groupInfo, senderData, message) {
        //创建聊天窗口
        let comp = Qt.createComponent("ChatWindow.qml", chatwindow_stacklayout)
        if (comp.status === Component.Ready) {
            let obj = comp.createObject(chatwindow_stacklayout, {
                                            "friendJsonData": groupInfo
                                        })
            //obj.receivedMessageSignal.connect(increateUnreadMessageCount)
            obj.sendMessageSignal.connect(sendMessageSignal)
            hostoryMessageListView.createMessage(groupInfo, message, "Group")
            obj.increateReceiveMessage(senderData, message, "Text")
        }
    }

    function increateUnreadMessageCount(account, message) {
        hostoryMessageListView.increateUnreadMessageCount(account, message)
    }

    //创建发送消息聊天窗口
    function createChatWindow(data) {

        var account = 0
        if (data.type === "user") {
            account = data.account
        } else if (data.type === "Group") {
            account = data.groupID
        }

        for (var i = 0; i < chatwindow_stacklayout.children.length; i++) {
            var item_ = chatwindow_stacklayout.children[i]
            if (item_ !== null) {
                if (account === item_.friendJsonData.account) {
                    hostoryMessageListView.setCurrentIndex(account)
                    return
                }
            }
        }

        let comp = Qt.createComponent("ChatWindow.qml", chatwindow_stacklayout)
        if (comp.status === Component.Ready) {
            let obj = comp.createObject(chatwindow_stacklayout, {
                                            "friendJsonData": data
                                        })
            obj.sendMessageSignal.connect(sendMessageSignal)
            // obj.receivedMessageSignal.connect(increateUnreadMessageCount)
        }
    }

    //发送消息时更新消息列表数据
    function sendMessageSignal(receiver, message, messageType) {
        hostoryMessageListView.modifyMessageData(receiver, message, messageType)
    }

    RowLayout {
        anchors.fill: parent

        MessageListView {
            id: hostoryMessageListView
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: 250
            onCurrentIndexChanged: index => {
                                       chatwindow_stacklayout.currentIndex = index
                                   }
        }
        StackLayout {
            id: chatwindow_stacklayout
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
