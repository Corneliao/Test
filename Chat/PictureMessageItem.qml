import QtQuick
import QtQuick.Layouts
import com.client
import QtQml

Item {
    id: picture_message_item
    property string state_type
    property string useraccount
    property var fileInfoData
    property int index
    property var pictureSize
    width: parent.width
    height: parent.height
    property string path: "null"
    Connections {
        target: Client
        function onDownloadFinishedForPicture(fileName, index) {
            if (index === picture_message_item.index) {
                path = fileName
            }
        }

        function onUploadFileForPicture(fileName, index) {
            if (index === picture_message_item.index) {
                path = fileName
            }
        }
    }

    state: state_type

    states: [
        State {
            name: "send"
            PropertyChanges {
                target: picture_layout
                layoutDirection: Qt.LeftToRight
            }
        },
        State {
            name: "recv"
            PropertyChanges {
                target: picture_layout
                layoutDirection: Qt.RightToLeft
            }
        }
    ]

    RowLayout {
        id: picture_layout
        width: parent.width
        spacing: 15
        Loader {
            Layout.fillWidth: true
        }

        Image {
            Layout.preferredHeight: pictureSize.height
            Layout.preferredWidth: pictureSize.width
            source: path === "null" ? "" : Qt.url("image://image_cr/" + path)
            fillMode: Image.PreserveAspectCrop
            sourceSize: picture_message_item.pictureSize
            clip: true
        }

        Image {
            id: head
            Layout.preferredHeight: 40
            Layout.preferredWidth: 40
            source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                           + picture_message_item.useraccount + ".jpg")
            sourceSize: Qt.size(40, 40)
            Layout.alignment: Qt.AlignTop
        }
    }
}
