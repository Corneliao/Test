import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

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
            Rectangle {

            }
        }
    }
}
