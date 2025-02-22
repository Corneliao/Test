#include <QCoreApplication>
#include <QThread>
#include <iostream>
#include <QThreadPool>
#include "clientcontainer.h"
class App : public QThread {
public:
    explicit App() = default;

protected:
    void run() override;
};

void App::run() {
    std::string command;
    qDebug() << "please input quit exit this app";
    while (true) {
        std::cin >> command;
        if (command == "quit")
            break;
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QThreadPool::globalInstance()->setMaxThreadCount(3);
    App *app = new App;
    QObject::connect(app, &QThread::finished, &a, &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(app, &QThread::finished, app, &QThread::deleteLater, Qt::QueuedConnection);
    app->start();

    ClientContainer client;

    return a.exec();
}
