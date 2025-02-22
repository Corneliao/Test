#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThreadPool>

#include "FramelessWindow/QuickFramelessWindow.h"
#include "include/imageprovider.h"
#include "include/clientcontainer.h"
#include "include/Captcha.h"
#include "include/imageproviderfromlocal.h"
#include "include/imageprovidercustomround.h"
static QObject* clientProvider(const QQmlEngine* engine, const QJSEngine* jsEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(jsEngine);

    const auto client = new ClientContainer;
    return client;
}

int main(int argc, char* argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QGuiApplication app(argc, argv);
    QThreadPool::globalInstance()->setMaxThreadCount(3);
    qmlRegisterSingletonType(QUrl("qrc:/Global.qml"), "com.global", 1, 0, "Global");
    qmlRegisterType<QuickFramelessWindow>("com.framelesswindow", 1, 0, "FramelessWindow");
    qmlRegisterSingletonType<ClientContainer>("com.client", 1, 0, "Client", clientProvider);
    qmlRegisterType<Captcha>("com.captcha", 1, 0, "Captcha");
    qmlRegisterType<UserInfo>("com.userinfo", 1, 0, "UserInfo");
    QQmlApplicationEngine engine;
    ImageProvider* provider = new ImageProvider();
    engine.addImageProvider("async", provider);
    ImageProviderFromLocal* localImageProvider = new ImageProviderFromLocal();
    engine.addImageProvider("asyncLocal", localImageProvider);
    ImageProviderCustomRound* customRound = new ImageProviderCustomRound();
    engine.addImageProvider("image_cr", customRound);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("Chat", "Main");

    return app.exec();
}
