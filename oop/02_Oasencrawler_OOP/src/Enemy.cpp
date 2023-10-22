//
// Created by alex on 09.03.22.
//

#include "../headers/Enemy.h"


Enemy::Enemy() {
    this->health = 0;
}

int Enemy::spawn(int health) {
    this->position.x = (rand() % 9) + 1;
    this->position.y = (rand() % 9) + 1;
    this->health = health;
    this->move();
    return 1;

}

int Enemy::getHealth() {
    return this->health;
}

int Enemy::setPosition(struct position newPosition) {
    this->position = newPosition;
    return 1;
}

int Enemy::kill() {
    this->health = 0;
    return 1;
}

int Enemy::damage(int dmg) {
    this->health -= dmg;
}

int Enemy::move() {
    //save cursor position
    printf("\033[s");
    //move cursor to spawn
    printf("\033[%d;%dH", this->position.y + 2, this->position.x + 2);
    //spawn monster ☻ \u263B ⦿ 29BF
    printf("\033[0;31m\u263B\033[1;37m");
    //restore cursor position
    printf("\033[u");
    return 1;
}

struct position Enemy::getPosition() {
    return this->position;
}
