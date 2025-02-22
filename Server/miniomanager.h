#ifndef MINIOMANAGER_H
#define MINIOMANAGER_H

#include <QObject>
#include <miniocpp/client.h>
#include <QDebug>
#include <QThreadPool>
#include <QRunnable>
class MinioManager : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit MinioManager(const QString &type, const QString &filePath, const QString &account, QObject *parent = nullptr);
    explicit MinioManager(const QString &type, const QString &filePath, QObject *parent = nullptr);
    bool uploadUserhead(const QString &account, const QString &bucketName);
    bool uploadFile();
    ~MinioManager() override = default;

protected:
    void run() override;
signals:
private:
    QString _type;
    QString _path;
    QString _account;
};

#endif  // MINIOMANAGER_H
