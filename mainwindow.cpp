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
    if(canMove==false)return;
    players.at(currentPlayer)->LansAction();
    EndPlayerMove();
}

void MainWindow::on_RobotaButton_clicked()
{
    if(canMove==false)return;
    players.at(currentPlayer)->DoRobotyAction();
    EndPlayerMove();
}

void MainWindow::on_HaraczButton_clicked()
{
    if(canMove==false)return;
    players.at(currentPlayer)->HaraczAction();
    EndPlayerMove();
}

void MainWindow::on_BMWButton_clicked()
{
    if(canMove==false)return;
    players.at(currentPlayer)->BMWAction();
    EndPlayerMove();
}

void MainWindow::on_IvanButton_clicked()
{
    if(canMove==false)return;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DODAC DO SERWERA
    for(int i = 0; i<players.size();i++)
    {
        if(i != currentPlayer) players.at(i)->IwanAction();
    }
    EndPlayerMove();
}

void MainWindow::onSetPlayerUI()
{
    if(client.hasToken)
    {
        Player* curPlayer = players.at(currentPlayer);

        if(curPlayer == nullptr) return;

        ui->HaraczButton->setEnabled(curPlayer->kasa >= 4);
        ui->BMWButton->setEnabled(true);
        ui->IvanButton->setEnabled(true);
        ui->LansButton->setEnabled(true);
        ui->RobotaButton->setEnabled(true);

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
        canMove = true;
    }
    else
    {
        ui->HaraczButton->setEnabled(false);
        ui->BMWButton->setEnabled(false);
        ui->IvanButton->setEnabled(false);
        ui->LansButton->setEnabled(false);
        ui->RobotaButton->setEnabled(false);

        ui->PlayerLabel->setText(QString::fromStdString("Remote player"));
        ui->SzacunLabel->setText(QString::number(0));
        ui->KasaLabel->setText(QString::number(0));
        ui->BMLabel->setText(QString::number(0));
        ui->HaraczLabel->setText(QString::number(0));
    }
}

void MainWindow::EndPlayerMove()
{
    currentPlayer ++;
    canMove = false;
    if(currentPlayer >= players.size())
    {
        currentPlayer = 0;
        client.sendMessage("pass");
    }
    else
    {
        std::string s = "current|"+players.at(currentPlayer)->playerName;
        client.sendMessage(QString::fromStdString(s));
    }

    //setPlayerUI();
}

void MainWindow::onShowFinalResult(std::string result)
{
    qDebug() << result;
    ui->GameResultWidget->setVisible(true);
    ui->GameplayWidget->setVisible(false);

    ui->ResultLabel->setText(QString::fromStdString(result));
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

    client.SetPlayerListReference(&players);
    client.connectToServer(ip, 1234);
}

void MainWindow::on_StartGameButton_clicked()
{
    ui->PlayerCreationWidget->setVisible(false);
    ui->GameResultWidget->setVisible(false);
    ui->GameplayWidget->setVisible(true);
    ui->PlayerWaitingForGame->setVisible(false);


    connect(&client,&Client::updateUISignal,this,&MainWindow::onSetPlayerUI);
    connect(&client,&Client::showFinalResult,this,&MainWindow::onShowFinalResult);

    client.sendMessage(QString("startGame"));
}
