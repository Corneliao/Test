import QtQuick
import com.framelesswindow
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Controls
import com.client
import QtQml
import com.global

FramelessWindow {
    property int userHeadSize: 80
    width: 320
    height: 450
    id: loginWindow

    signal loginSignal(string account)
    Component.onCompleted: {
        loginWindow.moveCenter()
        loginWindow.setWindowTitleBar(title_bar)
        Client.connectServer()
    }

    function createRegisterWindow() {
        let component = Qt.createComponent("Register.qml")
        if (component.status === Component.Ready) {
            let object = component.createObject()
        }
    }

    Connections {
        target: Client
        function onLoginSignal(data, state) {
            if (state) {
                loginWindow.loginSignal(username_input.text)
                Client.setUserAccount(username_input.text)
                Global.myJsonData = data
            } else {
                animation.to = 10
                animation.restart()
                timer_.restart()
            }
        }
    }

    NumberAnimation {
        id: animation
        target: notice__
        properties: "y"
        to: 10
        duration: 700
        easing.type: Easing.InOutQuint
    }

    Timer {
        id: timer_
        repeat: false
        interval: 1700
        onTriggered: {
            animation.to = -notice__.height
            animation.restart()
        }
    }

    Rectangle {
        id: notice__
        radius: 8
        width: 180
        height: 35
        anchors.horizontalCenter: parent.horizontalCenter
        y: -height
        RowLayout {
            anchors.fill: parent
            Loader {
                Layout.fillWidth: true
            }
            Image {
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                source: Qt.url("qrc:/res/ico/refuse.png")
            }
            Loader {
                Layout.preferredWidth: 10
            }

            Label {
                text: "账号或密码错误"
                font.pixelSize: 11
                font.bold: true
            }
            Loader {
                Layout.fillWidth: true
            }
        }

        z: 1
    }

    Rectangle {

        anchors.fill: parent

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.color("#fbc2eb")
            } // 起始颜色
            GradientStop {
                position: 1.0
                color: Qt.color("#a6c1ee")
            } // 结束颜色
        }

        ColumnLayout {
            anchors.fill: parent

            spacing: 15
            Item {
                id: title_bar
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 40
                Layout.fillWidth: true
            }

            Rectangle {

                color: Qt.color("white")
                radius: loginWindow.userHeadSize / 2
                Layout.preferredHeight: loginWindow.userHeadSize
                Layout.preferredWidth: loginWindow.userHeadSize
                Layout.alignment: Qt.AlignHCenter
                Image {
                    id: userhead
                    source: Qt.url(
                                "image://async/http://127.0.0.1:9005/userhead/"
                                + username_input.text + ".jpg")
                    height: loginWindow.userHeadSize - 4
                    width: loginWindow.userHeadSize - 4
                    anchors.centerIn: parent
                    sourceSize: Qt.size(loginWindow.userHeadSize - 4,
                                        loginWindow.userHeadSize - 4)
                }
            }

            Loader {
                Layout.preferredHeight: 10
                Layout.preferredWidth: 1
            }

            Rectangle {

                Layout.preferredHeight: 40
                Layout.preferredWidth: 260
                radius: 10
                Layout.alignment: Qt.AlignHCenter
                TextField {
                    id: username_input
                    width: parent.width - 5
                    height: parent.height - 2
                    placeholderText: "账号 "
                    anchors.verticalCenter: parent.verticalCenter
                    antialiasing: true
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }

            Rectangle {

                Layout.preferredHeight: 40
                Layout.preferredWidth: 260
                radius: 10
                Layout.alignment: Qt.AlignHCenter
                TextField {
                    id: password_input
                    width: parent.width - 5
                    height: parent.height - 2
                    placeholderText: "密码"
                    echoMode: TextField.Password
                    anchors.verticalCenter: parent.verticalCenter
                    antialiasing: true
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }

            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 260
                Layout.preferredHeight: 40
                color: Qt.color("#0098fe")
                radius: 10
                Label {
                    text: "登录"
                    color: Qt.color("white")
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (username_input.text.length <= 0
                                || password_input.text.length <= 0)
                            return
                        Client.login(username_input.text, password_input.text)
                    }
                }
            }

            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Label {
                text: "注册账号"
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                Layout.bottomMargin: 10
                color: Qt.color("#0098fe")
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }

                    onClicked: {
                        loginWindow.createRegisterWindow()
                    }
                }
            }
        }
    }
}
