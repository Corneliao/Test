import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Basic
import QtQml
import com.global
import com.client
import Qt.labs.platform

Item {
    id: chat_window
    property var friendJsonData
    property var waitFileQueue: []
    signal sendMessageSignal(string receiver, string message, string messageType)
    signal receivedMessageSignal(string account, string message)

    ListModel {
        id: message_item_model
    }

    FileDialog {
        id: file_dialog
        fileMode: FileDialog.OpenFile
        onAccepted: {

            var fileData = Client.parseFileInfo(currentFile.toString(
                                                    ).substring(8))

            chatwindow_script.sendMessage({
                                              "type": "sendMessage",
                                              "value": Global.myJsonData.account,
                                              "model": message_item_model,
                                              "messageType": "File",
                                              "fileInfo": {
                                                  "fileName": fileData.fileName,
                                                  "fileSize": fileData.fileSize
                                              }
                                          })
        }
    }

    FileDialog {
        id: picture_dialog
        nameFilters: ["图片(*.png);(*.jpg)"]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var fileData = Client.parseFileInfo(currentFile.toString(
                                                    ).substring(8))
            chatwindow_script.sendMessage({
                                              "type": "sendMessage",
                                              "value": Global.myJsonData.account,
                                              "model": message_item_model,
                                              "messageType": "Picture",
                                              "fileInfo": fileData
                                          })
        }
    }

    WorkerScript {
        id: chatwindow_script
        source: Qt.url("qrc:/js/WorkerScript.js")
        onMessage: messageObject => {
                       if (messageObject.type === "sendFinished"
                           || messageObject.type === "recvFinished") {
                           messageView.positionViewAtEnd()
                       }
                       if (messageObject.type === "sendFinished"
                           && messageObject.messageType === "file") {
                           Client.sendFile(Global.myJsonData,
                                           friendJsonData.account,
                                           file_dialog.currentFile.toString(
                                               ).substring(8),
                                           messageView.count - 1, "File")

                           //消息发送完成后触发信号
                           chat_window.sendMessageSignal(
                               chat_window.friendJsonData.account,
                               messageObject.fileName, "File")
                       }

                       if (messageObject.type === "sendFinished"
                           && messageObject.messageType === "Picture") {
                           Client.sendFile(Global.myJsonData,
                                           friendJsonData.account,
                                           picture_dialog.currentFile.toString(
                                               ).substring(8),
                                           messageView.count - 1, "Picture")

                           chat_window.sendMessageSignal(
                               chat_window.friendJsonData.account,
                               messageObject.fileName, "File")
                       }

                       if ((messageObject.type === "recvFinished")
                           && (messageObject.messageType === "file"
                               || messageObject.messageType === "Picture")) {
                           Client.downloadFile(messageView.count - 1,
                                               messageObject.fileName,
                                               messageObject.messageType)
                       }
                   }
    }

    function increateReceiveMessage(senderData, message, messageType) {
        chatwindow_script.sendMessage({
                                          "type": "receivedMessage",
                                          "value": senderData,
                                          "message": message,
                                          "model": message_item_model,
                                          "messageType": messageType
                                      })
        chat_window.receivedMessageSignal(senderData.account, message)
    }

    function increateReceiveFile(senderData, fileInfo, messageType) {
        chatwindow_script.sendMessage({
                                          "type": "receivedFile",
                                          "value": senderData,
                                          "fileInfo": fileInfo,
                                          "model": message_item_model,
                                          "messageType": messageType
                                      })
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.alignment: Qt.AlignTop

            Image {
                id: user_head
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    leftMargin: 8
                }
                source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                               + chat_window.friendJsonData.account + ".jpg")
                Layout.preferredHeight: parent.height - 15
                Layout.preferredWidth: parent.height - 15
                sourceSize: Qt.size(parent.height - 15, parent.height - 15)
            }
            Label {
                id: user_name
                text: chat_window.friendJsonData.name
                font.pixelSize: 13
                color: Qt.color("white")
                font.bold: true
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: user_head.right
                    leftMargin: 5
                }
            }

            color: "transparent"
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)
            Layout.rightMargin: 15
            radius: 10

            ListView {
                id: messageView
                anchors.fill: parent
                anchors.margins: 15
                model: message_item_model
                spacing: 20
                clip: true
                delegate: MessageItem {
                    required property string stateType
                    required property string account
                    required property string message
                    required property string messageType
                    required property var fileInfo
                    width: ListView.view.width
                    state_type: stateType
                    useraccount: account
                    usermessage: message
                    message_type: messageType
                    fileInfoData: fileInfo
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.rightMargin: 15
            Layout.preferredHeight: 40
            Layout.alignment: Qt.AlignBottom
            color: Qt.color("transparent")
            Layout.bottomMargin: 15
            radius: 10

            Rectangle {
                width: parent.width - 50
                height: parent.height
                radius: parent.radius
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)

                TextField {
                    id: message_input
                    width: parent.width - 95
                    height: parent.height - 3
                    anchors.verticalCenter: parent.verticalCenter
                    color: Qt.rgba(255 / 255, 255 / 255, 255 / 255, 200 / 255)
                    background: Rectangle {
                        color: "transparent"
                    }
                    placeholderText: "Enter message for you want..."
                    placeholderTextColor: Qt.rgba(255 / 255, 255 / 255,
                                                  255 / 255, 200 / 255)
                    Keys.onReturnPressed: {
                        chatwindow_script.sendMessage({
                                                          "type": "sendMessage",
                                                          "value": Global.myJsonData.account,
                                                          "message": message_input.text,
                                                          "model": message_item_model,
                                                          "messageType": "Text"
                                                      })
                        Client.sendMessage(Global.myJsonData,
                                           chat_window.friendJsonData.account,
                                           message_input.text, "Text")
                        chat_window.sendMessageSignal(
                                    chat_window.friendJsonData.account,
                                    message_input.text, "Text")
                        message_input.clear()
                    }
                }

                Image {
                    id: picture_ico
                    source: Qt.url("qrc:/res/ico/picture.png")
                    width: 25
                    height: 25
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            cursorShape = Qt.PointingHandCursor
                        }
                        onClicked: {
                            picture_dialog.open()
                        }
                    }
                }

                Image {
                    id: file_ico
                    source: Qt.url("qrc:/res/ico/file.png")
                    width: 28
                    height: 28
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: picture_ico.left
                    anchors.rightMargin: 13
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            cursorShape = Qt.PointingHandCursor
                        }
                        onClicked: {
                            file_dialog.open()
                        }
                    }
                }
            }

            Rectangle {
                width: parent.height
                height: parent.height
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 30 / 255)
                radius: 10
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: 10
                }
                Image {
                    width: 25
                    height: 25
                    anchors.centerIn: parent
                    source: Qt.url("qrc:/res/ico/send.png")
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                }
            }
        }
    }
}
