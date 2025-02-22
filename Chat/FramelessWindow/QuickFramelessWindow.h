//
// Created by Flache on 2025/1/8.
//

#ifndef UNTITLED_QUICKFRAMELESSWINDOW_H
#define UNTITLED_QUICKFRAMELESSWINDOW_H

#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

#include <QEvent>
#include <QQuickItem>
#include <QQuickWindow>

#include "private/QuickFramelessWindowPrivate.h"

class QuickFramelessWindow : public QQuickWindow
{
    Q_OBJECT

    QScopedPointer<QuickFramelessWindowPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QuickFramelessWindow);
    Q_DISABLE_COPY(QuickFramelessWindow);

public:
    explicit QuickFramelessWindow(QWindow* parent = Q_NULLPTR);

    ~QuickFramelessWindow() override = default;

    Q_INVOKABLE void moveCenter();

    Q_INVOKABLE void setWindowTitleBar(QQuickItem* titleBar = Q_NULLPTR);

    Q_INVOKABLE void showWindowState(const QString& type);

    Q_INVOKABLE void ignoreTitleItem(QQuickItem* item = Q_NULLPTR);

    Q_INVOKABLE qreal windowScaleDpi();

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) Q_DECL_OVERRIDE;

    bool event(QEvent* event) override;
};

#endif  // UNTITLED_QUICKFRAMELESSWINDOW_H
