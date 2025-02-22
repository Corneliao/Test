#ifndef USERDATABASEMANAGER_H
#define USERDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonArray>
class UserDatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit UserDatabaseManager(QObject *parent = nullptr);
    ~UserDatabaseManager() override;
    void init();

    Q_INVOKABLE void registerUser(const QVariant &user_info);
    Q_INVOKABLE void login(const QVariant &user_info);
    Q_INVOKABLE void selectUserInfo(const QString &type, const QString &account);
    Q_INVOKABLE void agreeFriendRequest(const QString &sender, const QString &receiver);
    Q_INVOKABLE void selectUserFriend(const QString &account);
signals:
    void loginSignal(const QJsonObject &json, bool login = false);
    void userInfoSignal(const QJsonObject &json);
    void friendDataSignal(const QJsonObject &json);

private:
    QString sql_connectionName;
};

#endif  // USERDATABASEMANAGER_H
