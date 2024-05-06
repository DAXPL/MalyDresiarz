#ifndef PLAYER_H
#define PLAYER_H

#include <string>
class Player
{
public:
    Player();
    Player(std::string newName, bool isPlayerRobot);
    std::string playerName;

    bool isRobot {false};

    int szacun {0};
    int kasa {0};
    int bmw {0};
    int haracz {0};

    void LansAction();
    void DoRobotyAction();
    void HaraczAction();
    void BMWAction();
    void IwanAction();

    void OnRoundStart();
    void MakeRandomMove();
};

#endif // PLAYER_H
