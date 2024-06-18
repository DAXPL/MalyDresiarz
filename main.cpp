#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include "client.h"
#include "server.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Server server;
    server.startServer();

    Client client;
    client.connectToServer("127.0.0.1", 1234);

    // Hardcoded messages
    QTimer::singleShot(2000, [&client](){ client.sendMessage("Hello, server!"); });
    QTimer::singleShot(4000, [&client](){ client.sendMessage("How are you?"); });
    QTimer::singleShot(6000, [&client](){ client.sendMessage("This is a test message."); });


    return a.exec();
}
