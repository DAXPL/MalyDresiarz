#include "player.h"
Player::Player()
{
    playerName = "Default";
    isRobot = true;
    szacun = 0;
    kasa = 0;
    bmw = 0;
    haracz = 0;
}
Player::Player(std::string newName, bool isPlayerRobot)
{
    playerName = newName;
    isRobot = isPlayerRobot;
    szacun = 0;
    kasa = 0;
    bmw = 0;
    haracz = 0;
}

void Player::LansAction()
{
    szacun ++;
}

void Player::DoRobotyAction()
{
    kasa += 2;
}

void Player::HaraczAction()
{
    if(kasa < 4) return;

    kasa -= 4;
    haracz ++;
}

void Player::BMWAction()
{
    bmw ++;
}

void Player::IwanAction()
{
    if(kasa >= 1) kasa --;
}

void Player::OnRoundStart()
{
    kasa += haracz;

    for(int i = 0; i<bmw; i++)
    {
        if(kasa >= 2)
        {
            kasa -= 2;
            szacun += 3;
        }
        else
        {
            szacun -= 3;
        }
    }

}

void Player::MakeRandomMove()
{
    int choice = std::rand() % ((kasa>=4)?5:4);
    switch(choice)
    {
    case 0:
        LansAction();
        break;
    case 1:
        DoRobotyAction();
        break;
    case 2:
        BMWAction();
        break;
    case 3:
        IwanAction();
        break;
    case 4:
        HaraczAction();
        break;
    }
}
