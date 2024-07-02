#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    void startServer();

signals:
    void ConnectionToHub(std::string result);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();

private:
    QList<QTcpSocket *> clients;
    int currentClientTour {0};
    int tourCounter {0};
    int roundsToPlay {3};

    int reportedWinners {0};
    int reportedWinnerScore {0};
    QString reportedWinner {"None"};

    void PassToken();
    void NextPlayer();
    void EndGame();
};

#endif // SERVER_H
