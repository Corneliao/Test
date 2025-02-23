import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: userinfo

    property var jsonData
    property string type

    signal sendMessage(var jsonData, string type)

    function showInfo(data, type) {
        if (stacklayout.children.length === 1) {
            let obj = info___.createObject(stacklayout, {
                                               "jsonData": data,
                                               "type": type
                                           })
            stacklayout.currentIndex = 1
        } else {
            let item = stacklayout.children[1]
            item.jsonData = data
        }
    }

    StackLayout {
        id: stacklayout
        anchors.fill: parent
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: Qt.color("transparent")
            Image {
                source: Qt.url("file:///D:/Doga/WallPaper/_20250215140909.jpg")
                width: 70
                height: 70
                anchors.centerIn: parent
            }
        }
    }

    Component {
        id: info___

        Rectangle {
            id: info_page
            property var jsonData
            property string type
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: Qt.color("transparent")

            Rectangle {
                width: parent.width - 150
                height: parent.height - 320
                color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 40 / 255)
                anchors.centerIn: parent
                radius: 10
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 25
                    color: Qt.color("transparent")
                    radius: 10

                    Image {
                        id: head
                        source: type
                                === "user" ? Qt.url(
                                                 "image://async/http://127.0.0.1:9005/userhead/"
                                                 + info_page.jsonData.account + ".jpg") : Qt.url(
                                                 "image://async/http://127.0.0.1:9005/userhead/GroupHead.png")
                        width: 70
                        height: 70
                        sourceSize: Qt.size(70, 70)
                        anchors {
                            left: parent.left
                            top: parent.top
                        }
                    }
                    Label {
                        id: name
                        text: info_page.jsonData.name === undefined ? "" : jsonData.name
                        font.bold: true
                        font.pixelSize: 15
                        color: Qt.color("white")
                        anchors {
                            left: head.right
                            leftMargin: 8
                            top: head.top
                            topMargin: 8
                        }
                    }

                    Image {
                        source: info_page.jsonData.gender
                                === "男" ? Qt.url(
                                              "qrc:/res/ico/man.png") : Qt.url(
                                              "qrc:/res/ico/woman.png")
                        visible: type === "user" ? true : false
                        width: 15
                        height: 15
                        anchors {
                            left: name.right
                            leftMargin: 12
                            verticalCenter: name.verticalCenter
                        }
                    }

                    Label {
                        text: type === "user" ? "账号：" + (info_page.jsonData.account) : "群号："
                                                + info_page.jsonData.groupInfo.groupID
                        font.pixelSize: 11
                        color: Qt.color("white")
                        anchors {
                            left: name.left
                            top: name.bottom
                            topMargin: 13
                        }
                    }

                    RowLayout {
                        id: email
                        spacing: 15
                        anchors {
                            top: head.bottom
                            topMargin: 30
                            left: head.left
                        }

                        Image {
                            source: Qt.url("qrc:/res/ico/email.png")
                            Layout.preferredHeight: 30
                            Layout.preferredWidth: 30
                        }
                        Label {
                            text: info_page.jsonData.email
                            color: Qt.color("white")
                        }
                    }

                    RowLayout {
                        spacing: 15
                        anchors {
                            top: email.bottom
                            topMargin: 30
                            left: email.left
                        }

                        Image {
                            source: Qt.url("qrc:/res/ico/phone.png")
                            Layout.preferredHeight: 30
                            Layout.preferredWidth: 30
                        }
                        Label {
                            text: info_page.jsonData.phone
                            color: Qt.color("white")
                        }
                    }

                    Rectangle {
                        width: 180
                        height: 35
                        radius: 10
                        color: Qt.color("#62252c")
                        anchors {
                            bottom: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                        }

                        Label {
                            color: Qt.color("white")
                            font.bold: true
                            text: "发消息"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                            }
                            onClicked: {
                                userinfo.sendMessage(info_page.jsonData,
                                                     info_page.type)
                            }
                        }
                    }
                }
            }
        }
    }
}
