import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.global

Rectangle {
    id: featureBar
    signal featurBarIndexChanged(int index)
    property int featureBarCurrentIndex: feature_list_view.currentIndex

    color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 80 / 255)
    radius: 20
    property string useraccount

    function setCurrentIndex(index) {
        feature_list_view.currentIndex = index
    }

    ColumnLayout {
        anchors.fill: parent

        Image {
            source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                           + featureBar.useraccount + ".jpg")
            Layout.preferredHeight: 45
            Layout.preferredWidth: 45
            smooth: true
            antialiasing: true
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            Layout.topMargin: 13
            sourceSize: Qt.size(45, 45)
        }
        spacing: 15

        ListView {
            id: feature_list_view
            spacing: 15
            model: ico_model
            Layout.fillHeight: true
            Layout.fillWidth: true
            delegate: Rectangle {
                id: ico_
                required property string selected_ico_path
                required property string ico_path
                required property int index
                required property string type
                width: ListView.view.width
                height: 40
                color: Qt.color("transparent")

                Rectangle {
                    id: bubble_unread
                    visible: ico_.type === "message" ? (Global.unreadMessageCount === "0" ? false : true) : (Global.newFriendRequestCount === "0" ? false : true)

                    width: 10
                    height: 10
                    radius: 10 / 2
                    color: Qt.color("#b14347")
                    anchors {
                        top: parent.top
                        right: parent.right
                        rightMargin: 10
                    }
                }

                Image {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    source: ico_.ListView.isCurrentItem ? ico_.selected_ico_path : ico_.ico_path
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        ico_.ListView.view.currentIndex = ico_.index
                        featureBar.featurBarIndexChanged(ico_.index)
                    }
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                    }
                }
            }
            highlight: Item {
                Rectangle {
                    width: 3
                    height: parent.width - 40
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    color: Qt.color("#b14347")
                    radius: 3 / 2
                }
            }
        }
    }

    ListModel {
        id: ico_model
        ListElement {
            selected_ico_path: "qrc:/res/ico/MingcuteMessage1Line_sel.png"
            ico_path: "qrc:/res/ico/MingcuteMessage1Line.png"
            type: "message"
        }
        ListElement {
            selected_ico_path: "qrc:/res/ico/friend_sel.png"
            ico_path: "qrc:/res/ico/friend.png"
            type: "friend"
        }
    }
}
