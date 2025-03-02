#ifndef CLIENTCONTAINER_H
#define CLIENTCONTAINER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include "clientwork.h"

//---------------Server--------------
class Server : public QTcpServer {
public:
    explicit Server(QObject *parent = nullptr);

    ~Server() override;

    void dealDisconnected();
    void loginSucceed(const QString &account);
    void addFriend(const QString &sender, const QString &receiver);
    void updateFriendList(const QString &sender, const QJsonObject &json);
    void sendMessageToUser(const QJsonObject &senderData, const QString &receiver, const QString &message, const QString &type);
    void sendFileToUser(const QJsonObject &senderData, const QString &receiver, const QJsonObject &fileInfo, const QString &messageType);
    void createGroupChat(const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo);
    void sendGroupMessage(const QJsonObject &groupInfo, const QJsonArray &members, const QJsonObject &senderData, const QString &message);
    void deleteFriend(const QString &user, const QString &_friend);

protected:
    void incomingConnection(qintptr socketDescript) override;

private:
    QMap<QThread *, ClientWork *> m_clients;
    QMap<ClientWork *, QString> client_account;
    QMutex mutex;
    QMutex mutex1;
};

//---------------ClientContaier--------------
class ClientContainer : public QObject {
    Q_OBJECT
public:
    explicit ClientContainer(QObject *parent = nullptr);

signals:
private:
    Server *_server = nullptr;
};

#endif  // CLIENTCONTAINER_H
