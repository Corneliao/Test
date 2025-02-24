#include "clientwork.h"
#include "miniomanager.h"
#include "globalproperties.h"
ClientWork::ClientWork(const qintptr &socketDescript, QObject *parent) {
    this->m_socketDescript = socketDescript;
}

ClientWork::~ClientWork() {
    if (this->sql_thread) {
        this->sql_thread->exit();
        this->sql_thread->wait();
    }
}

void ClientWork::init() {
    this->m_socket = new QTcpSocket(this);
    this->m_socket->setSocketDescriptor(this->m_socketDescript);
    connect(this->m_socket, &QTcpSocket::disconnected, this, &ClientWork::disconnected, Qt::DirectConnection);
    connect(this->m_socket, &QTcpSocket::readyRead, this, &ClientWork::ReadClientMessage, Qt::DirectConnection);

    // 创建数据库线程
    this->sql_thread = new QThread;
    this->database_manager = new UserDatabaseManager;
    this->database_manager->moveToThread(this->sql_thread);
    connect(this->sql_thread, &QThread::started, this->database_manager, &UserDatabaseManager::init, Qt::DirectConnection);
    connect(this->sql_thread, &QThread::finished, this->database_manager, &UserDatabaseManager::deleteLater, Qt::DirectConnection);
    connect(this->sql_thread, &QThread::finished, this->sql_thread, &QThread::deleteLater, Qt::DirectConnection);
    connect(this->database_manager, &UserDatabaseManager::loginSignal, this, &ClientWork::loginState, Qt::QueuedConnection);
    connect(this->database_manager, &UserDatabaseManager::userInfoSignal, this, &ClientWork::selectUserInfo, Qt::QueuedConnection);
    connect(this->database_manager, &UserDatabaseManager::friendDataSignal, this, &ClientWork::selectUserFriends, Qt::QueuedConnection);
    connect(
            this->database_manager,
            &UserDatabaseManager::createGroupChatSignal,
            this,
            [=](const bool state, const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo) {
                if (state) {
                    emit this->createGroupChatSignal(senderData, members, groupInfo);
                } else {
                    QJsonObject object;
                    object.insert("type", "createGroupFailed");
                    object.insert("state", state);
                    QJsonDocument doc(object);
                    QByteArray data = doc.toJson();
                    this->m_socket->write(data);
                    this->m_socket->flush();
                }
            },
            Qt::QueuedConnection);
    this->sql_thread->start();
}

void ClientWork::receivedFriendRequest(const QString &sender) {
    QMetaObject::invokeMethod(this->database_manager, "selectUserInfo", Qt::QueuedConnection, Q_ARG(QString, "receivedRequest"), Q_ARG(QString, sender));
}

