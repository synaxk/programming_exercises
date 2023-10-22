//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_SHIP_H
#define FLOTTENKAMPF_SHIP_H

#include <string>
#include "ShipIsDeadException.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>


struct Position {
    int x;
    int y;
};

class Ship {
public:
    Ship(std::array<std::array<int, 10>, 20> &grid);
    ~Ship();
    virtual void attack(Ship &enemy) = 0;
    void takeDamage(int damage);
    std::string getName();
    int getSize() const;
    int getID();
    int getShell();
    void print();
    void spawn(bool player);
    void move(bool player);
protected:
    int getDistance(Ship &enemy);
    std::string name;
    int id;
    int shell;
    int size;
    int damage;
private:
    bool checkGridPos(Position position);
    void resetGridPos();
    void setGridPos();
    std::array<std::array<int, 10>, 20> &grid;
    struct Position pos;
};


#endif //FLOTTENKAMPF_SHIP_H
