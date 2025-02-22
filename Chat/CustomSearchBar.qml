import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

Rectangle {
    property alias placeholder_text: field.placeholderText
    TextField {
        id: field
        width: parent.width - 5
        height: parent.height - 2
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle {
            color: Qt.color("transparent")
        }
    }
}
