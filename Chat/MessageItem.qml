import QtQuick
import QtQuick.Layouts
import com.client

Item {

    id: root

    property string state_type
    property string message_type
    property string usermessage
    property string useraccount
    property var fileInfoData
    required property int index
    height: message_type === "Text" ? 40 : 80

    Loader {
        id: loader
        asynchronous: true
        source: message_type === "Text" ? Qt.url(
                                              "TextMessageItem.qml") : (message_type === "File" ? Qt.url("FileMessageItem.qml") : Qt.url("PictureMessageItem.qml"))
        anchors.fill: parent
        onLoaded: {

            item.state_type = root.state_type
            item.useraccount = root.useraccount
            if (root.message_type === "Text") {
                item.usermessage = root.usermessage
            } else if (root.message_type === "File") {
                item.fileInfoData = root.fileInfoData
                item.index = root.index
            } else if (root.message_type === "Picture") {
                var size = Qt.size(0, 0)
                if (root.state_type === "send") {
                    size = Client.getPictureSize(fileInfoData.filePath)
                } else if (root.state_type === "recv") {
                    size = Client.getPictureSize(fileInfoData.fileName)
                }

                if (size.width === size.height) {
                    root.height = 120
                    item.pictureSize = Qt.size(120, 120)
                }
                if (size.width > size.height) {
                    root.height = 180
                    item.pictureSize = Qt.size(320, 180)
                }

                item.fileInfoData = root.fileInfoData
                item.index = root.index
            }
        }
    }
}
