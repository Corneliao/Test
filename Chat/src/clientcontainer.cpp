#include "../include/clientcontainer.h"
#include "../include/miniomanager.h"
#include <QPixmap>
#include <qthreadpool.h>
#include "../include/SaveChatHistory.h"
UserInfo::UserInfo(QObject *parent) {}

UserInfo::~UserInfo() {
    qInfo() << __FUNCTION__ << "userinfo 销毁 ";
}

QString UserInfo::username() const {
    return m_username;
}

void UserInfo::setUsername(const QString &username) {
    if (m_username == username)
        return;
    m_username = username;
}

QString UserInfo::useraccount() const {
    return m_useraccount;
}

void UserInfo::setUseraccount(const QString &useraccount) {
    if (m_useraccount == useraccount)
        return;
    m_useraccount = useraccount;
}

QString UserInfo::userpassword() const {
    return m_userpassword;
}

void UserInfo::setUserpassword(const QString &userpassword) {
    if (m_userpassword == userpassword)
        return;
    m_userpassword = userpassword;
}

QString UserInfo::userhead() const {
    return m_userhead;
}

void UserInfo::setUserhead(const QString &newUserhead) {
    if (m_userhead == newUserhead)
        return;
    m_userhead = newUserhead;
}

QString UserInfo::gender() const {
    return m_gender;
}

void UserInfo::setGender(const QString &gender) {
    if (m_gender == gender)
        return;
    m_gender = gender;
}

QString UserInfo::email() const {
    return m_email;
}

void UserInfo::setEmail(const QString &newEmail) {
    if (m_email == newEmail)
        return;
    m_email = newEmail;
}

QString UserInfo::phone() const {
    return m_phone;
}

void UserInfo::setPhone(const QString &newPhone) {
    if (m_phone == newPhone)
        return;
    m_phone = newPhone;
}

//-----------------Work------------

ClientWork::ClientWork(QObject *parent) {
    // QString uid = QUuid::createUuid().toString();
    // qDebug() << __FUNCTION__ << uid;
}

void ClientWork::init() {
    this->m_socket = new QTcpSocket(this);
    this->m_socket->connectToHost(QHostAddress::LocalHost, 8888);
    if (!this->m_socket->waitForConnected()) {
        this->isConnected = false;
        return;
    }
    this->isConnected = true;
    connect(this->m_socket, &QTcpSocket::readyRead, this, &ClientWork::ReadData, Qt::DirectConnection);
}

void ClientWork::setUserAccount(const QString &account) {
    this->m_account = account;
}

