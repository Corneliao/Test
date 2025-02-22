#ifndef MINIOMANAGER_H
#define MINIOMANAGER_H

#include <QObject>
#include <QRunnable>
#include <QCryptographicHash>
#include <miniocpp/client.h>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>
#include <QPointer>
#include <QtConcurrentRun>
#include <QFutureWatcher>
class MinioManager : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit MinioManager(const QString &type, const QString &filepath, const QString &messageType, QObject *parent = nullptr);
    bool uploadFile();

protected:
    void run() override;

private:
    QString _path;
    QString _type;
    bool flag = false;
    QString _messageType;

signals:

    void progressSignal(const qreal &pos);
    void downloadProgress(const qreal &pos);
    void downloadFinishedForFile(const QString &fileName);
    void downloadFinishedForPicture(const QString &fileName);
    void uploadFinished(const QString &fileName);
};

#endif  // MINIOMANAGER_H
