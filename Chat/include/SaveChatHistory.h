//
// Created by Flach on 25-1-18.
//

#ifndef SAVECHATHISTORY_H
#define SAVECHATHISTORY_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
class SaveChatHistory : public QObject {
    Q_OBJECT
public:
    explicit SaveChatHistory();

    static void saveChatMessage(const QString &message, const QString &type, const QString &user, const QString &friend_);
    static QByteArrayList getChatsHistory(const QString &friendAccount);
    static QJsonArray getChatMessage(const QList<QString> &accountlist);

    void test();
};

#endif  // SAVECHATHISTORY_H
