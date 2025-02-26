import QtQuick
import QtQuick.Controls.Material
import com.client

Menu {
    width: 100
    height: 70
    modal: true
    property string account
    MenuItem {
        text: "删除好友"
        onTriggered: {

            Client.deleteFriend(account)
        }
    }
}
