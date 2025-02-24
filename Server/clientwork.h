#ifndef CLIENTWORK_H
#define CLIENTWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include "userdatabasemanager.h"
#include <QMutex>
#include <QMutexLocker>
class ClientWork : public QObject {
    Q_OBJECT
public:
    explicit ClientWork(const qintptr &socketDescript, QObject *parent = nullptr);
    ~ClientWork() override;
    void init();

    Q_INVOKABLE void receivedFriendRequest(const QString &sender);
    Q_INVOKABLE void updateFriendList(const QJsonObject &json);
    Q_INVOKABLE void sendMessage(const QJsonObject &senderData, const QString &message, const QString &type);
    Q_INVOKABLE void sendFile(const QJsonObject &senderData, const QJsonObject &fileInfo, const QString &messageType);
    Q_INVOKABLE void receivedGroupNotification(const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo);
    Q_INVOKABLE void sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message);
signals:
    void disconnected();
    void loginSucceedSignal(const QString &account);
    void addFriendSignal(const QString &sender, const QString &receiver);
    void updateFriendListSignal(const QString &sender, const QJsonObject &json);
    void sendMessageSignal(const QJsonObject &senderData, const QString &receiver, const QString &message, const QString &type);
    void sendFileSignal(const QJsonObject &senderData, const QString &receiver, const QJsonObject &fileInfo, const QString &messageType);
    void createGroupChatSignal(const QJsonObject &senderData, const QJsonArray &members, const QJsonObject &groupInfo);
    void sendGroupMessageSignal(const QJsonObject &groupInfo, const QJsonArray &members, const QJsonObject &senderData, const QString &message);

private:
    void ReadClientMessage();
    void loginState(const QJsonObject &json, bool login);
    void selectUserInfo(const QJsonObject &json);
    void selectUserFriends(const QJsonObject &json);
    qintptr m_socketDescript;
    QTcpSocket *m_socket = nullptr;
    QString sql_connectionName;

    QThread *sql_thread = nullptr;
    UserDatabaseManager *database_manager = nullptr;
    QString m_account;
    QMutex mutex;
};

#endif  // CLIENTWORK_H
