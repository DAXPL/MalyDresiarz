#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::startServer()
{
    if (listen(QHostAddress::Any, 1234)) {
        qDebug() << "[Server] Server started!";
    } else {
        qDebug() << "[Server] Server failed to start!";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);
    clients << client;

    connect(client, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "[Server] Client connected:" << socketDescriptor;
}

void Server::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString data = QString::fromUtf8(client->readAll());

    qDebug() << "[Server] Got data: " << data;
    QStringList messages =  data.split(";");

    for(int i = 0; i< messages.count(); i++)
    {
        QStringList args =  messages[i].split("|");
        if(args.length()<=0) return;

        std::string command = args[0].toStdString();

        //dodac liczenie argumentow
        if(command=="pass")
        {
            PassToken();
        }
        else if(command=="startGame")
        {
            qDebug() << "[Server] Starting game";
            tourCounter=0;
            currentClientTour=-1;
            PassToken();
        }
        else if(command=="current")
        {
            qDebug() << "[Server] NextPlayer:" << args[1];
            NextPlayer();
        }
        else if(command=="ivan")
        {
            qDebug() << "[Server] IVAN!";
            for(int i=0;i<clients.length();i++)
            {
                if(clients[i] != client)
                {
                    clients[i]->write(QString("ivanAction").toUtf8());
                }
            }
        }
        else if(command=="winner")
        {
            qDebug() << "[Server] Highest score has" << args[1] <<" : "<<args[2];
            bool parseScore = false;
            int score = args[2].toInt(&parseScore);

            if(parseScore)
            {
                reportedWinners ++;
                if(score > reportedWinnerScore)
                {
                    reportedWinnerScore = score;
                    reportedWinner = args[1];
                }
                else if(score == reportedWinnerScore)
                {
                    reportedWinner += "&"+args[1];
                }
            }

            if(reportedWinners>=clients.count())
            {
                qDebug() << "[Server] Game was won by" << reportedWinner <<" : "<<reportedWinnerScore;
                for(int i=0;i<clients.length();i++)
                {
                    std::string message= "showWinner|"+reportedWinner.toStdString();
                    clients[i]->write(QString::fromStdString(message).toUtf8());
                }
            }
        }
    }

}

void Server::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    clients.removeAll(client);
    client->deleteLater();

    qDebug() << "[Server] Client disconnected";
}

void Server::PassToken()
{
    qDebug() << "[Server] Passing token";
    currentClientTour++;

    if(currentClientTour >= clients.length())
    {
        currentClientTour=0;
        tourCounter++;
    }

    if(tourCounter>=roundsToPlay)
    {
        EndGame();
    }
    else
    {
        for(int i=0;i<clients.length();i++)
        {
            if(i==currentClientTour)
            {
                clients[i]->write(QString("setToken|1").toUtf8());
            }
            else
            {
                clients[i]->write(QString("setToken|0").toUtf8());
            }
        }
    }
}

void Server::NextPlayer()
{
    for(int i=0;i<clients.length();i++)
        clients[i]->write(QString("startMove").toUtf8());

}

void Server::EndGame()
{
    qDebug() <<"Ending game";

    reportedWinnerScore = 0;
    reportedWinner = "";
    reportedWinners = 0;

    for(int i=0;i<clients.length();i++)
    {
        clients[i]->write(QString("getWinner").toUtf8());
    }
}
