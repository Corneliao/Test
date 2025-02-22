#include "../include/imageprovider.h"
ImageProviderResponse::ImageProviderResponse(const QString& id, const QSize& requestSize) : _id(qMove(id)), _requestSize(qMove(requestSize)) {}

void ImageProviderResponse::loadImage() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkDiskCache* cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory("D:/cache");
    manager->setCache(cache);
    connect(
            manager,
            &QNetworkAccessManager::finished,
            this,
            [=](QNetworkReply* reply) {
                if (reply->error() == QNetworkReply::NoError) {
                    this->m_imageData = reply->readAll();
                    this->_image = this->RoundImage();
                    emit this->finished();
                }
                reply->deleteLater();
            },
            Qt::DirectConnection);
    QNetworkRequest request(this->_id);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    manager->get(request);
}

QImage ImageProviderResponse::RoundImage() {
    QImage image;
    image.loadFromData(this->m_imageData);
    if (image.isNull())
        return {};

    QImage result(this->_requestSize, QImage::Format_ARGB32);
    if (result.width() <= 0)
        return {};
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    QPainterPath path;
    QRect rect = QRect(0, 0, this->_requestSize.width(), this->_requestSize.height()).adjusted(1, 1, -1, -1);
    path.addRoundedRect(rect, this->_requestSize.width() / 2, this->_requestSize.height() / 2);
    painter.setClipPath(path);
    painter.drawImage(0, 0, image.scaled(this->_requestSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return result;
}

QQuickTextureFactory* ImageProviderResponse::textureFactory() const {
    if (this->_image.isNull())
        return {};
    return QQuickTextureFactory::textureFactoryForImage(this->_image);
}

QQuickImageResponse* ImageProvider::requestImageResponse(const QString& id, const QSize& requestSize) {
    ImageProviderResponse* response = new ImageProviderResponse(id, requestSize);
    response->loadImage();
    return response;
}
