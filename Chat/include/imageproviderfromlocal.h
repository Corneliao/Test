#ifndef IMAGEPROVIDERFROMLOCAL_H
#define IMAGEPROVIDERFROMLOCAL_H

#include <QQuickAsyncImageProvider>
#include <QQuickImageResponse>
#include <QPainter>
#include <QPainterPath>
class ImageProviderFromLocalResponse : public QQuickImageResponse {
    Q_OBJECT
public:
    explicit ImageProviderFromLocalResponse(const QString& id, const QSize& requestSize);
    ~ImageProviderFromLocalResponse() override = default;
    void loadImage();
    QImage RoundImage();

protected:
    QQuickTextureFactory* textureFactory() const override;

private:
    QString _id;
    QSize _requestSize = QSize(0, 0);
    QImage _image;
};

class ImageProviderFromLocal : public QQuickAsyncImageProvider {
    Q_OBJECT
public:
    using QQuickAsyncImageProvider::QQuickAsyncImageProvider;
    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestSize);
};

#endif  // IMAGEPROVIDERFROMLOCAL_H
