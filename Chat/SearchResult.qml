import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQml
import com.client

Pane {

    padding: 2
    Material.elevation: 6
    Material.roundedScale: Material.MediumScale
    contentItem: Item {

        id: info
        property string userAccount
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Image {
                Layout.topMargin: 20
                source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                               + info.userAccount + ".jpg")
                Layout.preferredHeight: 55
                Layout.preferredWidth: 55
                Layout.alignment: Qt.AlignHCenter
                sourceSize: Qt.size(55, 55)
            }

            Rectangle {
                id: user_info
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)
                Layout.preferredWidth: parent.width - 250
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter
                radius: 10

                Connections {
                    target: Client
                    function onUserInfoSignal(data) {
                        var json = JSON.parse(data)
                        info.userAccount = json.account
                        script.sendMessage({
                                               "type": "search",
                                               "value": json,
                                               "model": label_model
                                           })
                    }
                }

                WorkerScript {
                    id: script
                    source: "qrc:/js/WorkerScript.js"
                }

                ListModel {
                    id: label_model
                }

                Flow {
                    anchors.fill: parent
                    anchors.margins: 25
                    spacing: 20
                    Repeater {
                        model: label_model
                        delegate: Item {
                            id: user_label
                            required property string title
                            required property string value

                            width: title_label.width
                                   > value_label.width ? title_label.width : value_label.width
                            height: title_label.height + value_label.height
                            Label {
                                id: title_label
                                text: user_label.title
                                font.pixelSize: 12
                                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255,
                                               120 / 255)
                            }
                            Label {
                                id: value_label
                                text: user_label.value
                                font.pixelSize: 10
                                anchors {
                                    left: title_label.left
                                    top: title_label.bottom
                                    topMargin: 2
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 40
                color: Qt.color("#0098fe")
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 15
                radius: 10
                Label {
                    text: "加好友"
                    color: Qt.color("white")
                    anchors.centerIn: parent
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                    onClicked: {
                        if (info.userAccount.length <= 0
                                || info.userAccount === mainWindow.userAccount)
                            return
                        Client.addFriend(mainWindow.userAccount,
                                         info.userAccount)
                    }
                }
            }
        }
    }
}
