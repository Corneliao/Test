//
// Created by Flach on 25-2-8.
//

#include "../include/Captcha.h"
Captcha::Captcha(QQuickItem *parent) {}
void Captcha::rebuild() {
    this->m_captchaValue = generateCaptcha(4);
    this->m_captcha = drawCaptcha(this->m_captchaValue, 120, 40);
    this->update();
}
bool Captcha::captchaValue(const QString &value) const noexcept {
    const QString lower = this->m_captchaValue.toLower();
    if (const QString upper = this->m_captchaValue.toUpper(); value == lower || value == upper) {
        return true;
    }
    return false;
}
void Captcha::paint(QPainter *painter) {
    this->m_captchaValue = generateCaptcha(4);
    this->m_captcha = drawCaptcha(this->m_captchaValue, 120, 40);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPixmap(0, 0, this->m_captcha);
}
QString Captcha::generateCaptcha(const int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString captcha;
    for (int i = 0; i < length; ++i) {
        const qint64 index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        captcha.append(possibleCharacters.at(index));
    }
    return captcha;
}
QPixmap Captcha::drawCaptcha(const QString &captcha, const int width, const int height) {
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setFont(QFont("Arial", 20));
    // 绘制验证码字符
    for (int i = 0; i < captcha.length(); ++i) {
        const int x = 10 + i * 20;
        const int y = height / 2 + 10;
        const int angle = QRandomGenerator::global()->bounded(-30, 30);
        painter.save();
        painter.translate(x, y);
        painter.rotate(angle);
        painter.drawText(0, 0, QString(captcha[i]));
        painter.restore();
    }
    // 添加干扰线条
    for (int i = 0; i < 10; ++i) {
        const int x1 = QRandomGenerator::global()->bounded(width);
        const int y1 = QRandomGenerator::global()->bounded(height);
        const int x2 = QRandomGenerator::global()->bounded(width);
        const int y2 = QRandomGenerator::global()->bounded(height);
        painter.drawLine(x1, y1, x2, y2);
    }
    // 添加干扰点
    for (int i = 0; i < 50; ++i) {
        const int x = QRandomGenerator::global()->bounded(width);
        const int y = QRandomGenerator::global()->bounded(height);
        painter.drawPoint(x, y);
    }
    painter.end();
    return pixmap;
}
