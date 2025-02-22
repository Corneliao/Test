pragma Singleton

import QtQuick

QtObject {
    property var messageModel
    property var myJsonData
    property string unreadMessageCount: "0"
    property string newFriendRequestCount: "0"
}
