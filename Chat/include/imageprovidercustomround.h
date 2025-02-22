#ifndef IMAGEPROVIDERCUSTOMROUND_H
#define IMAGEPROVIDERCUSTOMROUND_H

#include <QQuickAsyncImageProvider>
#include <QObject>
#include <QQuickImageResponse>
#include <QPainter>
#include <QPainterPath>
class ImageProviderCustomRoundResponse : public QQuickImageResponse {
public:
    explicit ImageProviderCustomRoundResponse(const QString& id, const QSize& requestSize);
    void loadImage();

    QImage roundImage();

protected:
    QQuickTextureFactory* textureFactory() const override;

private:
    QImage _image;
    QString _id;
    QSize _requestSize = QSize(-1, -1);
};

class ImageProviderCustomRound : public QQuickAsyncImageProvider {
    Q_OBJECT
public:
    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestSize);
};

#endif  // IMAGEPROVIDERCUSTOMROUND_H
