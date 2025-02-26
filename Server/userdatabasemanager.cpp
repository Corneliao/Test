#include "userdatabasemanager.h"
#include "globalproperties.h"
UserDatabaseManager::UserDatabaseManager(QObject *parent) {}

UserDatabaseManager::~UserDatabaseManager() {
    qInfo() << __FUNCTION__ << "数据据线程：" << QThread::currentThreadId() << "销毁~";
    {
        QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
        db.close();
    }
    QSqlDatabase::removeDatabase(this->sql_connectionName);
}

void UserDatabaseManager::init() {
    qInfo() << __FUNCTION__ << "数据据线程：" << QThread::currentThreadId();
    this->sql_connectionName = QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()));
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", this->sql_connectionName);
    db.setPassword("y..750242");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPort(3306);
    db.setDatabaseName("user");
}

void UserDatabaseManager::registerUser(const QVariant &user_info) {
    userInfo info = user_info.value<userInfo>();

    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);

    if (!db.transaction())
        return;
    QSqlQuery query(db);

    query.prepare("INSERT INTO usersinfo (userAccount,userName,userPassword,gender,email,phone) VALUES (:account,:name,:password,:gender,:email,:phone)");
    query.bindValue(":account", info.user_account);
    query.bindValue(":name", info.user_name);
    query.bindValue(":password", info.user_password);
    query.bindValue(":gender", info.user_gender);
    query.bindValue(":email", info.user_email);
    query.bindValue(":phone", info.user_phone);
    if (query.exec()) {
        db.commit();
    } else {
        db.rollback();
        qDebug() << query.lastError();
    }
}

void UserDatabaseManager::login(const QVariant &user_info) {
    userInfo info = user_info.value<userInfo>();

    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
    QSqlQuery query(db);
    // query.prepare("select userPassword from usersinfo where userAccount = " + info.user_account);
    query.prepare("select userPassword,userName,gender,email,phone from usersinfo where userAccount = " + info.user_account);
    if (query.exec()) {
        while (query.next()) {
            QJsonObject object;
            QString password = query.value(0).toString();
            object.insert("type", "loginVerify");
            if (password == info.user_password) {
                QString name = query.value(1).toString();
                QString gender = query.value(2).toString();
                QString email = query.value(3).toString();
                QString phone = query.value(4).toString();

                object.insert("account", info.user_account);
                object.insert("name", name);
                object.insert("gender", gender);
                object.insert("phone", phone);
                object.insert("email", email);
                emit this->loginSignal(object, true);
            } else {
                emit this->loginSignal(object, false);
            }
            return;
        }

    } else {
        query.lastError();
    }
}

void UserDatabaseManager::selectUserInfo(const QString &type, const QString &account) {
    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
    QSqlQuery query(db);

    query.prepare("select userName,gender,email,phone from usersinfo where userAccount = " + account);
    if (query.exec()) {
        while (query.next()) {
            QString name = query.value(0).toString();
            QString gender = query.value(1).toString();
            QString email = query.value(2).toString();
            QString phone = query.value(3).toString();

            qDebug() << "查询到用户信息：" << "用户名：" << name << "性别：" << gender << "邮箱：" << email << "电话：" << phone;
            QJsonObject object;
            object.insert("type", type);
            object.insert("account", account);
            object.insert("name", name);
            object.insert("gender", gender);
            object.insert("phone", phone);
            object.insert("email", email);

            emit this->userInfoSignal(object);
        }
    } else {
        qInfo() << __FUNCTION__ << query.lastError();
    }
}

void UserDatabaseManager::agreeFriendRequest(const QString &sender, const QString &receiver) {
    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
    if (!db.transaction())
        return;

    QSqlQuery query(db);
    query.prepare("insert into userfriend (user,friend) values (:user,:friend)");
    query.bindValue(":user", sender);
    query.bindValue(":friend", receiver);
    if (query.exec()) {
        query.prepare("insert into userfriend (user,friend) values (:user,:friend)");
        query.bindValue(":user", receiver);
        query.bindValue(":friend", sender);
        if (query.exec()) {
            db.commit();

        } else {
            db.rollback();
        }

    } else {
        db.rollback();
    }
}

void UserDatabaseManager::selectUserFriend(const QString &account) {
    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
    QSqlQuery query(db);

    query.prepare("select friend from userfriend where userfriend.user = " + account);

    if (query.exec()) {
        QJsonObject object;
        object.insert("type", "friendData");
        QJsonArray array;

        while (query.next()) {
            QString friend_account = query.value(0).toString();
            QSqlQuery query_(db);
            query_.prepare("select userName,gender,email,phone from usersinfo where userAccount = " + friend_account);
            if (query_.exec()) {
                while (query_.next()) {
                    QString name = query_.value(0).toString();
                    QString gender = query_.value(1).toString();
                    QString email = query_.value(2).toString();
                    QString phone = query_.value(3).toString();

                    QJsonObject data;
                    data.insert("account", friend_account);
                    data.insert("name", name);
                    data.insert("gender", gender);
                    data.insert("phone", phone);
                    data.insert("email", email);
                    data.insert("type", "user");
                    array.append(data);
                }
            }
        }
        query.prepare("select group_id from group_member where  user_id = " + account);
        if (query.exec()) {
            while (query.next()) {
                QString groupid = query.value(0).toString();
                QSqlQuery query_(db);
                query_.prepare("select group_name from group_chat where group_id = " + groupid);
                if (query_.exec()) {
                    while (query_.next()) {
                        QString group_name = query_.value(0).toString();
                        QJsonObject object;
                        object.insert("groupName", group_name);
                        object.insert("groupID", groupid);
                        object.insert("type", "Group");

                        QJsonArray groupMembers;
                        QSqlQuery query_members(db);
                        query_members.prepare("select user_id from group_member where group_id = " + groupid);
                        if (query_members.exec()) {
                            while (query_members.next()) {
                                QString user_id = query_members.value(0).toString();
                                groupMembers.append(user_id);
                            }
                            object.insert("members", groupMembers);
                        }
                        array.append(object);
                    }
                }
            }
        }
        object.insert("userData", array);
        emit this->friendDataSignal(object);
    }
}

void UserDatabaseManager::createGroupChat(const QJsonObject &admin, const QJsonArray &members, const QJsonObject &groupInfo) {
    QSqlDatabase db = QSqlDatabase::database(this->sql_connectionName);
    if (!db.transaction())
        return;
    QSqlQuery query(db);
    query.prepare("insert into group_chat (group_id,group_name) values (:id,:name)");
    query.bindValue(":id", groupInfo["groupID"].toInt());
    query.bindValue(":name", groupInfo["groupName"].toString());
    if (query.exec()) {
        query.finish();
        query.prepare("insert into group_member (group_id,user_id)values (:id,:uid)");
        query.bindValue(":id", groupInfo["groupID"].toInt());
        for (int i = 0; i < members.count(); i++) {
            query.bindValue(":uid", members[i]["account"].toString());
            if (!query.exec()) {
                db.rollback();
                emit this->createGroupChatSignal(false, {}, {}, {});
                return;
            }
        }
        query.finish();
        db.commit();
        emit this->createGroupChatSignal(true, admin, members, groupInfo);
    } else {
        qDebug() << __FUNCTION__ << "group_chat failed";
    }
}