void ClientWork::updateFriendList(const QJsonObject &json) {
    QJsonObject object;
    object.insert("type", "updateFriendList");
    object.insert("userData", json);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::ReadClientMessage() {
    QByteArray data = this->m_socket->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (doc.isObject() && error.error == QJsonParseError::NoError) {
        QJsonObject object = doc.object();
        if (QString type = object["type"].toString(); type == "login") {
            QString account = object["account"].toString();
            QString password = object["password"].toString();
            userInfo info;
            info.user_account = account;
            info.user_password = password;

            QMetaObject::invokeMethod(this->database_manager, "login", Qt::QueuedConnection, Q_ARG(QVariant, QVariant::fromValue(info)));
        } else if (type == "register") {
            QString useraccount = object["useraccount"].toString();
            QString username = object["username"].toString();
            QString password = object["password"].toString();
            QString userhead = object["userhead"].toString();
            QString email = object["email"].toString();
            QString phone = object["phone"].toString();
            QString gender = object["gender"].toString();
            userInfo info{useraccount, username, password, userhead, gender, email, phone};
            QMetaObject::invokeMethod(this->database_manager, "registerUser", Qt::QueuedConnection, Q_ARG(QVariant, QVariant::fromValue(info)));

            MinioManager *minio = new MinioManager("upload", userhead, useraccount);
            QThreadPool::globalInstance()->start(minio);
        } else if (type == "searchFriend") {
            QString search_account = object["account"].toString();
            QMetaObject::invokeMethod(this->database_manager, "selectUserInfo", Qt::QueuedConnection, Q_ARG(QString, "searchResult"), Q_ARG(QString, search_account));
        } else if (type == "addFriend") {
            QString sender = object["sender"].toString();
            QString receiver = object["receiver"].toString();
            emit this->addFriendSignal(sender, receiver);
        } else if (type == "agreeRequest") {
            QString sender = object["sender"].toString();
            QJsonObject userData = object["userData"].toObject();
            emit this->updateFriendListSignal(sender, userData);
            QMetaObject::invokeMethod(this->database_manager, "agreeFriendRequest", Qt::QueuedConnection, Q_ARG(QString, sender), Q_ARG(QString, userData["account"].toString()));
        } else if (type == "selectFriend") {
            QString account = object["account"].toString();
            QMetaObject::invokeMethod(this->database_manager, "selectUserFriend", Qt::QueuedConnection, Q_ARG(QString, account));
        } else if (type == "sendMessage") {
            QJsonObject sender_ = object["senderData"].toObject();
            QString receiver_ = object["receiver"].toString();
            QString message = object["message"].toString();
            QString messageType = object["messageType"].toString();
            emit this->sendMessageSignal(sender_, receiver_, message, messageType);
        } else if (type == "sendFile") {
            QJsonObject sender_ = object["senderData"].toObject();
            QString receiver_ = object["receiver"].toString();
            QString fileName = object["fileName"].toString();
            qint64 fileSize = object["fileSize"].toInt();
            QString messageType = object["messageType"].toString();

            QJsonObject object;
            object.insert("fileName", fileName);
            object.insert("fileSize", fileSize);
            emit this->sendFileSignal(sender_, receiver_, object, messageType);
        } else if (type == "createGroupChat") {
            QJsonObject senderData = object["senderData"].toObject();
            QJsonArray members = object["members"].toArray();
            QJsonObject groupInfo = object["groupInfo"].toObject();
            QMetaObject::invokeMethod(this->database_manager, "createGroupChat", Qt::QueuedConnection, Q_ARG(QJsonObject, senderData), Q_ARG(QJsonArray, members), Q_ARG(QJsonObject, groupInfo));
        } else if (type == "sendGroupMessage") {
            QJsonObject groupInfo = object.value("groupInfo").toObject();
            QJsonArray members = object["members"].toArray();
            QJsonObject senderData = object["senderData"].toObject();
            QString message = object["message"].toString();
            emit this->sendGroupMessageSignal(groupInfo, members, senderData, message);
        }
    }
}

void ClientWork::loginState(const QJsonObject &json, bool login) {
    if (login) {
        this->m_account = json["account"].toString();
        emit this->loginSucceedSignal(this->m_account);
    }
    QJsonObject object = json;
    object.insert("state", login);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::selectUserInfo(const QJsonObject &json) {
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::selectUserFriends(const QJsonObject &json) {
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::sendMessage(const QJsonObject &senderData, const QString &message, const QString &type) {
    QJsonObject jsonObject;
    jsonObject.insert("type", "receivedMessage");
    jsonObject.insert("senderData", senderData);
    jsonObject.insert("message", message);
    jsonObject.insert("messageType", type);

    QJsonDocument doc(jsonObject);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::sendFile(const QJsonObject &senderData, const QJsonObject &fileInfo, const QString &messageType) {
    QJsonObject object;
    object.insert("type", "receivedFile");
    object.insert("senderData", senderData);
    object.insert("fileInfo", fileInfo);
    object.insert("messageType", messageType);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::receivedGroupNotification(const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo) {
    QJsonObject object;
    object.insert("type", "receivedGroupInvite");
    object.insert("admin", senderData);
    object.insert("members", members);
    object.insert("groupInfo", groupInfo);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    this->m_socket->write(data);
    this->m_socket->flush();
}

void ClientWork::sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message) {
    qDebug() << "发送群聊消息" << senderData["account"].toString();

    QJsonObject object;
    object.insert("type", "receivedGroupMessage");
    object.insert("senderData", senderData);
    object.insert("groupInfo", groupInfo);
    object.insert("message", message);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
    this->m_socket->flush();
}
