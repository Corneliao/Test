#include "miniomanager.h"
#include <fstream>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
MinioManager::MinioManager(const QString &type, const QString &filePath, const QString &account, QObject *parent) : QObject{parent} {
    this->_type = type;
    this->_path = filePath;
    this->_account = account;
    this->setAutoDelete(true);
}

MinioManager::MinioManager(const QString &type, const QString &filePath, QObject *parent) {
    this->_type = type;
    this->_path = filePath;
    this->setAutoDelete(true);
}

bool MinioManager::uploadUserhead(const QString &account, const QString &bucketName) {
    QFile file(this->_path);

    if (!file.open(QFile::ReadOnly))
        return false;

    // 获取文件后缀名
    QFileInfo info(this->_path);
    const QString suffix = info.suffix();

    // 获取文件md5值
    // QCryptographicHash hash(QCryptographicHash::Md5);
    // if (!hash.addData(&file)) {
    //     qWarning("Failed to read file data.");
    //     return false;
    // }
    // file.close();
    // const QString file_md5 = hash.result().toHex();

    // 读取文件
    std::ifstream infile(this->_path.toStdString(), std::ios::binary | std::ios::ate);
    if (!infile)
        return false;
    std::streamsize fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // 上传文件
    minio::s3::BaseUrl base_url("127.0.0.1:9005", false);
    minio::creds::StaticProvider provider{"Doga", "y..750242"};
    minio::s3::Client client(base_url, &provider);

    minio::s3::PutObjectArgs args(infile, static_cast<long>(fileSize), 0);
    args.bucket = bucketName.toStdString();
    args.object = account.toStdString() + "." + suffix.toStdString();
    args.progressfunc = [&](const minio::http::ProgressFunctionArgs &progress_function_args) -> bool {
        if (progress_function_args.upload_speed > 0) {
        } else {
            const qreal progress = progress_function_args.uploaded_bytes * 1.0 / progress_function_args.upload_total_bytes;
            if (progress >= 0) {
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
    std::ifstream infile(this->_path.toStdString(), std::ios::binary | std::ios::ate);
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
                qDebug() << progress;
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
    if (this->_type == "upload") {
        [[maybe_unused]] bool flag = this->uploadUserhead(this->_account, "userhead");
    }
    if (this->_type == "uploadFile") {
        [[maybe_unused]] bool flag = this->uploadFile();
    }
}
