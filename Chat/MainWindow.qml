import QtQuick
import com.framelesswindow
import QtQuick.Layouts
import com.client

FramelessWindow {
    width: 1050
    height: 680
    color: Qt.color("#62252c")
    id: mainWindow
    property string userAccount
    property var noticeComponent: notice
    property var featureBarComponent: featurbar_
    Component.onCompleted: {
        mainWindow.moveCenter()
        mainWindow.setWindowTitleBar(title_bar)
    }

    NotificationContainer {
        id: notice
        x: mainWindow.width - width - 20
        y: 15
    }
    RowLayout {
        function adjustWindowMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin
            anchors.topMargin = top_margin
            anchors.rightMargin = right_margin
            anchors.bottomMargin = bottom_margin
        }

        anchors.fill: parent

        spacing: 20
        NAFeatureBar {
            id: featurbar_
            useraccount: mainWindow.userAccount
            Layout.preferredWidth: 60
            Layout.leftMargin: 15
            Layout.preferredHeight: parent.height - 40
            onFeaturBarIndexChanged: index => {
                                         stacklayout.currentIndex = index
                                     }
        }

        ColumnLayout {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width
            NATitleBar {
                id: title_bar
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 30
                Layout.fillWidth: true
            }
            StackLayout {
                id: stacklayout
                Layout.fillHeight: true
                Layout.fillWidth: true
                MessageContainer {
                    id: messageContainer
                    Layout.preferredHeight: parent.height
                    Layout.preferredWidth: parent.width
                }
                MyFriendContainer {
                    Layout.preferredHeight: parent.height
                    Layout.preferredWidth: parent.width
                    onCreateMessageSignal: data => {
                                               messageContainer.hostoryMessageView.createMessage(
                                                   data, "")
                                               stacklayout.currentIndex = 0
                                               featurbar_.setCurrentIndex(0)
                                               messageContainer.createChatWindow(
                                                   data)
                                           }
                }
            }
        }
    }
}
