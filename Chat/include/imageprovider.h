#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QPainter>
#include <QPainterPath>
#include <QQuickAsyncImageProvider>
#include <QQuickImageResponse>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkDiskCache>
class ImageProviderResponse : public QQuickImageResponse {
public:
    explicit ImageProviderResponse(const QString& id, const QSize& requestSize);
    ~ImageProviderResponse() override = default;
    void loadImage();

    [[nodiscard]] QImage RoundImage();

protected:
    QQuickTextureFactory* textureFactory() const override;

private:
    QString _id;
    QSize _requestSize = QSize(0, 0);
    QImage _image;
    QByteArray m_imageData;
};

class ImageProvider : public QQuickAsyncImageProvider {
    Q_OBJECT
public:
    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestSize);
};

#endif  // IMAGEPROVIDER_H
