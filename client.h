#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "player.h"

class Client: public QObject
{
    Q_OBJECT

    public:
        Client(QObject *parent = nullptr);
        void connectToServer(const QString &host, quint16 port);
        void sendMessage(const QString &message);
        bool hasToken {false};
        void SetPlayerListReference(std::vector<Player*> *p);

    signals:
        void updateUISignal();
        void newRoundSignal();
        void ivanAction();
        void showFinalResult(std::string result);

    private slots:
        void onReadyRead();

    private:
        QTcpSocket *socket;
        std::vector<Player*> *players;

};

#endif // CLIENT_H
