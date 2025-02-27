#include "../include/miniomanager.h"
#include <fstream>
MinioManager::MinioManager(const QString &type, const QString &filepath, const QString &messageType, QObject *parent) {
    this->setAutoDelete(true);
    this->_type = type;
    this->_path = filepath;

    this->_messageType = messageType;
}

bool MinioManager::uploadFile() {
    QFile file(this->_path);

    if (!file.open(QFile::ReadOnly))
        return false;

    // 获取文件后缀名
    QFileInfo info(this->_path);
    const QString suffix = info.suffix();

    // 获取文件md5值
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(&file)) {
        qWarning("Failed to read file data.");
        return false;
    }
    file.close();
    const QString file_md5 = hash.result().toHex();

    // 读取文件
    std::ifstream infile(this->_path.toStdWString(), std::ios::binary | std::ios::ate);
    if (!infile)
        return false;
    std::streamsize fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // 上传文件
    minio::s3::BaseUrl base_url("127.0.0.1:9005", false);
    minio::creds::StaticProvider provider{"Doga", "y..750242"};
    minio::s3::Client client(base_url, &provider);

    minio::s3::PutObjectArgs args(infile, static_cast<long>(fileSize), 0);
    args.bucket = "file";
    args.object = file_md5.toStdString() + "." + suffix.toStdString();
    args.progressfunc = [&](const minio::http::ProgressFunctionArgs &progress_function_args) -> bool {
        if (progress_function_args.upload_speed > 0) {
        } else {
            const qreal progress = progress_function_args.uploaded_bytes * 1.0 / progress_function_args.upload_total_bytes;
            if (progress >= 0) {
                emit this->progressSignal(progress);
                if (progress == 1) {
                    if (this->flag) {
                        return true;
                    }
                    this->flag = true;
                    emit this->uploadFinished(file_md5 + "." + suffix);
                }
            };
        }
        return true;
    };

    if (minio::s3::PutObjectResponse response = client.PutObject(args); response) {
        return true;
    } else {
        qDebug() << response.Error().String();
    }
    return false;
}

void MinioManager::run() {
    if (this->_type == "uploadFile") {
        this->uploadFile();
    }
    if (this->_type == "downloadFile") {
        QPointer<QEventLoop> loop = new QEventLoop;
        QScopedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);

        QNetworkReply *reply = manager.data()->get(QNetworkRequest(QUrl("http://127.0.0.1:9005/file/" + this->_path)));
        connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 loadSize, qint64 totalSize) {
            qDebug() << __FUNCTION__ << loadSize * 1.0 / totalSize;
            emit this->downloadProgress(loadSize * 1.0 / totalSize);
        });

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QFile file(this->_path);
                if (file.open(QFile::WriteOnly)) {
                    file.write(data);
                    file.close();
                }
                if (this->_messageType == "File") {
                    emit this->downloadFinishedForFile(this->_path);
                } else if (this->_messageType == "Picture") {
                    emit this->downloadFinishedForPicture(this->_path);
                }
            }
            reply->deleteLater();
            loop->quit();
        });
        loop->exec();
    }
}
