#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::startServer()
{
    if (listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server started!";
    } else {
        qDebug() << "Server failed to start!";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);
    clients << client;

    connect(client, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "Client connected:" << socketDescriptor;
}

void Server::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = client->readAll();
    qDebug() << "Server got data: " << QString::fromUtf8(data);

    for (QTcpSocket *otherClient : clients)
    {
            otherClient->write(data);
    }
}

void Server::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    clients.removeAll(client);
    client->deleteLater();

    qDebug() << "Client disconnected";
}
