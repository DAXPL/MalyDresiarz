#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);


    ui->PlayerCreationWidget->setVisible(true);
    ui->GameResultWidget->setVisible(false);
    ui->GameplayWidget->setVisible(false);
    ui->PlayerWaitingForGame->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LansButton_clicked()
{
    players.at(currentPlayer)->LansAction();
    EndPlayerMove();
}

void MainWindow::on_RobotaButton_clicked()
{
    players.at(currentPlayer)->DoRobotyAction();
    EndPlayerMove();
}

void MainWindow::on_HaraczButton_clicked()
{
    players.at(currentPlayer)->HaraczAction();
    EndPlayerMove();
}

void MainWindow::on_BMWButton_clicked()
{
    players.at(currentPlayer)->BMWAction();
    EndPlayerMove();
}

void MainWindow::on_IvanButton_clicked()
{
    for(int i = 0; i<players.size();i++)
    {
        if(i != currentPlayer) players.at(i)->IwanAction();
    }
    EndPlayerMove();
}

void MainWindow::setPlayerUI()
{
    Player* curPlayer = players.at(currentPlayer);

    if(curPlayer == nullptr) return;

    ui->HaraczButton->setEnabled(curPlayer->kasa >= 4);
    ui->PlayerLabel->setText(QString::fromStdString(curPlayer->playerName));
    ui->SzacunLabel->setText(QString::number(curPlayer->szacun));
    ui->KasaLabel->setText(QString::number(curPlayer->kasa));
    ui->BMLabel->setText(QString::number(curPlayer->bmw));
    ui->HaraczLabel->setText(QString::number(curPlayer->haracz));

    if(curPlayer->isRobot)
    {
        curPlayer->MakeRandomMove();
        EndPlayerMove();
    }
}

void MainWindow::EndPlayerMove()
{
    currentPlayer ++;

    if(currentPlayer >= players.size())
    {
        currentPlayer = 0;
        roundCounter ++;

        if(roundCounter >= 10)
        {
            //Koniec gry
            ui->GameResultWidget->setVisible(true);
            ui->GameplayWidget->setVisible(false);

            Player* winPlayer = players.at(0);

            for(int i = 0; i<players.size(); i++)
            {
                if(players.at(i)->szacun > winPlayer->szacun)
                {
                    winPlayer = players.at(i);
                }
            }

            ui->ResultLabel->setText(QString::fromStdString(winPlayer->playerName));
        }
    }

    setPlayerUI();
}

void MainWindow::on_addPlayerButton_clicked()
{
    std::string newName = ui->PlayerNameEdit->text().toStdString();
    bool isRobot = ui->isRobotCheckbox->isChecked();

    players.push_back(new Player(newName,isRobot));

    ui->PlayerNameEdit->setText("Nazwa gracza");
}

void MainWindow::on_endCreationButton_clicked()
{
    ui->PlayerCreationWidget->setVisible(false);
    ui->GameResultWidget->setVisible(false);
    ui->GameplayWidget->setVisible(false);
    ui->PlayerWaitingForGame->setVisible(true);

    bool isHost = ui->isHostCheckbox->isChecked();
    QString ip = ui->ServerIP->text();
    if(isHost)
    {
        server.startServer();
    }else{
        ui->StartGameButton->setVisible(false);
    }
    client.connectToServer(ip, 1234);
}

void MainWindow::on_StartGameButton_clicked()
{
    ui->PlayerCreationWidget->setVisible(false);
    ui->GameResultWidget->setVisible(false);
    ui->GameplayWidget->setVisible(true);
    ui->PlayerWaitingForGame->setVisible(false);

    setPlayerUI();
}
