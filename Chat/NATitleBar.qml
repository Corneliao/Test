import QtQuick
import QtQuick.Layouts

Item {
    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 15
        anchors.rightMargin: 15
        Image {
            source: Qt.url("qrc:/res/ico/MingcuteMinimizeLine.png")
            Layout.preferredHeight: 20
            Layout.preferredWidth: 20
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    mainWindow.showWindowState("min")
                }
                onEntered: {
                    cursorShape = Qt.PointingHandCursor
                }
            }
        }
        Image {
            source: Qt.url("qrc:/res/ico/FluentMaximize16Regular.png")
            Layout.preferredHeight: 20
            Layout.preferredWidth: 20
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    mainWindow.showWindowState("max")
                }
                onEntered: {
                    cursorShape = Qt.PointingHandCursor
                }
            }
        }
        Image {
            source: Qt.url("qrc:/res/ico/IconamoonClose.png")
            Layout.preferredHeight: 20
            Layout.preferredWidth: 20
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    mainWindow.close()
                }
                onEntered: {
                    cursorShape = Qt.PointingHandCursor
                }
            }
        }
    }
}
