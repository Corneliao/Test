import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.client
import QtQml

Rectangle {
    id: file_message_item
    property string state_type
    property string useraccount
    property var fileInfoData
    property int index

    color: Qt.color("transparent")
    width: parent.width
    height: file_layout.height
    property real progress: 0.0

    state: state_type

    Connections {
        target: Client

        function onUploadFileProgress(pos, index) {
            if (index === file_message_item.index) {
                progress_bar.value = pos
            }
        }
        function onDownloadFileProgress(pos, index) {
            if (index === file_message_item.index) {
                progress_bar.value = pos
            }
        }
    }

    states: [
        State {
            name: "send"
            PropertyChanges {
                target: file_layout
                layoutDirection: Qt.LeftToRight
            }
        },
        State {
            name: "recv"
            PropertyChanges {
                target: file_layout
                layoutDirection: Qt.RightToLeft
            }
        }
    ]

    RowLayout {
        id: file_layout
        width: parent.width
        spacing: 15
        Loader {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            id: contentParentItem
            Layout.preferredWidth: contentRect.width + 20
            Layout.preferredHeight: contentRect.height + 25
            // color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 80 / 255)
            color: Qt.color("#c1696c")
            radius: 10

            Rectangle {
                id: contentRect
                width: fileName.width + 120
                height: content_layout.height
                color: Qt.color("transparent")
                anchors.centerIn: parent

                RowLayout {
                    id: content_layout

                    Image {
                        source: Qt.url("qrc:/res/ico/exe.png")
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                    }

                    ColumnLayout {
                        Label {
                            id: fileName
                            text: file_message_item.fileInfoData.fileName
                            font.pixelSize: 12
                            color: Qt.color("white")
                        }
                        Label {
                            text: (file_message_item.fileInfoData.fileSize / 1024 / 1024).toFixed(
                                      2).toString() + "M"
                            font.pixelSize: 12
                            color: Qt.color("white")
                        }
                        CustomProgressBar {
                            id: progress_bar
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: 5
                            value: file_message_item.progress
                        }
                    }
                }
                Component.onCompleted: {
                    if (contentParentItem.height > 40) {
                        head.Layout.alignment = Qt.AlignTop
                        contentParentItem.Layout.alignment = Qt.AlignTop
                    }
                }
            }
        }
        Image {
            id: head
            Layout.preferredHeight: 40
            Layout.preferredWidth: 40
            source: Qt.url("image://async/http://127.0.0.1:9005/userhead/"
                           + file_message_item.useraccount + ".jpg")
            sourceSize: Qt.size(40, 40)
        }
    }
}
