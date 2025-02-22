import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    width: 140
    height: 120
    padding: 2
    spacing: 2
    Material.roundedScale: Material.MediumScale
    Material.background: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255)

    MenuItem {
        contentItem: Item {
            id: add_friend
            Label {
                text: "添加好友"
                anchors.centerIn: parent
                font.pixelSize: 13
                color: Qt.color("white")
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                add_popup.close()
                freindListView.createAddFriendWindow()
            }
        }
    }

    MenuItem {
        contentItem: Item {
            //radius: 8
            id: create_group
            Label {
                text: "创建群聊"
                anchors.centerIn: parent
                font.pixelSize: 13
                color: Qt.color("white")
            }
        }
    }
}
