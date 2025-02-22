import QtQuick
import QtQml

QtObject {

    id: root

    property string userAccount

    property var loader: Loader {
        source: "MainWindow.qml"
        active: false
        onLoaded: {
            item.userAccount = root.userAccount
        }
    }

    property var login: Login {
        onLoginSignal: account => {
                           root.userAccount = account
                           root.login.destroy()
                           timer.start()
                       }
    }

    property var timer: Timer {
        id: timer
        running: false
        repeat: false
        onTriggered: {
            root.loader.active = true
            this.destroy()
        }
    }
}
