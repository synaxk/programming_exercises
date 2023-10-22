//
// Created by alex on 01.03.22.
//

#include "../headers/Player.h"

Player::Player(int *gridSize) {
    this->health = 5;
    this->relics = 0;
    this->gridSize = gridSize;
    this->damageTaken = 0;
    this->healingDone = 0 ;
    this->position.x = 0;
    this->position.y = 0;
    this->buff = false;
    this->deBuff = false;
    this->weapon = false;
}

int Player::move(char direction) {
    switch (direction) {
        case 'w':
            if(position.y > 0) {
                position.y--;
                printf("\033[A"); // 033 = ESC, [, A = UP
            } else {
                printf("\a");
            }
            break;
        case 'a':
            if(position.x > 0) {
                position.x--;
                printf("\033[D"); // 033 = ESC, [, D = LEFT
            }else {
                printf("\a");
            }
            break;
        case 's':
            if(position.y < *gridSize-1) {
                position.y++;
                printf("\033[B"); // 033 = ESC, [, B = DOWN
            }else {
                printf("\a");
            }
            break;
        case 'd':
            if(position.x < *gridSize-1) {
                position.x++;
                printf("\033[C"); // 033 = ESC, [, C = RIGHT
            } else {
                printf("\a");
            }
            break;
        default:
            break;

    }
    return 1;
}

Player::Player() {

}

int Player::getHealth() {
    return this->health;
}
int Player::getRelics() {
    return this->relics;
}

int Player::addRelic() {
    this->relics++;
}

int Player::heal() {
    if (health < 5) {
        health++;
        healingDone++;
    }
    return health;
}

int Player::damage(int dmg) {
    if (health >= 1) {
        health -= dmg;
        damageTaken++;
    }
    return health;
}

struct position Player::getPosition() {
    return this->position;
}

int Player::toggleBuff() {
    if (this->buff) {
        this->buff = false;
        printf("\033[2 q\033]12;white\a");
    } else {
        this->buff = true;
        printf("\033[1 q\033]12;yellow\a");
    }
}
int Player::toggleDeBuff() {
    if (this->deBuff) {
        this->deBuff = false;
        printf("\033[2 q\033]12;white\a");
    } else {
        this->deBuff = true;
        printf("\033[1 q\033]12;red\a");

    }
}
int Player::toggleWeapon() {
    if (this->weapon) {
        this->weapon = false;
        printf("\033[2 q\033]12;white\a");
    } else {
        this->weapon = true;
        printf("\033[2 q\033]12;blue\a");
    }
}