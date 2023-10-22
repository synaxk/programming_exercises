//
// Created by alex on 01.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_PLAYER_H
#define INC_02_OASENCRAWLER_OOP_PLAYER_H

#include "utils.h"


class Player {
public:
    Player();
    Player(int *gridSize);

    int move(char direction);
    int getHealth();
    int getRelics();
    int addRelic();
    struct position getPosition();
    int heal();
    int damage(int dmg);
    int toggleBuff();
    int toggleDeBuff();
    int toggleWeapon();

    int damageTaken;
    int healingDone;

private:
    bool buff;
    bool deBuff;
    bool weapon;
    int health;
    int relics;
    int *gridSize;
    struct position position;
};


#endif //INC_02_OASENCRAWLER_OOP_PLAYER_H
