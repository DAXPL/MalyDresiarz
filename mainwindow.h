#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "player.h"

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

private:
    Ui::MainWindow *ui;
    int currentPlayer {0};
    int roundCounter {1};

    void setPlayerUI();
    void EndPlayerMove();
};
#endif // MAINWINDOW_H
