//
// Created by alex on 10.03.22.
//

#include "../headers/Bullet.h"

Bullet::Bullet() {
    this->damage = 1;
}
Bullet::Bullet(int damage, struct position position) {
    this->damage = damage;
    this->position = position;
}

int Bullet::setPosition(struct position position) {
    this->position = position;
}

struct position Bullet::getPosition(){
    return this->position;
}

int Bullet::move() {
    //save cursor position
    printf("\033[s");
    //move cursor to spawn
    printf("\033[%d;%dH", this->position.y + 2, this->position.x + 2);
    //spawn monster
    printf("\033[0;31m\u29BF\033[1;37m");
    //restore cursor position
    printf("\033[u");
    return 1;
}