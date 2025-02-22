import QtQuick
import QtQuick.Layouts

Item {
    id: text_message
    property string useraccount
    property string state_type: "send"
    property string usermessage
    width: parent.width
    height: parent.height

    state: state_type

    states: [
        State {
            name: "send"
            PropertyChanges {
                target: item_layout
                layoutDirection: Qt.LeftToRight
            }
        },
        State {
            name: "recv"
            PropertyChanges {
                target: item_layout
                layoutDirection: Qt.RightToLeft
            }
        }
    ]

    RowLayout {
        id: item_layout
        anchors.fill: parent

        Loader {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.preferredHeight: parent.height - 5
            Layout.preferredWidth: user_message.width + 20
            //color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 80 / 255)
            color: Qt.color("#c1696c")
            radius: 10
            Text {
                id: user_message
                text: text_message.usermessage
                anchors.centerIn: parent
                color: Qt.color("white")
            }
        }

        Image {
            id: user_head
            source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                           + text_message.useraccount + ".jpg")
            Layout.preferredHeight: 40
            Layout.preferredWidth: 40
            sourceSize: Qt.size(40, 40)
        }
    }
}
