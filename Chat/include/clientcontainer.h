#ifndef CLIENTCONTAINER_H
#define CLIENTCONTAINER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>
#include <QQueue>
#include <QDateTime>
#include <QFile>
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
class UserInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString useraccount READ useraccount WRITE setUseraccount NOTIFY useraccountChanged FINAL)
    Q_PROPERTY(QString userpassword READ userpassword WRITE setUserpassword NOTIFY userpasswordChanged FINAL)
    Q_PROPERTY(QString userhead READ userhead WRITE setUserhead NOTIFY userheadChanged FINAL)
    Q_PROPERTY(QString gender READ gender WRITE setGender NOTIFY genderChanged FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged FINAL)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged FINAL)
public:
    explicit UserInfo(QObject *parent = nullptr);
    ~UserInfo() override;
    QString username() const;
    void setUsername(const QString &username);
    QString useraccount() const;
    void setUseraccount(const QString &useraccount);

    QString userpassword() const;
    void setUserpassword(const QString &userpassword);

    QString gender() const;
    void setGender(const QString &gender);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString phone() const;
    void setPhone(const QString &newPhone);

    QString userhead() const;
    void setUserhead(const QString &newUserhead);

signals:
    void usernameChanged();
    void useraccountChanged();

    void userpasswordChanged();

    void genderChanged();

    void emailChanged();

    void phoneChanged();

    void userheadChanged();

private:
    QString m_username;
    QString m_useraccount;
    QString m_userpassword;
    QString m_gender;
    QString m_email;
    QString m_phone;
    QString m_userhead;
};

//-----------------------------
class ClientWork : public QObject {
    Q_OBJECT
public:
    explicit ClientWork(QObject *parent = nullptr);
    void init();
    Q_INVOKABLE void setUserAccount(const QString &account);
    Q_INVOKABLE void login(const QString &account, const QString &password);
    Q_INVOKABLE void registerUser(const QVariant &user_info);
    Q_INVOKABLE void searchUser(const QString &account);
    Q_INVOKABLE void addFriend(const QString &sender, const QString &receiver);
    Q_INVOKABLE void agreeFriendRequest(const QString &sender, const QJsonObject &json);
    Q_INVOKABLE void selectUserFirend(const QString &account);
    Q_INVOKABLE void sendMessage(const QJsonObject &sender_, const QString &receiver_, const QString &message, const QString &type);
    Q_INVOKABLE void sendFile(const QJsonObject &sender_, const QString &receiver_, const QString &filepath, int index, const QString &messageType);
    Q_INVOKABLE void downloadFile(int index, const QString &fileName, const QString &messageType);
    Q_INVOKABLE void createGroupChat(const QJsonObject &sender, const QVariant &receivers, const QString &groupName);
    Q_INVOKABLE void sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message);
    Q_INVOKABLE void deleteFriend(const QString &account);
    Q_INVOKABLE void quitGroup(const QString &quiter, const QString &groupID);
signals:
    void loginSignal(const QJsonObject &json, bool login);
    void userInfoSignal(const QByteArray &data);
    void receivedRequestUserInfoSignal(const QByteArray &data);
    void updateFriendListSignal(const QJsonObject &json);
    void friendsData(const QJsonArray &json);
    void receivedMessageSignal(const QJsonObject &senderData, const QString &message, const QString &messageType);
    void uploadFileProgress(const qreal &pos, int index);
    void receivedFileSignal(const QJsonObject &senderData, const QJsonObject &fileInfo, const QString &messageType);
    void downloadFileProgress(const qreal &pos, int index);
    void downloadFinishedForFile(const QString &fileName, int index);
    void downloadFinishedForPicture(const QString &fileName, int index);
    void uploadFileForFile(const QString &fileName, int index);
    void uploadFileForPicture(const QString &fileName, int index);
    void receivedGroupInvitedSignal(const QJsonObject &groupInfo);
    void receiveGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message);
    void deleteFriendSucceed(const QString &account);

private:
    void ReadData();

    void updateProgress(const qreal &pos);

    QTcpSocket *m_socket = nullptr;
    bool isConnected = false;
    QString m_account;
    QJsonObject myJsonData;
};

class ClientContainer : public QObject {
    Q_OBJECT
public:
    explicit ClientContainer(QObject *parent = nullptr);
    ~ClientContainer() override;

    Q_INVOKABLE void connectServer();

    Q_INVOKABLE void setUserAccount(const QString &account);
    Q_INVOKABLE void login(const QString &account, const QString &password);
    Q_INVOKABLE void registerUser(const UserInfo *user_info);
    Q_INVOKABLE void searchUser(const QString &account);
    Q_INVOKABLE void addFriend(const QString &sender, const QString &receiver);
    Q_INVOKABLE void agreeFriendRequest(const QString &sender, const QJsonObject &json);
    Q_INVOKABLE void selectUserFirend(const QString &account);
    Q_INVOKABLE void sendMessage(const QJsonObject &sender_, const QString &receiver_, const QString &message, const QString &type);
    Q_INVOKABLE void sendFile(const QJsonObject &sender_, const QString &receiver_, const QString &filepath, int index, const QString &messageType);
    Q_INVOKABLE QJsonObject parseFileInfo(const QString &path);
    Q_INVOKABLE void downloadFile(int index, const QString &fileName, const QString &messageType);
    Q_INVOKABLE void createGroupChat(const QJsonObject &sender, const QVariant &receivers, const QString &groupName);
    Q_INVOKABLE QSize getPictureSize(const QString &filepath);
    Q_INVOKABLE void sendGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message);
    Q_INVOKABLE void getChatHistoryMessage(const QString &friendAccount);
    Q_INVOKABLE void getChatMessage(const QList<QString> &accounts);
    Q_INVOKABLE void deleteFriend(const QString &account);
    Q_INVOKABLE void quitGroup(const QString &quiter, const QString &groupID);
signals:
    void loginSignal(const QJsonObject &json, bool login);
    void userInfoSignal(const QByteArray &data);
    void receivedRequestUserInfoSignal(const QByteArray &data);
    void updateFriendListSignal(const QJsonObject &json);
    void friendsData(const QJsonArray &json);
    void receivedMessageSignal(const QJsonObject &senderData, const QString &message, const QString &messageType);
    void receivedFileSignal(const QJsonObject &senderData, const QJsonObject &fileInfo, const QString &messageType);
    void uploadFileProgress(const qreal &pos, int index);
    void downloadFileProgress(const qreal &pos, int index);
    void downloadFinishedForFile(const QString &fileName, int index);
    void downloadFinishedForPicture(const QString &fileName, int index);
    void uploadFileForFile(const QString &fileName, int index);
    void uploadFileForPicture(const QString &fileName, int index);
    void receivedGroupInvitedSignal(const QJsonObject &groupInfo);
    void receiveGroupMessage(const QJsonObject &groupInfo, const QJsonObject &senderData, const QString &message);
    void chatMessageSignal(const QJsonArray &array);
    void deleteFriendSucceed(const QString &account);

private:
    ClientWork *m_clientwork = nullptr;
    QThread *m_thread = nullptr;
    QFutureWatcher<QJsonArray> *watcher = nullptr;
};

#endif  // CLIENTCONTAINER_H