void ClientWork::login(const QString &account, const QString &password) {
    if (!this->isConnected)
        return;
    QJsonObject object;
    object.insert("type", "login");
    object.insert("account", account);
    object.insert("password", password);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::registerUser(const QVariant &user_info) {
    if (!this->isConnected)
        return;
    UserInfo *info = user_info.value<UserInfo *>();

    QJsonObject object;
    object.insert("type", "register");
    object.insert("useraccount", info->useraccount());
    object.insert("username", info->username());
    object.insert("password", info->userpassword());
    object.insert("userhead", info->userhead());
    object.insert("email", info->email());
    object.insert("phone", info->phone());
    object.insert("gender", info->gender());

    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::searchUser(const QString &account) {
    QJsonObject object;
    object.insert("type", "searchFriend");
    object.insert("account", account);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::addFriend(const QString &sender, const QString &receiver) {
    QJsonObject object;
    object.insert("type", "addFriend");
    object.insert("sender", sender);
    object.insert("receiver", receiver);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::agreeFriendRequest(const QString &sender, const QJsonObject &json) {
    QJsonObject _json = json;
    _json.remove("type");

    QJsonObject object;
    object.insert("type", "agreeRequest");
    object.insert("sender", sender);
    object.insert("userData", _json);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::selectUserFirend(const QString &account) {
    QJsonObject json;
    json.insert("type", "selectFriend");
    json.insert("account", account);

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
}

void ClientWork::sendMessage(const QJsonObject &sender_, const QString &receiver_, const QString &message, const QString &type) {
    QJsonObject jsonObject;
    jsonObject.insert("type", "sendMessage");
    jsonObject.insert("senderData", sender_);
    jsonObject.insert("receiver", receiver_);
    jsonObject.insert("message", message);
    jsonObject.insert("messageType", type);

    QJsonDocument doc(jsonObject);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
    this->m_socket->flush();

    SaveChatHistory::saveChatMessage(message, "send", sender_["account"].toString(), receiver_);
}

void ClientWork::sendFile(const QJsonObject &sender_, const QString &receiver_, const QString &filepath, int index, const QString &messageType) {
    QFileInfo info(filepath);
    qint64 fileSize = info.size();

    MinioManager *minio = new MinioManager("uploadFile", filepath, messageType);
    connect(
            minio,
            &MinioManager::uploadFinished,
            this,
            [=](const QString &fileName) {
                QJsonObject object;
                object.insert("type", "sendFile");
                object.insert("senderData", sender_);
                object.insert("receiver", receiver_);
                object.insert("fileName", fileName);
                object.insert("messageType", messageType);
                object.insert("fileSize", fileSize);

                QJsonDocument doc(object);
                QByteArray data = doc.toJson();
                this->m_socket->write(data);

                if (messageType == "File") {
                    emit this->uploadFileForFile(fileName, index);
                } else if (messageType == "Picture") {
                    emit this->uploadFileForPicture(filepath, index);
                }
            },
            Qt::QueuedConnection);
    connect(minio, &MinioManager::progressSignal, this, [=](const qreal &pos) { emit this->uploadFileProgress(pos, index); }, Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(minio);
}

void ClientWork::downloadFile(int index, const QString &fileName, const QString &messageType) {
    MinioManager *minio = new MinioManager("downloadFile", fileName, messageType);
    connect(minio, &MinioManager::downloadProgress, this, [=](const qreal &pos) { emit this->downloadFileProgress(pos, index); }, Qt::QueuedConnection);
    connect(minio, &MinioManager::downloadFinishedForFile, this, [=](const QString &_fileName) { emit this->downloadFinishedForFile(_fileName, index); }, Qt::QueuedConnection);
    connect(minio, &MinioManager::downloadFinishedForPicture, this, [=](const QString &_fileName) { emit this->downloadFinishedForPicture(_fileName, index); }, Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(minio);
}

void ClientWork::createGroupChat(const QJsonObject &sender, const QVariant &receivers, const QString &groupName) {
    // create GroupInfo

    int group_id = QDateTime::currentMSecsSinceEpoch();

    QJsonObject groupInfo;
    groupInfo.insert("groupName", groupName);
    groupInfo.insert("groupID", group_id);
    groupInfo.insert("admin", sender["account"].toString());

    QJsonArray array;

    QList arrayList = receivers.toList();

    for (int i = 0; i < arrayList.size(); i++) {
        array.append(arrayList[i].toJsonObject());
    }
    array.append(sender);

    QJsonObject object;
    object.insert("type", "createGroupChat");
    object.insert("senderData", sender);

    object.insert("members", array);
    object.insert("groupInfo", groupInfo);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
    this->m_socket->flush();
}

void ClientWork::sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message) {
    QString sender_account = senderData["account"].toString();

    QJsonArray member = groupInfo["members"].toArray();

    for (int i = 0; i < member.count(); i++) {
        if (member[i].toString() == sender_account) {
            member.removeAt(i);
            break;
        }
    }

    QJsonObject object;
    object.insert("type", "sendGroupMessage");
    object.insert("groupInfo", groupInfo);
    object.insert("members", member);
    object.insert("message", message);
    object.insert("senderData", senderData);

    QJsonDocument doc(object);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    this->m_socket->write(data);
    this->m_socket->flush();
}

void ClientWork::deleteFriend(const QString &account) {
    QJsonObject object;
    object.insert("type", "deleteFriend");
    object.insert("account", account);
    QJsonDocument doc(object);
    QByteArray data = doc.toJson();
    this->m_socket->write(data);
    this->m_socket->flush();
}

void ClientWork::ReadData() {
    QByteArray data = this->m_socket->readAll();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(data, &error);

    if (document.isObject() && error.error == QJsonParseError::NoError) {
        QJsonObject object = document.object();
        if (QString type = object["type"].toString(); type == "loginVerify") {
            bool state = object["state"].toBool();
            if (state) {
                object["type"] = "user";
                this->myJsonData = object;
                emit this->loginSignal(object, state);
            } else {
                emit this->loginSignal(QJsonObject(), state);
            }

        } else if (type == "searchResult") {
            QString account = object["account"].toString();
            QString name = object["name"].toString();
            QString gender = object["gender"].toString();
            QString phone = object["phone"].toString();
            QString email = object["email"].toString();

            QJsonObject object;
            object.insert("account", account);
            object.insert("name", name);
            object.insert("gender", gender);
            object.insert("phone", phone);
            object.insert("email", email);
            object["type"] = "user";
            QJsonDocument doc(object);
            QByteArray data = doc.toJson();
            emit this->userInfoSignal(data);
        } else if (type == "receivedRequest") {
            QString account = object["account"].toString();
            QString name = object["name"].toString();
            QString gender = object["gender"].toString();
            QString phone = object["phone"].toString();
            QString email = object["email"].toString();

            QJsonObject object;
            object.insert("account", account);
            object.insert("name", name);
            object.insert("gender", gender);
            object.insert("phone", phone);
            object.insert("email", email);
            object["type"] = "user";
            QJsonDocument doc(object);
            QByteArray data = doc.toJson();
            emit this->receivedRequestUserInfoSignal(data);
        } else if (type == "updateFriendList") {
            QJsonObject userData = object["userData"].toObject();
            userData["type"] = "user";
            emit this->updateFriendListSignal(userData);
        } else if (type == "friendData") {
            QJsonArray userData = object["userData"].toArray();
            emit this->friendsData(userData);
        } else if (type == "receivedMessage") {
            QJsonObject sender = object["senderData"].toObject();
            QString message = object["message"].toString();
            QString messageType = object["messageType"].toString();
            SaveChatHistory::saveChatMessage(message, "recv", this->m_account, sender["account"].toString());
            emit this->receivedMessageSignal(sender, message, messageType);
        } else if (type == "receivedFile") {
            QJsonObject senderData = object["senderData"].toObject();
            QJsonObject fileInfo = object["fileInfo"].toObject();
            QString messageType = object["messageType"].toString();
            emit this->receivedFileSignal(senderData, fileInfo, messageType);
        } else if (type == "receivedGroupInvite") {
            QJsonObject adminData = object["admin"].toObject();
            Q_UNUSED(adminData);
            QJsonArray array = object["members"].toArray();
            QJsonObject groupInfo = object["groupInfo"].toObject();
            emit this->receivedGroupInvitedSignal(array, groupInfo);
        } else if (type == "receivedGroupMessage") {
            QJsonObject senderData = object["senderData"].toObject();
            QJsonObject groupInfo = object["groupInfo"].toObject();
            QString message = object["message"].toString();
            emit this->receiveGroupMessage(groupInfo, senderData, message);
        } else if (type == "deleteFriend") {
            QString account = object["account"].toString();
            emit this->deleteFriendSucceed(account);
        }
    }
}

void ClientWork::updateProgress(const qreal &pos) {}

//-----------------Container
ClientContainer::ClientContainer(QObject *parent) : QObject{parent} {
    this->watcher = new QFutureWatcher<QJsonArray>(this);
    connect(this->watcher, &QFutureWatcher<QJsonArray>::finished, this, [=]() {
        auto list = this->watcher->result();
        emit this->chatMessageSignal(list);
    });
}

ClientContainer::~ClientContainer() {
    if (this->m_thread) {
        this->m_thread->exit();
        this->m_thread->wait();
    }
}

void ClientContainer::connectServer() {
    this->m_thread = new QThread;
    this->m_clientwork = new ClientWork;
    this->m_clientwork->moveToThread(this->m_thread);
    connect(this->m_thread, &QThread::started, this->m_clientwork, &ClientWork::init, Qt::DirectConnection);
    connect(this->m_thread, &QThread::finished, this->m_clientwork, &ClientWork::deleteLater, Qt::DirectConnection);
    connect(this->m_thread, &QThread::finished, this->m_thread, &QThread::deleteLater, Qt::DirectConnection);
    connect(this->m_clientwork, &ClientWork::loginSignal, this, &ClientContainer::loginSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::userInfoSignal, this, &ClientContainer::userInfoSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::receivedRequestUserInfoSignal, this, &ClientContainer::receivedRequestUserInfoSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::updateFriendListSignal, this, &ClientContainer::updateFriendListSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::friendsData, this, &ClientContainer::friendsData, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::receivedMessageSignal, this, &ClientContainer::receivedMessageSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::uploadFileProgress, this, &ClientContainer::uploadFileProgress, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::receivedFileSignal, this, &ClientContainer::receivedFileSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::downloadFileProgress, this, &ClientContainer::downloadFileProgress, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::downloadFinishedForFile, this, &ClientContainer::downloadFinishedForFile, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::downloadFinishedForPicture, this, &ClientContainer::downloadFinishedForPicture, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::uploadFileForFile, this, &ClientContainer::uploadFileForFile, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::uploadFileForPicture, this, &ClientContainer::uploadFileForPicture, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::receivedGroupInvitedSignal, this, &ClientContainer::receivedGroupInvitedSignal, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::receiveGroupMessage, this, &ClientContainer::receiveGroupMessage, Qt::QueuedConnection);
    connect(this->m_clientwork, &ClientWork::deleteFriendSucceed, this, &ClientContainer::deleteFriendSucceed, Qt::QueuedConnection);
    this->m_thread->start();
}

void ClientContainer::setUserAccount(const QString &account) {
    QMetaObject::invokeMethod(this->m_clientwork, "setUserAccount", Qt::QueuedConnection, Q_ARG(QString, account));
}

void ClientContainer::login(const QString &account, const QString &password) {
    QMetaObject::invokeMethod(this->m_clientwork, "login", Qt::QueuedConnection, Q_ARG(QString, account), Q_ARG(QString, password));
}

void ClientContainer::registerUser(const UserInfo *user_info) {
    QMetaObject::invokeMethod(this->m_clientwork, "registerUser", Qt::QueuedConnection, Q_ARG(QVariant, QVariant::fromValue(user_info)));
}

void ClientContainer::searchUser(const QString &account) {
    QMetaObject::invokeMethod(this->m_clientwork, "searchUser", Qt::QueuedConnection, Q_ARG(QString, account));
}

void ClientContainer::addFriend(const QString &sender, const QString &receiver) {
    QMetaObject::invokeMethod(this->m_clientwork, "addFriend", Qt::QueuedConnection, Q_ARG(QString, sender), Q_ARG(QString, receiver));
}

void ClientContainer::agreeFriendRequest(const QString &sender, const QJsonObject &json) {
    QMetaObject::invokeMethod(this->m_clientwork, "agreeFriendRequest", Qt::QueuedConnection, Q_ARG(QString, sender), Q_ARG(QJsonObject, json));
}

void ClientContainer::selectUserFirend(const QString &account) {
    QMetaObject::invokeMethod(this->m_clientwork, "selectUserFirend", Qt::QueuedConnection, Q_ARG(QString, account));
}

void ClientContainer::sendMessage(const QJsonObject &sender_, const QString &receiver_, const QString &message, const QString &type) {
    QMetaObject::invokeMethod(this->m_clientwork, "sendMessage", Qt::QueuedConnection, Q_ARG(QJsonObject, sender_), Q_ARG(QString, receiver_), Q_ARG(QString, message), Q_ARG(QString, type));
}

void ClientContainer::sendFile(const QJsonObject &sender_, const QString &receiver_, const QString &filepath, int index, const QString &messageType) {
    QMetaObject::invokeMethod(this->m_clientwork, "sendFile", Qt::QueuedConnection, Q_ARG(QJsonObject, sender_), Q_ARG(QString, receiver_), Q_ARG(QString, filepath), Q_ARG(int, index), Q_ARG(QString, messageType));
}

QJsonObject ClientContainer::parseFileInfo(const QString &path) {
    QFileInfo info(path);

    QString fileName = info.fileName();
    qint64 fileSize = info.size();

    QJsonObject fileObject;
    fileObject.insert("fileName", fileName);
    fileObject.insert("fileSize", fileSize);
    fileObject.insert("filePath", path);
    return fileObject;
}

void ClientContainer::downloadFile(int index, const QString &fileName, const QString &messageType) {
    QMetaObject::invokeMethod(this->m_clientwork, "downloadFile", Qt::QueuedConnection, Q_ARG(int, index), Q_ARG(QString, fileName), Q_ARG(QString, messageType));
}

void ClientContainer::createGroupChat(const QJsonObject &sender, const QVariant &receivers, const QString &groupName) {
    QMetaObject::invokeMethod(this->m_clientwork, "createGroupChat", Qt::QueuedConnection, Q_ARG(QJsonObject, sender), Q_ARG(QVariant, receivers), Q_ARG(QString, groupName));
}

QSize ClientContainer::getPictureSize(const QString &filepath) {
    QPixmap pixmap(filepath);
    if (!pixmap.isNull()) {
        return pixmap.size();
    }
    return {};
}

void ClientContainer::sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message) {
    QMetaObject::invokeMethod(this->m_clientwork, "sendGroupMessage", Qt::QueuedConnection, Q_ARG(QJsonObject, groupInfo), Q_ARG(QJsonObject, senderData), Q_ARG(QString, message));
}

void ClientContainer::getChatHistoryMessage(const QString &friendAccount) {
    // SaveChatHistory history;
    // QFuture<QByteArrayList> future = QtConcurrent::run(&SaveChatHistory::getChatsHistory, friendAccount);
    // this->watcher->setFuture(future);
}

void ClientContainer::getChatMessage(const QList<QString> &accounts) {
    SaveChatHistory history;
    QFuture<QJsonArray> future = QtConcurrent::run(&SaveChatHistory::getChatMessage, accounts);
    this->watcher->setFuture(future);
}

void ClientContainer::deleteFriend(const QString &account) {
    QMetaObject::invokeMethod(this->m_clientwork, "deleteFriend", Qt::QueuedConnection, Q_ARG(QString, account));
}
