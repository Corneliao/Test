#include "../include/imageprovidercustomround.h"

ImageProviderCustomRoundResponse::ImageProviderCustomRoundResponse(const QString &id, const QSize &requestSize) {
    this->_id = id;
    this->_requestSize = requestSize;
}

void ImageProviderCustomRoundResponse::loadImage() {
    this->_image = this->roundImage();
    if (!_image.isNull()) {
        emit this->finished();
    }
}

QImage ImageProviderCustomRoundResponse::roundImage() {
    QImage image(this->_id);

    if (image.isNull()) {
        return {};
    }
    QImage result(this->_requestSize, QImage::Format_ARGB32);
    if (result.width() <= 0)
        return {};

    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    QPainterPath path;
    qreal round_dpi = 10 * 1.0 / image.width();
    qreal temp = image.width() * round_dpi;
    path.addRoundedRect(0, 0, this->_requestSize.width(), this->_requestSize.height(), temp, temp);
    painter.setClipPath(path);
    painter.drawImage(0, 0, image.scaled(this->_requestSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return result;
}

QQuickTextureFactory *ImageProviderCustomRoundResponse::textureFactory() const {
    if (!this->_image.isNull()) {
        return QQuickTextureFactory::textureFactoryForImage(this->_image);
    }
    return {};
}

QQuickImageResponse *ImageProviderCustomRound::requestImageResponse(const QString &id, const QSize &requestSize) {
    ImageProviderCustomRoundResponse *response = new ImageProviderCustomRoundResponse(id, requestSize);
    response->loadImage();
    return response;
}
