#include "client.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>

Client::Client(QObject *parent) : QObject(parent),socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

void Client::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void Client::sendMessage(const QString &message)
{
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    }
}

void Client::onReadyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "Received: " << (QString::fromUtf8(data)).toStdString();
}
