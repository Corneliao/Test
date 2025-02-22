//
// Created by Flache on 2025/1/8.
//

#ifndef UNTITLED_QUICKFRAMELESSWINDOWPRIVATE_H
#define UNTITLED_QUICKFRAMELESSWINDOWPRIVATE_H

#include <QQuickWindow>
#include <QScreen>

#define RESIZEWINDOWBORDERWIDTH 8

class QuickFramelessWindow;

class QuickFramelessWindowPrivate : public QQuickWindow {
Q_OBJECT

    QuickFramelessWindow *const q_ptr;

    Q_DECLARE_PUBLIC(QuickFramelessWindow);
public:
    explicit QuickFramelessWindowPrivate(QuickFramelessWindow *q);

    ~QuickFramelessWindowPrivate() override = default;

private:
    void setWindowStyle();

    int adjustResizeWindow(const HWND &hwnd, const int &x, const int &y);

    void moveCenter();

    void setWindowTitleBar(QQuickItem *titlebar = Q_NULLPTR);

    int TackingWindowTitleBar(const QPoint &point);

    void adjustWindowMargin(const int &left_margin, const int &top_margin, const int &right_margin,
                            const int &bottom_margin);

    void showWindowState(const QString &type);

    void ignoreTitleItem(QQuickItem * item = Q_NULLPTR);

    QSize screenSize = QSize(0, 0);
    QQuickItem *titleBar = Q_NULLPTR;
    qreal dpr = 0.0;
    QList<QQuickItem *> titleWhiteListItem;
};


#endif //UNTITLED_QUICKFRAMELESSWINDOWPRIVATE_H
