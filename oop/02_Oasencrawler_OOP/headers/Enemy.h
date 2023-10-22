//
// Created by alex on 09.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_ENEMY_H
#define INC_02_OASENCRAWLER_OOP_ENEMY_H

#include "utils.h"
#include "Player.h"

class Enemy {
public:
    Enemy();
    int spawn(int health);
    int move();
    struct position getPosition();
    int setPosition(struct position newPosition);
    int getHealth();
    int kill();
    int damage(int dmg);
private:
    int health;
    struct position position;
};


#endif //INC_02_OASENCRAWLER_OOP_ENEMY_H
