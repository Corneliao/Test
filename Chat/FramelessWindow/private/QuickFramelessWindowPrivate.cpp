//
// Created by Flache on 2025/1/8.
//

#include "../private/QuickFramelessWindowPrivate.h"

#include <QGuiApplication>

#include "../QuickFramelessWindow.h"

QuickFramelessWindowPrivate::QuickFramelessWindowPrivate(QuickFramelessWindow* q) : q_ptr(q) {
    this->screenSize = QGuiApplication::primaryScreen()->size();
    this->dpr = this->devicePixelRatio();
}

void QuickFramelessWindowPrivate::setWindowStyle() {
    Q_Q(QuickFramelessWindow);
    // q->setColor(QColor(0xeff1f2));
    q->setFlags(this->flags() | Qt::WindowType::Window | Qt::FramelessWindowHint);

    // 窗口Aero风格
    HWND hwnd = reinterpret_cast<HWND>(q->winId());
    DWORD style = ::GetWindowLongPtrW(hwnd, GWL_STYLE);
    ::SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);

    // 窗口阴影
    const MARGINS shadow = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

/**
 * @brief 窗口居中
 */
void QuickFramelessWindowPrivate::moveCenter() {
    Q_Q(QuickFramelessWindow);
    q->setX((this->screenSize.width() - q->width()) / 2);
    q->setY((this->screenSize.height() - q->height()) / 2);
    q->setVisible(true);
}

/**
 * @brief 窗口伸缩
 * @param hwnd
 * @param x
 * @param y
 * @return
 */
int QuickFramelessWindowPrivate::adjustResizeWindow(HWND const& hwnd, const int& x, const int& y) {
    Q_Q(QuickFramelessWindow);
    int result = 0;
    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(hwnd, &rect);

    bool resize_width = q->minimumWidth() != q->maximumWidth();
    bool resize_height = q->minimumHeight() != q->maximumHeight();

    if (resize_width) {
        if (x >= rect.left && x < rect.left + RESIZEWINDOWBORDERWIDTH)
            result = HTLEFT;
        if (x <= rect.right && x > rect.right - RESIZEWINDOWBORDERWIDTH)
            result = HTRIGHT;
    }
    if (resize_height) {
        if (y >= rect.top && y < rect.top + RESIZEWINDOWBORDERWIDTH)
            result = HTTOP;
        if (y <= rect.bottom && y > rect.bottom - RESIZEWINDOWBORDERWIDTH)
            result = HTBOTTOM;
    }

    if (resize_width && resize_height) {
        if (x >= rect.left && x < rect.left + RESIZEWINDOWBORDERWIDTH && y >= rect.top && y < rect.top + RESIZEWINDOWBORDERWIDTH)
            result = HTTOPLEFT;
        if (x <= rect.right && x > rect.right - RESIZEWINDOWBORDERWIDTH && y >= rect.top && y < rect.top + RESIZEWINDOWBORDERWIDTH)
            result = HTTOPRIGHT;
        if (x >= rect.left && x < rect.left + RESIZEWINDOWBORDERWIDTH && y <= rect.bottom && y > rect.bottom - RESIZEWINDOWBORDERWIDTH)
            result = HTBOTTOMLEFT;
        if (x <= rect.right && x > rect.right - RESIZEWINDOWBORDERWIDTH && y <= rect.bottom && y > rect.bottom - RESIZEWINDOWBORDERWIDTH)
            result = HTBOTTOMRIGHT;
    }

    return result;
}

/**
 * @brief 设置窗口标题栏
 * @param titlebar
 */
void QuickFramelessWindowPrivate::setWindowTitleBar(QQuickItem* titlebar) {
    if (titlebar == Q_NULLPTR)
        return;
    this->titleBar = titlebar;
}

/**
 * 处理窗口标题栏
 * @param point
 * @return
 */
int QuickFramelessWindowPrivate::TackingWindowTitleBar(const QPoint& point) {
    int result = 0;
    if (this->titleBar == Q_NULLPTR)
        return result;
    QPointF pointF = this->titleBar->mapFromGlobal(QPointF(point.x() / this->dpr, point.y() / dpr));
    if (!this->titleBar->contains(pointF))
        return result;
    QQuickItem* child = this->titleBar->childAt(pointF.x(), pointF.y());
    if (!child)
        result = HTCAPTION;
    else {
        if (this->titleWhiteListItem.contains(child))
            result = HTCAPTION;
    }

    return result;
}

/**
 * @brief 窗口最大化以及最小化时调整窗口边距 防止窗口溢出
 * @param left_margin
 * @param top_margin
 * @param right_margin
 * @param bottom_margin
 */
void QuickFramelessWindowPrivate::adjustWindowMargin(const int& left_margin, const int& top_margin, const int& right_margin, const int& bottom_margin) {
    Q_Q(QuickFramelessWindow);
    if (q->contentItem()->childItems().length() <= 0)
        return;
    QQuickItem* layout_item = q->contentItem()->childItems().at(0);
    if (!layout_item)
        return;
    if (QString className = layout_item->metaObject()->className(); className.contains("Layout")) {
        QMetaObject::invokeMethod(layout_item, "adjustWindowMargins", Qt::DirectConnection, Q_ARG(QVariant, left_margin), Q_ARG(QVariant, top_margin), Q_ARG(QVariant, right_margin), Q_ARG(QVariant, bottom_margin));
    }
}

void QuickFramelessWindowPrivate::showWindowState(const QString& type) {
    Q_Q(QuickFramelessWindow);
    HWND hwnd = reinterpret_cast<HWND>(q->winId());
    if (type == tr("max"))
        if (q->windowState() == Qt::WindowState::WindowMaximized)
            ShowWindow(hwnd, SW_SHOWNORMAL);
        else
            ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    else
        ShowWindow(hwnd, SW_SHOWMINIMIZED);
}

void QuickFramelessWindowPrivate::ignoreTitleItem(QQuickItem* item) {
    if (!item)
        return;
    if (this->titleWhiteListItem.contains(item))
        return;
    this->titleWhiteListItem.append(item);
}
