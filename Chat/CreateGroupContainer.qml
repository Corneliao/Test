import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import com.global
import com.client

Popup {
    anchors.centerIn: parent
    dim: true
    modal: true
    focus: true
    padding: 0
    Material.roundedScale: Material.MediumScale
    Material.background: Qt.color("transparent")
    contentItem: Rectangle {
        radius: 10
        color: Qt.color("#b14347")

        ColumnLayout {
            anchors.fill: parent
            CustomSearchBar {
                id: group_name
                Layout.preferredWidth: parent.width - 200
                Layout.preferredHeight: 40
                placeholder_text: "群聊名称"
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 40 / 255)
                radius: 10
                Layout.topMargin: 80
                Layout.alignment: Qt.AlignHCenter
            }
            Flow {
                id: flow_layout
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 20
                spacing: 20
                Repeater {
                    id: repeat_
                    model: Global.friendModelData
                    clip: true
                    delegate: Rectangle {
                        property bool isChecked: check_box.checkState
                        required property int index
                        required property var friendData
                        visible: friendData.type === "user" ? true : false
                        color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 100 / 255)
                        width: 200
                        height: 80
                        radius: 10
                        RowLayout {
                            anchors.fill: parent
                            spacing: 10
                            Loader {
                                Layout.fillWidth: true
                            }
                            Image {
                                id: user_head
                                Layout.preferredHeight: 50
                                Layout.preferredWidth: 50
                                source: friendData.type === "user" ? Qt.url(
                                                                         "image://async/http://127.0.0.1:9005/userhead/" + friendData.account + ".jpg") : ""
                                sourceSize: Qt.size(50, 50)
                            }
                            Label {
                                id: user_name
                                text: friendData.type === "user" ? friendData.name : ""
                                color: Qt.color("white")
                                font.bold: true
                            }
                            CheckBox {
                                id: check_box
                            }
                            Loader {
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }
            Rectangle {
                id: create_button
                Layout.preferredWidth: 120
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.bottomMargin: 10
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 100 / 255)
                radius: 10
                Label {
                    text: "创建"
                    anchors.centerIn: parent
                    color: Qt.color("white")
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                    onClicked: {
                        if (group_name.searchBar.text.length <= 0)
                            return
                        var users = []
                        for (var i = 0; i < Global.friendModelData.count; i++) {
                            if (repeat_.itemAt(i).isChecked) {
                                users.push(repeat_.itemAt(i).friendData)
                            }
                        }
                        Client.createGroupChat(Global.myJsonData, users,
                                               group_name.searchBar.text)
                    }
                }
            }
        }
    }
}
