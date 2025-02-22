//
// Created by Flache on 2025/1/8.
//

#include "QuickFramelessWindow.h"

QuickFramelessWindow::QuickFramelessWindow(QWindow* parent) : QQuickWindow(parent), d_ptr(new QuickFramelessWindowPrivate(this))
{
    Q_D(QuickFramelessWindow);
    d->setWindowStyle();
}

bool QuickFramelessWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    Q_D(QuickFramelessWindow);
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message) {
        case WM_NCCALCSIZE: {
            NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
            if (params.rgrc[0].top != 0) params.rgrc[0].top -= 1;
            *result = WVR_REDRAW;
            return true;
        }
        case WM_NCHITTEST: {
            int x = GET_X_LPARAM(msg->lParam);
            int y = GET_Y_LPARAM(msg->lParam);
            *result = d->adjustResizeWindow(msg->hwnd, x, y);
            if (*result != 0) return true;
            *result = d->TackingWindowTitleBar(QPoint(x, y));
            if (*result != 0) return true;
            return false;
        }
        default:
            return QQuickWindow::nativeEvent(eventType, message, result);
    }
}

void QuickFramelessWindow::moveCenter()
{
    Q_D(QuickFramelessWindow);
    d->moveCenter();
}

bool QuickFramelessWindow::event(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        Q_D(QuickFramelessWindow);
        RECT rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
        rect.left = std::abs(rect.left);
        rect.top = std::abs(rect.left);
        rect.right = std::abs(rect.left);
        rect.bottom = std::abs(rect.left);
        if (windowState() == Qt::WindowState::WindowMaximized)
            d->adjustWindowMargin(static_cast<int>(rect.left / d->dpr), static_cast<int>(rect.top / d->dpr), static_cast<int>(rect.right / d->dpr), static_cast<int>(rect.bottom / d->dpr));
        else
            d->adjustWindowMargin(0, 0, 0, 0);
    }
    return QQuickWindow::event(event);
}

void QuickFramelessWindow::setWindowTitleBar(QQuickItem* titleBar)
{
    Q_D(QuickFramelessWindow);
    d->setWindowTitleBar(titleBar);
}

void QuickFramelessWindow::showWindowState(const QString& type)
{
    Q_D(QuickFramelessWindow);
    d->showWindowState(type);
}

void QuickFramelessWindow::ignoreTitleItem(QQuickItem* item)
{
    Q_D(QuickFramelessWindow);
    d->ignoreTitleItem(item);
}

qreal QuickFramelessWindow::windowScaleDpi()
{
    Q_D(QuickFramelessWindow);
    return d->dpr;
}
