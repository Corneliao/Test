#include "../include/imageproviderfromlocal.h"

ImageProviderFromLocalResponse::ImageProviderFromLocalResponse(const QString &id, const QSize &requestSize) {
    this->_id = id;
    this->_requestSize = requestSize;
}

QQuickImageResponse *ImageProviderFromLocal::requestImageResponse(const QString &id, const QSize &requestSize) {
    ImageProviderFromLocalResponse *local_response = new ImageProviderFromLocalResponse(id, requestSize);
    local_response->loadImage();
    return local_response;
}

void ImageProviderFromLocalResponse::loadImage() {
    this->_image = this->RoundImage();
    emit this->finished();
}

QImage ImageProviderFromLocalResponse::RoundImage() {
    QImage image(this->_id);
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

QQuickTextureFactory *ImageProviderFromLocalResponse::textureFactory() const {
    if (this->_image.isNull())
        return {};
    return QQuickTextureFactory::textureFactoryForImage(this->_image);
}
