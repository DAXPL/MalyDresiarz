#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "player.h"
#include "client.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Player*> players;

private slots:
    void on_LansButton_clicked();
    void on_RobotaButton_clicked();
    void on_HaraczButton_clicked();
    void on_BMWButton_clicked();
    void on_IvanButton_clicked();

    void on_addPlayerButton_clicked();

    void on_endCreationButton_clicked();

    void on_StartGameButton_clicked();

    void on_isHostCheckbox_stateChanged(int arg1);

public slots:
    void onSetPlayerUI();
    void onShowFinalResult(std::string result);
    void on_Ivan();
    void onConnectionToHub(std::string result);

private:
    Ui::MainWindow *ui;
    int currentPlayer {0};
    int roundCounter {1};

    void EndPlayerMove();

    bool canMove{false};

    Server server;
    Client client;
};
#endif // MAINWINDOW_H
