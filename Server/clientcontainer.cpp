#include "clientcontainer.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
    this->listen(QHostAddress::Any, 8888);
}

Server::~Server() {
    if (this->m_clients.size() > 0) {
        for (auto iterator = this->m_clients.begin(); iterator != this->m_clients.end(); iterator++) {
            iterator.key()->exit();
            iterator.key()->wait();
        }
        this->m_clients.clear();
    }
}

void Server::dealDisconnected() {
    QMutexLocker locker(&mutex);
    ClientWork *work = qobject_cast<ClientWork *>(sender());
    this->client_account.remove(work);
    this->m_clients.key(work)->exit();
    this->m_clients.key(work)->wait();
    this->m_clients.remove(this->m_clients.key(work));
}

void Server::loginSucceed(const QString &account) {
    QMutexLocker locker(&mutex1);
    ClientWork *work = qobject_cast<ClientWork *>(sender());
    this->client_account.insert(work, account);
}

void Server::addFriend(const QString &sender, const QString &receiver) {
    if (this->client_account.contains(this->client_account.key(receiver)))
        QMetaObject::invokeMethod(this->client_account.key(receiver), "receivedFriendRequest", Qt::QueuedConnection, Q_ARG(QString, sender));
}

void Server::updateFriendList(const QString &sender, const QJsonObject &json) {
    if (this->client_account.contains(this->client_account.key(sender)))
        QMetaObject::invokeMethod(this->client_account.key(sender), "updateFriendList", Qt::QueuedConnection, Q_ARG(QJsonObject, json));
}

void Server::sendMessageToUser(const QJsonObject &senderData, const QString &receiver, const QString &message, const QString &type) {
    if (this->client_account.contains(this->client_account.key(receiver))) {
        QMetaObject::invokeMethod(this->client_account.key(receiver), "sendMessage", Qt::QueuedConnection, Q_ARG(QJsonObject, senderData), Q_ARG(QString, message), Q_ARG(QString, type));
    }
}

void Server::sendFileToUser(const QJsonObject &senderData, const QString &receiver, const QJsonObject &fileInfo, const QString &messageType) {
    if (this->client_account.contains(this->client_account.key(receiver))) {
        QMetaObject::invokeMethod(this->client_account.key(receiver), "sendFile", Qt::QueuedConnection, Q_ARG(QJsonObject, senderData), Q_ARG(QJsonObject, fileInfo), Q_ARG(QString, messageType));
    }
}

void Server::createGroupChat(const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo) {
    for (int i = 0; i < members.count(); i++) {
        QMutexLocker locker(&this->mutex);
        QString account = members[i].toString();
        if (this->client_account.contains(this->client_account.key(account))) {
            QMetaObject::invokeMethod(this->client_account.key(account), "receivedGroupNotification", Qt::QueuedConnection, Q_ARG(QJsonObject, senderData), Q_ARG(QJsonArray, members), Q_ARG(QJsonObject, groupInfo));
        }
    }
}

void Server::incomingConnection(qintptr socketDescript) {
    QThread *thread = new QThread;
    ClientWork *work = new ClientWork(socketDescript);
    work->moveToThread(thread);
    connect(thread, &QThread::started, work, &ClientWork::init, Qt::DirectConnection);
    connect(thread, &QThread::finished, work, &ClientWork::deleteLater, Qt::DirectConnection);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater, Qt::DirectConnection);
    //-------------queuedConnections--------------
    connect(work, &ClientWork::disconnected, this, &Server::dealDisconnected, Qt::QueuedConnection);
    connect(work, &ClientWork::loginSucceedSignal, this, &Server::loginSucceed, Qt::QueuedConnection);
    connect(work, &ClientWork::addFriendSignal, this, &Server::addFriend, Qt::QueuedConnection);
    connect(work, &ClientWork::updateFriendListSignal, this, &Server::updateFriendList, Qt::QueuedConnection);
    connect(work, &ClientWork::sendMessageSignal, this, &Server::sendMessageToUser, Qt::QueuedConnection);
    connect(work, &ClientWork::sendFileSignal, this, &Server::sendFileToUser, Qt::QueuedConnection);
    connect(work, &ClientWork::createGroupChatSignal, this, &Server::createGroupChat, Qt::QueuedConnection);
    thread->start();
    this->m_clients.insert(thread, work);
}

ClientContainer::ClientContainer(QObject *parent) : QObject{parent} {
    this->_server = new Server(this);
}
