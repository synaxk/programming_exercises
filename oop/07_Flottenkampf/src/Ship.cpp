//
// Created by alex on 21.05.22.
//

#include "../headers/Ship.h"


Ship::Ship(std::array<std::array<int, 10>, 20> &grid) : grid(grid) {
    this->shell = 0;
    this->size = 0;
    this->damage = 0;
    this->pos = {.x = -1, .y = -1};
}

void Ship::takeDamage(int damage) {
    if (this->shell <= 0) {
        throw ShipIsDeadException();
    }
    this->shell -= damage;
}

void Ship::attack(Ship &enemy){}

void Ship::spawn(bool player) {
    struct Position newPos = {.x = -1, .y = -1};
    if (!player) {
        newPos = {.x = rand() % 10, .y = rand() % 10};
    } else {
        newPos = {.x = rand() % 10, .y = (rand() % 10) + 10};
    }

    if (this->grid[newPos.y][newPos.x] == -1) {
        this->pos = newPos;
        (this->grid[this->pos.y][this->pos.x]) = this->id;
    } else {
        this->spawn(player);
    }
}

void Ship::move(bool player) {
    ///check possible direction - if new position is free
    if (this->shell < 50 ) {/// if shell is low move away from opponent
        if (!player) { /// player 1 top grid
            if (this->pos.y > 0) { /// move away
                 if (checkGridPos({.x = this->pos.x, .y = this->pos.y -1 })) {
                     this->resetGridPos();
                     this->pos.y--;
                     this->setGridPos();
                 }
            }
        } else { /// player 2 - bottom grid
            if (this->pos.y < 19) { /// move away
                if (checkGridPos({.x = this->pos.x, .y = this->pos.y +1 })) {
                    this->resetGridPos();
                    this->pos.y++;
                    this->setGridPos();
                }
            }
        }
    } else { /// move in direction of opponent
        if (!player) {
            if (this->pos.y < 9) {
                if (checkGridPos({.x = this->pos.x, .y = this->pos.y + 1 })) {
                    this->resetGridPos();
                    this->pos.y++;
                    this->setGridPos();
                }
            }
        } else {
            if (this->pos.y > 10) {
                if (checkGridPos({.x = this->pos.x, .y = this->pos.y - 1 })) {
                    this->resetGridPos();
                    this->pos.y--;
                    this->setGridPos();
                }
            }
        }
    }
    ///move
    ///reset old grid position
}

int Ship::getDistance(Ship &enemy) {
    int dist, distX, distY = 0;
    distX = abs(this->pos.x - enemy.pos.x);
    distY = abs(this->pos.y - enemy.pos.y);
    dist = distX + distY;
    std::cout << "DISTANCE: " << dist << std::endl;
    return dist;
}

void Ship::resetGridPos() {
    this->grid[this->pos.y][this->pos.x] = -1;
}

void Ship::setGridPos() {
    this->grid[this->pos.y][this->pos.x] = this->id;
}

bool Ship::checkGridPos(Position position) {
    if (grid[position.y][position.x] == -1) {
        return true;
    }
    return false;
}

int Ship::getSize() const {
    return this->size;
}

int Ship::getID() {
    return this->id;
}

int Ship::getShell() {
    return this->shell;
}

std::string Ship::getName() {
    return this->name;
}

void Ship::print() {
    std::cout << this->id << " " << this->name << " - Shell:" << this->shell << std::endl;
}

Ship::~Ship() = default;
