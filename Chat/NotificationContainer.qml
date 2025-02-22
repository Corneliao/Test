import QtQuick
import com.framelesswindow
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQml
import com.client
import com.global

Popup {

    signal agreeFriendRequest(var jsonData)

    width: 420
    height: 400
    padding: 2

    Material.background: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 10 / 255)
    contentItem: Item {
        id: noticeContentIten

        WorkerScript {
            id: notice_script
            source: "qrc:/js/WorkerScript.js"
        }

        Connections {
            target: Client
            function onReceivedRequestUserInfoSignal(data) {
                var json = JSON.parse(data)
                notice_script.sendMessage({
                                              "type": "request",
                                              "value": json,
                                              "model": notice_model
                                          })
            }
        }
        ColumnLayout {
            anchors.fill: parent
            spacing: 25
            Label {
                text: "通知"
                color: Qt.color("white")
                font.bold: true
                font.pixelSize: 15
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.topMargin: 10
                Layout.leftMargin: 10
            }

            ListView {
                id: noticeListView
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                spacing: 10
                model: notice_model
                delegate: Item {
                    id: notice_item
                    required property var jsonData
                    required property int index
                    width: ListView.view.width
                    height: 60
                    Rectangle {
                        width: parent.width - 40
                        height: parent.height
                        anchors.centerIn: parent
                        color: Qt.color("#62252c")
                        radius: 10
                        Image {
                            id: user_head
                            source: Qt.url(
                                        "image://async/http://127.0.0.1:9005/userhead/"
                                        + notice_item.jsonData.account + ".jpg")
                            sourceSize: Qt.size(40, 40)
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                leftMargin: 10
                            }
                        }

                        Label {
                            text: notice_item.jsonData.name
                            color: Qt.color("white")
                            font.pixelSize: 13
                            font.bold: true
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: user_head.right
                                leftMargin: 10
                            }
                        }

                        Image {
                            id: agree_ico
                            source: Qt.url("qrc:/res/ico/agree.png")
                            width: 25
                            height: 25
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                                rightMargin: 25
                            }
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    cursorShape = Qt.PointingHandCursor
                                }
                                onClicked: {
                                    Client.agreeFriendRequest(
                                                notice_item.jsonData.account,
                                                Global.myJsonData)
                                    notice_model.remove(notice_item.index)
                                    notice.agreeFriendRequest(
                                                notice_item.jsonData)
                                }
                            }
                        }
                        Image {
                            source: Qt.url("qrc:/res/ico/refuse.png")
                            width: 25
                            height: 25
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: agree_ico.left
                                rightMargin: 25
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
        }
        ListModel {
            id: notice_model
        }
    }
}
