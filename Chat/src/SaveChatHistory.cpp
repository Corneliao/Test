//
// Created by Flach on 25-1-18.
//

#include "../include/SaveChatHistory.h"

SaveChatHistory::SaveChatHistory() = default;
void SaveChatHistory::saveChatMessage(const QString &message, const QString &type, const QString &user, const QString &friend_) {
    QFile file(friend_ + ".json");
    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        const QByteArray json = file.readAll();
        file.close();

        if (const QJsonDocument doc = QJsonDocument::fromJson(json); doc.isObject()) {
            QJsonObject obj = doc.object();
            const QString user_ = obj.value("user").toString();
            if (const QString friend_account = obj.value("friend").toString(); user_ == user && friend_account == friend_) {
                QJsonArray History = obj.value("History").toArray();
                QJsonObject Message;
                Message.insert("type", type);
                Message.insert("message", message);
                History.append(Message);

                obj.insert("History", History);
                const QJsonDocument newDoc(obj);

                // 打开文件以写入
                if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                    qWarning("无法打开文件以写入");
                    return;
                }

                // 写入新的JSON数据
                QTextStream out(&file);
                out << newDoc.toJson();

                // 关闭文件
                file.close();
                return;
            }
        }
    } else {
        QJsonObject json;
        json["user"] = user;
        json["friend"] = friend_;

        QJsonObject Message;
        Message["message"] = message;
        Message["type"] = type;

        // 可以继续添加QJsonArray或其他QJsonObject
        QJsonArray history;
        history.append(Message);
        json.insert("History", history);

        // 使用QJsonDocument包装QJsonObject
        const QJsonDocument doc(json);

        // 将JSON文档转换为字符串
        const QByteArray bytes = doc.toJson();

        // 创建并打开一个文件
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 如果文件打开失败，打印错误信息并返回
            qWarning("无法打开文件");
            return;
        }

        // 将JSON数据写入文件
        QTextStream out(&file);
        out << bytes;

        // 关闭文件
        file.close();
    }
}
QByteArrayList SaveChatHistory::getChatsHistory(const QString &friendAccount) {
    QFile file(friendAccount + ".json");
    if (!file.exists())
        return {};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};
    const QByteArray json = file.readAll();
    file.close();
    if (const QJsonDocument doc = QJsonDocument::fromJson(json); doc.isObject()) {
        QByteArrayList bytes;
        const QJsonObject obj = doc.object();
        const QJsonArray history = obj.value("History").toArray();
        for (const auto &i : history) {
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            const QJsonObject messageHistory = i.toObject();
            const QString type = messageHistory.value("type").toString();
            const QString message = messageHistory.value("message").toString();
            stream << type << friendAccount << message;
            qDebug() << friendAccount << message;
            bytes.append(data);
        }
        return bytes;
    }
    return {};
}

QJsonArray SaveChatHistory::getChatMessage(const QList<QString> &accountlist) {
    QJsonArray array;
    for (int i = 0; i < accountlist.count(); i++) {
        QString account = accountlist.at(i);
        QFile file(account + ".json");
        if (!file.exists())
            return {};
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return {};
        const QByteArray json = file.readAll();
        file.close();
        if (const QJsonDocument doc = QJsonDocument::fromJson(json); doc.isObject()) {
            const QJsonObject obj = doc.object();
            array.append(obj);
        }
    }
    if (array.size() > 0)
        return array;
    return {};
}
