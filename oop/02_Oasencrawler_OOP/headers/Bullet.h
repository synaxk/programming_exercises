//
// Created by alex on 10.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_BULLET_H
#define INC_02_OASENCRAWLER_OOP_BULLET_H

#include "utils.h"

class Bullet {
public:
    Bullet();
    Bullet(int damage, struct position position);
    int move();
    int setPosition(struct position position);
    struct position getPosition();
private:
    int damage;
    struct position position;
};


#endif //INC_02_OASENCRAWLER_OOP_BULLET_H
