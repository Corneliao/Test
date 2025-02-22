import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import com.framelesswindow
import Qt.labs.platform
import com.captcha
import com.client
import com.userinfo

FramelessWindow {
    id: register_window

    minimumHeight: 600
    minimumWidth: 800

    property string userHeadPath
    Component.onCompleted: {
        register_window.moveCenter()
        register_window.setWindowTitleBar(spacerItem)
    }

    ColumnLayout {
        function adjustWindowMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin
            anchors.topMargin = top_margin
            anchors.rightMargin = right_margin
            anchors.bottomMargin = bottom_margin
        }

        anchors.fill: parent

        Item {
            id: titleBar

            Layout.fillWidth: true
            Layout.preferredHeight: 40

            RowLayout {
                id: titlebar_layout

                anchors.fill: parent
                spacing: 15

                Loader {
                    id: spacerItem

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Repeater {
                    model: ["qrc:/res/ico/MingcuteMinimizeLine.png", "qrc:/res/ico/FluentMaximize16Regular.png", "qrc:/res/ico/IconamoonClose.png"]

                    delegate: Image {
                        id: tool_button
                        required property var modelData
                        required property var index
                        //  sourceSize: Qt.size(25, 25)
                        Layout.preferredHeight: 18
                        Layout.preferredWidth: 18
                        antialiasing: true
                        mipmap: true
                        smooth: true
                        source: modelData

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                            }
                            onClicked: {
                                if (tool_button.index === 2) {
                                    register_window.destroy()
                                }
                            }
                        }
                    }
                }
                Loader {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 7
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 60
                anchors.rightMargin: 60

                Label {
                    color: Qt.color("#80D0C7")
                    font.bold: true
                    font.pixelSize: 20
                    text: "NACHT"
                }
                Loader {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Rectangle {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 65
                    border.color: Qt.color("#80D0C7")
                    border.width: 1
                    color: Qt.color("transparent")
                    radius: 18

                    Label {
                        anchors.centerIn: parent
                        text: "help"
                    }
                }
            }
        }
        StackLayout {
            id: stack_layout

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: 80
            Layout.rightMargin: 80

            Item {
                id: input_username

                function backAnimation() {
                    name_para_animation.restart()
                }

                Layout.fillHeight: true
                Layout.fillWidth: true

                ParallelAnimation {
                    id: name_para_animation

                    running: false

                    NumberAnimation {
                        duration: 500
                        from: 0
                        properties: "opacity"
                        target: input_username
                        to: 1
                    }
                    PropertyAnimation {
                        id: back_animation

                        duration: 1000
                        easing.type: Easing.InOutExpo
                        from: input_username.width + 80
                        properties: "x"
                        target: input_username
                        to: 0
                    }
                }
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 20

                    Loader {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 1
                    }
                    FileDialog {
                        id: select_userhead

                        fileMode: FileDialog.OpenFile

                        onAccepted: {
                            register_window.userHeadPath = currentFile.toString(
                                        ).substring(8)
                            userhead.source = "image://asyncLocal/" + register_window.userHeadPath
                        }
                    }
                    Image {
                        id: userhead

                        Layout.alignment: Qt.AlignHCenter
                        height: 80
                        width: 80
                        sourceSize: Qt.size(80, 80)
                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                select_userhead.open()
                            }
                        }
                    }
                    Text {
                        id: text_

                        Layout.alignment: Qt.AlignHCenter
                        font.bold: true
                        font.pixelSize: 18
                        text: "Registere started with your userName"
                    }
                    TextField {
                        id: username_field

                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: 60
                        Layout.preferredWidth: text_.contentWidth
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                        placeholderText: "Type Here"
                        placeholderTextColor: Qt.color("#d3d4e0")

                        background: Rectangle {
                            border.color: Qt.color("#d3d4e0")
                            border.width: 1
                            color: Qt.color("transparent")
                            radius: 35
                        }

                        Item {
                            id: warn_item

                            height: 30
                            visible: false
                            width: 120

                            anchors {
                                left: parent.right
                                leftMargin: 8
                                verticalCenter: parent.verticalCenter
                            }
                            Image {
                                id: warning_

                                antialiasing: true
                                height: 25
                                smooth: true
                                source: Qt.url("qrc:/res/ico/SiWarningFill.png")

                                //sourceSize: Qt.size(25, 25)
                                width: 25
                            }
                            Label {
                                font.pixelSize: 11
                                text: "用户名不能为空"

                                anchors {
                                    left: warning_.right
                                    leftMargin: 7
                                    verticalCenter: warning_.verticalCenter
                                }
                            }
                        }
                    }
                    NumberAnimation {
                        id: warn_animation

                        duration: 2000
                        from: 1
                        properties: "opacity"
                        target: warn_item
                        to: 0
                    }
                    Rectangle {
                        id: button_

                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 90
                        color: Qt.color("#80D0C7")
                        radius: 20

                        Label {
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: 13
                            text: "Get Start"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                if (username_field.text.length <= 0) {
                                    warn_item.visible = true
                                    warn_animation.restart()
                                    return
                                }
                                stack_layout.currentIndex = (stack_layout.currentIndex + 1)
                                let next_object = stack_layout.children[stack_layout.currentIndex]
                                next_object.startAnimation()
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                            }
                            onPressed: {
                                button_animation_.to = 0.9
                                button_animation_.restart()
                            }
                            onReleased: {
                                button_animation_.to = 1
                                button_animation_.restart()
                            }
                        }
                        NumberAnimation {
                            id: button_animation_

                            duration: 700
                            properties: "scale"
                            target: button_
                        }
                    }
                    Loader {
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 1
                    }
                }
            }
            Item {
                id: userinfo

                property string email_warn_text: "邮箱不能为空"
                property string phone_warn_text: '手机号不能为空给'

                function infoBackAnimation() {
                    userinfo.visible = true
                    para_animation.restart()
                }
                function startAnimation() {
                    info_animation.restart()
                }

                Layout.fillHeight: true
                Layout.fillWidth: true

                ParallelAnimation {
                    id: para_animation

                    running: false

                    PropertyAnimation {
                        id: info_back_animation

                        duration: 1000
                        easing.type: Easing.InOutExpo
                        from: userinfo.width + 80
                        properties: "x"
                        target: userinfo
                        to: 0
                    }
                    NumberAnimation {
                        duration: 500
                        from: 0
                        properties: "opacity"
                        target: userinfo
                        to: 1
                    }
                }
                RowLayout {
                    anchors.fill: parent

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 25

                            Loader {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                            RowLayout {
                                Image {
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
                                    source: Qt.url(
                                                "qrc:/res/ico/MaterialSymbolsArrowBackRounded.png")
                                    sourceSize: Qt.size(20, 20)
                                }
                                Label {
                                    Layout.alignment: Qt.AlignLeft
                                    font.bold: true
                                    font.pixelSize: 13
                                    text: "Back"

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true

                                        onClicked: {
                                            if (stack_layout.currentIndex !== 0) {
                                                stack_layout.currentIndex
                                                        = stack_layout.currentIndex - 1
                                                let object = stack_layout.children[stack_layout.currentIndex]
                                                object.backAnimation()
                                            }
                                        }
                                        onEntered: {
                                            cursorShape = Qt.PointingHandCursor
                                        }
                                    }
                                }
                            }
                            Label {
                                Layout.alignment: Qt.AlignLeft
                                font.bold: true
                                font.pixelSize: 18
                                text: "输入你的个人信息"
                            }
                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                Label {
                                    id: email_text

                                    text: "邮箱地址"

                                    anchors {
                                        left: parent.left
                                        top: parent.top
                                    }
                                }
                                TextField {
                                    id: email_input

                                    height: 40
                                    placeholderText: "example@qq.com"
                                    width: 200

                                    background: Rectangle {
                                        antialiasing: true
                                        border.color: Qt.rgba(0 / 255, 0 / 255,
                                                              0 / 255, 20 / 255)
                                        border.width: 1
                                        color: "transparent"
                                        radius: 7
                                    }

                                    onTextChanged: {
                                        if (!email_warin.visible) {
                                            return
                                        }
                                        email_warin.visible = false
                                    }

                                    anchors {
                                        left: email_text.left
                                        top: email_text.bottom
                                        topMargin: 13
                                    }
                                }
                                Item {
                                    id: email_warin

                                    height: 25
                                    visible: false
                                    width: 100

                                    anchors {
                                        left: email_input.right
                                        leftMargin: 8
                                        verticalCenter: email_input.verticalCenter
                                    }
                                    Image {
                                        id: email_warn_image

                                        anchors.verticalCenter: parent.verticalCenter
                                        antialiasing: true
                                        height: 20
                                        smooth: true
                                        source: Qt.url(
                                                    "qrc:/res/ico/SiWarningFill.png")
                                        //sourceSize: Qt.size(25, 25)
                                        width: 20
                                    }
                                    Label {
                                        font.pixelSize: 9
                                        text: userinfo.email_warn_text

                                        anchors {
                                            left: email_warn_image.right
                                            leftMargin: 7
                                            verticalCenter: email_warn_image.verticalCenter
                                        }
                                    }
                                }
                                Label {
                                    id: gender_text

                                    text: "性别"

                                    anchors {
                                        left: email_input.left
                                        top: email_input.bottom
                                        topMargin: 13
                                    }
                                }
                                CustomComboBox {
                                    id: gender_comboBox

                                    anchors {
                                        left: gender_text.left
                                        top: gender_text.bottom
                                        topMargin: 13
                                    }
                                }
                                Label {
                                    id: phone_text

                                    text: "手机号"

                                    anchors {
                                        left: gender_text.left
                                        top: gender_comboBox.bottom
                                        topMargin: 14
                                    }
                                }
                                TextField {
                                    id: phone_input

                                    height: 40
                                    maximumLength: 11
                                    placeholderText: "Phone"
                                    width: 200

                                    background: Rectangle {
                                        antialiasing: true
                                        border.color: Qt.rgba(0 / 255, 0 / 255,
                                                              0 / 255, 20 / 255)
                                        border.width: 1
                                        color: "transparent"
                                        radius: 7
                                    }

                                    onTextChanged: {
                                        if (!phone_warn.visible) {
                                            return
                                        }
                                        phone_warn.visible = false
                                    }

                                    anchors {
                                        left: phone_text.left
                                        top: phone_text.bottom
                                        topMargin: 13
                                    }
                                }
                                Item {
                                    id: phone_warn

                                    height: 25
                                    visible: false
                                    width: 100

                                    anchors {
                                        left: phone_input.right
                                        leftMargin: 8
                                        verticalCenter: phone_input.verticalCenter
                                    }
                                    Image {
                                        id: phone_warn_image

                                        anchors.verticalCenter: parent.verticalCenter
                                        antialiasing: true
                                        height: 20
                                        smooth: true
                                        source: Qt.url(
                                                    "qrc:/res/ico/SiWarningFill.png")
                                        width: 20
                                    }
                                    Label {
                                        font.pixelSize: 9
                                        text: userinfo.phone_warn_text

                                        anchors {
                                            left: phone_warn_image.right
                                            leftMargin: 7
                                            verticalCenter: phone_warn_image.verticalCenter
                                        }
                                    }
                                }
                            }
                            Loader {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 230
                            }
                        }
                    }
                    Loader {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                    Image {
                        Layout.preferredHeight: 300
                        Layout.preferredWidth: 300
                        source: Qt.url("qrc:/res/ico/register_image_1.png")
                        sourceSize: Qt.size(300, 300)
                    }
                }
                PropertyAnimation {
                    id: info_animation

                    duration: 500
                    from: 0
                    properties: "scale,opacity"
                    running: false
                    target: userinfo
                    to: 1
                }
            }

            //password
            Item {
                id: password_item

                function startAnimation_3() {
                    password_animation.restart()
                }

                Layout.fillHeight: true
                Layout.fillWidth: true

                NumberAnimation {
                    id: password_animation

                    duration: 1000
                    from: 0
                    properties: "opacity"
                    target: password_item
                    to: 1
                }
                Item {
                    anchors.centerIn: parent
                    height: 300
                    width: 200

                    ColumnLayout {
                        anchors.fill: parent

                        RowLayout {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            spacing: 8

                            Image {
                                Layout.preferredHeight: 20
                                Layout.preferredWidth: 20
                                antialiasing: true
                                source: Qt.url(
                                            "qrc:/res/ico/MaterialSymbolsArrowBackRounded.png")
                            }
                            Label {
                                font.bold: true
                                font.pixelSize: 13
                                text: "Back"

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    onClicked: {
                                        if (stack_layout.currentIndex > 0) {
                                            stack_layout.currentIndex
                                                    = stack_layout.currentIndex - 1
                                            let object = stack_layout.children[stack_layout.currentIndex]
                                            object.infoBackAnimation()
                                        }
                                    }
                                    onEntered: {
                                        cursorShape = Qt.PointingHandCursor
                                    }
                                }
                            }
                        }
                        Loader {
                            Layout.preferredHeight: 20
                            Layout.preferredWidth: 1
                        }
                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            Label {
                                id: password_text

                                text: "密码"
                            }
                            TextField {
                                id: password_input

                                height: 40
                                placeholderText: "Password"
                                width: 240

                                background: Rectangle {
                                    antialiasing: true
                                    border.color: Qt.rgba(0 / 255, 0 / 255,
                                                          0 / 255, 40 / 255)
                                    border.width: 1
                                    color: "transparent"
                                    radius: 7
                                }

                                onTextChanged: {
                                    password_warn_item.visible = false
                                }

                                anchors {
                                    left: password_text.left
                                    top: password_text.bottom
                                    topMargin: 13
                                }
                            }
                            Item {
                                id: password_warn_item

                                visible: false

                                anchors {
                                    left: password_input.right
                                    leftMargin: 10
                                    verticalCenter: password_input.verticalCenter
                                }
                                Image {
                                    id: password_warn_image

                                    anchors.verticalCenter: parent.verticalCenter
                                    antialiasing: true
                                    height: 20
                                    smooth: true
                                    source: Qt.url(
                                                "qrc:/res/ico/SiWarningFill.png")
                                    width: 20
                                }
                                Label {
                                    font.pixelSize: 9
                                    text: "请设置您的密码"

                                    anchors {
                                        left: password_warn_image.right
                                        leftMargin: 5
                                        verticalCenter: password_warn_image.verticalCenter
                                    }
                                }
                            }
                            TextField {
                                id: captcha_input

                                height: 40
                                placeholderText: "验证码"
                                width: 120

                                background: Rectangle {
                                    antialiasing: true
                                    border.color: Qt.rgba(0 / 255, 0 / 255,
                                                          0 / 255, 40 / 255)
                                    border.width: 1
                                    color: "transparent"
                                    radius: 7
                                }

                                onTextChanged: {
                                    captcha_warn_item.visible = false
                                }

                                anchors {
                                    left: password_input.left
                                    top: password_input.bottom
                                    topMargin: 13
                                }
                            }
                            Captcha {
                                id: captcha

                                height: 40
                                width: 120

                                anchors {
                                    left: captcha_input.right
                                    verticalCenter: captcha_input.verticalCenter
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    onClicked: {
                                        captcha.rebuild()
                                    }
                                    onEntered: {
                                        cursorShape = Qt.PointingHandCursor
                                    }
                                }
                            }
                            Item {
                                id: captcha_warn_item

                                visible: false

                                anchors {
                                    left: captcha.right
                                    leftMargin: 5
                                    verticalCenter: captcha.verticalCenter
                                }
                                Image {
                                    id: captcha_warn_image

                                    anchors.verticalCenter: parent.verticalCenter
                                    antialiasing: true
                                    height: 20
                                    smooth: true
                                    source: Qt.url(
                                                "qrc:/res/ico/SiWarningFill.png")
                                    width: 20
                                }
                                Label {
                                    font.pixelSize: 9
                                    text: "验证码错误"

                                    anchors {
                                        left: captcha_warn_image.right
                                        verticalCenter: captcha_warn_image.verticalCenter
                                    }
                                }
                            }
                            Rectangle {
                                color: Qt.color("#80D0C7")
                                height: 35
                                radius: 20
                                width: 85

                                Label {
                                    anchors.centerIn: parent
                                    color: "white"
                                    font.bold: true
                                    text: "完成"
                                }
                                anchors {
                                    horizontalCenter: parent.horizontalCenter
                                    top: captcha_input.bottom
                                    topMargin: 20
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    onClicked: {
                                        if (password_input.text.length <= 0) {
                                            password_warn_item.visible = true
                                            return
                                        }
                                        let ret = captcha.captchaValue(
                                                captcha_input.text)
                                        if (!ret) {
                                            captcha_warn_item.visible = true
                                            return
                                        }

                                        let account = Math.floor(
                                                Math.random(
                                                    ) * (999999999 - 111111111) + 111111111)
                                        compelted_.useraccount = account
                                        stack_layout.currentIndex = stack_layout.currentIndex + 1
                                        let info = user_info.createObject(
                                                register_window)
                                        info.useraccount = account
                                        info.username = username_field.text
                                        info.userpassword = password_input.text
                                        info.gender = gender_comboBox.currentValue
                                        info.email = email_input.text
                                        info.phone = phone_input.text
                                        info.userhead = register_window.userHeadPath
                                        Client.registerUser(info)
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item {
                id: compelted_

                property string useraccount

                Layout.fillHeight: true
                Layout.fillWidth: true

                Label {
                    anchors.centerIn: parent
                    font.pixelSize: 25
                    text: "你的账号为：<font color =  orange>" + compelted_.useraccount + "</font>"
                    textFormat: Text.RichText
                }
            }
        }
        Rectangle {
            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
            Layout.bottomMargin: 15
            Layout.preferredHeight: 35
            Layout.preferredWidth: 90
            Layout.rightMargin: 80
            color: Qt.color("#80D0C7")
            radius: 20
            visible: stack_layout.currentIndex !== 0
                     && stack_layout.currentIndex !== 2

            Label {
                anchors.centerIn: parent
                color: "white"
                text: "Next"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    if (email_input.text.length <= 0) {
                        userinfo.email_warn_text = "邮箱地址不能为空"
                        email_warin.visible = true
                    }
                    if (phone_input.text.length <= 0) {
                        userinfo.phone_warn_text = "手机号不能为空"
                        phone_warn.visible = true
                    }

                    let email_text = email_input.text.toString()
                    let eamil_correct = email_text.includes("@qq.com")
                        && email_input.text.length >= 1
                    let phone_correct = phone_input.text.length < 11
                        && phone_input.text.length >= 1
                    if (!eamil_correct) {
                        userinfo.email_warn_text = "邮箱格式不正确"
                        email_warin.visible = true
                    }

                    if (phone_correct) {
                        userinfo.phone_warn_text = "请输入正确的手机号"
                        phone_warn.visible = true
                    }

                    if (!eamil_correct || phone_correct)
                        return
                    if (email_input.text.length <= 0
                            || phone_input.text.length <= 0)
                        return

                    if (stack_layout.currentIndex !== stack_layout.children.length - 1) {
                        stack_layout.currentIndex = stack_layout.currentIndex + 1
                        userinfo.visible = false
                        let object = stack_layout.children[stack_layout.currentIndex]
                        object.startAnimation_3()
                    }
                }
                onEntered: {
                    cursorShape = Qt.PointingHandCursor
                }
            }
        }
    }

    Component {
        id: user_info

        UserInfo {}
    }
}
