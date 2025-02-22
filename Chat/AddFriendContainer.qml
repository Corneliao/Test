import QtQuick
import com.framelesswindow
import QtQuick.Layouts
import QtQuick.Controls
import com.client

FramelessWindow {

    id: add_freind_container
    width: 640
    height: 480
    modality: Qt.ApplicationModal

    Component.onCompleted: {
        add_freind_container.moveCenter()
        add_freind_container.setWindowTitleBar(title_bar)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        function adjustWindowMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin
            anchors.topMargin = top_margin
            anchors.rightMargin = right_margin
            anchors.bottomMargin = bottom_margin
        }

        Item {
            id: title_bar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.alignment: Qt.AlignTop

            Label {
                text: "好友搜索"
                anchors.centerIn: parent
            }

            Image {
                source: Qt.url("qrc:/res/ico/IconamoonClose.png")
                width: 25
                height: 25
                anchors {
                    right: parent.right
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                antialiasing: true
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }

                    onClicked: {
                        add_freind_container.destroy()
                    }
                }
            }
        }
        Rectangle {

            Layout.leftMargin: 25
            Layout.rightMargin: 25
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)
            radius: 10
            TextField {
                id: searchBar
                placeholderText: "输入搜索关键词"
                width: parent.width - 5
                height: parent.height - 2
                background: Rectangle {
                    color: "transparent"
                }
                anchors.verticalCenter: parent.verticalCenter
                Keys.onReturnPressed: {
                    Client.searchUser(text)
                }
            }
        }
        SearchResult {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 15
        }
    }
}
