//
// Created by Flach on 25-2-8.
//

#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QPainter>
#include <QPixmap>
#include <QQuickPaintedItem>
#include <QRandomGenerator>
class Captcha : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit Captcha(QQuickItem *parent = nullptr);

    Q_INVOKABLE void rebuild();

    [[nodiscard]] Q_INVOKABLE bool captchaValue(const QString &value) const noexcept;

protected:
    void paint(QPainter *painter) override;

private:
    [[nodiscard]] static QString generateCaptcha(int length);
    [[nodiscard]] static QPixmap drawCaptcha(const QString &captcha, int width, int height);

    QPixmap m_captcha;
    QString m_captchaValue;
};

#endif  // CAPTCHA_H
