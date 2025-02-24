WorkerScript.onMessage = function func(message) {
    var type = message.type
    var json = message.value
    var model = message.model

    if (type === "search") {

        model.clear()

        model.append({
                         "title": "Account",
                         "value": json.account
                     })
        model.append({
                         "title": "Name",
                         "value": json.name
                     })
        model.append({
                         "title": "Gender",
                         "value": json.gender
                     })
        model.append({
                         "title": "Phone",
                         "value": json.phone
                     })
        model.append({
                         "title": "Email",
                         "value": json.email
                     })

        model.sync()
    } else if (type === "request") {
        model.append({
                         "jsonData": json
                     })
        model.sync()
    } else if (type === "updateFriend") {
        model.append({
                         "friendData": json,
                         "type": "user"
                     })
        model.sync()
    } else if (type === "friendData") {
        for (var i = 0; i < json.length; i++) {
            console.log(json[i].type)
            model.append({
                             "friendData": json[i]
                         })
        }
        model.sync()
    } else if (type === "increateMessage") {
        var usermessage = message.message
        var item_type = message.itemType
        for (var j = 0; j < model.count; j++) {
            let _account = model.get(j).messageData.account
            if (json.account === _account) {
                return
            }
        }
        json.message = usermessage
        model.append({
                         "messageData": json,
                         "unreadCount": 0,
                         "itemType": item_type
                     })
        model.sync()
        WorkerScript.sendMessage({
                                     "type": "increateMessageFinished"
                                 })
    } else if (type === "sendMessage") {
        increaseMessageItemForMyself(message)
    } else if (type === "receivedMessage") {
        receivedMessage(message)
    } else if (type === "addFriendData") {
        model.append({
                         "friendData": json
                         // "type": "user"
                     })
        model.sync()
    } else if (type === "updateMessage") {
        updateMessage(message)
    } else if (type === "clearUnreadCount") {
        model.set(json, {
                      "unreadCount": 0
                  })

        model.sync()

        var count_ = 0
        for (var h = 0; h < model.count; h++) {
            var _count = model.get(h).unreadCount
            count_ += _count
        }

        WorkerScript.sendMessage({
                                     "type": "UnreadCount",
                                     "count": count_
                                 })
    } else if (type === "countUnreadCount") {
        countUnreadCount(message)
    } else if (type === "receivedFile") {
        receivedFile(message)
    } else if (type === "receivedGroupInvited") {
        model.append({
                         "friendData": json
                         //"type": "Group"
                     })
        model.sync()
    }
}

function increaseMessageItemForMyself(message) {
    var model = message.model
    var message_type = message.messageType
    var account = message.value

    if (message_type === "Text") {
        var usermessage = message.message
        model.append({
                         "stateType": "send",
                         "account": account,
                         "message": usermessage,
                         "messageType": message_type,
                         "fileInfo": {}
                     })
        model.sync()
        WorkerScript.sendMessage({
                                     "type": "sendFinished"
                                 })
    } else if (message_type === "File") {
        var file_data = message.fileInfo
        model.append({
                         "stateType": "send",
                         "account": account,
                         "messageType": message_type,
                         "fileInfo": file_data,
                         "message": ""
                     })
        model.sync()
        WorkerScript.sendMessage({
                                     "type": "sendFinished",
                                     "messageType": "file",
                                     "fileName": file_data.fileName
                                 })
    } else if (message_type === "Picture") {
        var picture_data = message.fileInfo
        model.append({
                         "stateType": "send",
                         "account": account,
                         "messageType": message_type,
                         "fileInfo": picture_data,
                         "message": ""
                     })
        model.sync()
        WorkerScript.sendMessage({
                                     "type": "sendFinished",
                                     "messageType": "Picture",
                                     "fileName": picture_data.fileName
                                 })
    }
}

function receivedMessage(message) {
    var model = message.model
    var jsonData = message.value
    var usermessage = message.message
    var message_type = message.messageType
    model.append({
                     "stateType": "recv",
                     "account": jsonData.account,
                     "message": usermessage,
                     "fileInfo": {},
                     "messageType": message_type
                 })
    model.sync()
    WorkerScript.sendMessage({
                                 "type": "recvFinished"
                             })
}

function increateUnreadMessageCount(message) {
    var account = message.value
    var model = message.model
    var index = message.index
    var count = model.get(index).unreadCount
    count++
    model.set(index, {
                  "unreadCount": count++
              })
    model.sync()
}

function updateMessage(message) {
    var model = message.model
    var usermessage = message.message
    var viewIndex = message.viewIndex
    var account = message.value

    if (viewIndex === -1) {
        for (var i = 0; i < model.count; i++) {
            let jsonData = model.get(i).messageData
            if (jsonData.account === account) {
                jsonData.message = usermessage
                model.set(i, {
                              "messageData": jsonData
                          })
            }
        }
        model.sync()
        return
    }

    for (var j = 0; j < model.count; j++) {
        var data = model.get(j).messageData
        if (account === data.account) {
            data.message = usermessage
            if (viewIndex !== j) {
                let unread_account = model.get(j).unreadCount
                unread_account++

                model.set(j, {
                              "messageData": data,
                              "unreadCount": unread_account
                          })
                WorkerScript.sendMessage({
                                             "type": "haveUnreadMessage"
                                         })
            } else {
                let unread_account = model.get(j).unreadCount
                model.set(j, {
                              "messageData": data
                          })
            }
        }
    }
    model.sync()
}
function countUnreadCount(message) {
    var model = message.model
    var count_ = 0
    for (var i = 0; i < model.count; i++) {
        var _count = model.get(i).unreadCount
        count_ += _count
    }

    WorkerScript.sendMessage({
                                 "type": "UnreadCount",
                                 "count": count_
                             })
}

function receivedFile(message) {
    var model = message.model
    var jsonData = message.value
    var fileInfo = message.fileInfo
    var message_type = message.messageType
    model.append({
                     "stateType": "recv",
                     "account": jsonData.account,
                     "fileInfo": fileInfo,
                     "messageType": message_type,
                     "message": ""
                 })
    model.sync()
    WorkerScript.sendMessage({
                                 "type": "recvFinished",
                                 "messageType": message_type,
                                 "fileName": fileInfo.fileName
                             })
}
