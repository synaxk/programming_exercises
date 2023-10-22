//
// Created by alex on 06.04.22.
//

#ifndef INC_04_DEEPMINER_BOT_H
#define INC_04_DEEPMINER_BOT_H
#include "Block.h"
#include <mutex>
#include "stdexcept"


class Bot {
public:
    explicit Bot(int id, Block* block);
    ~Bot();
    virtual void mine(std::array<std::array<std::mutex,5>,5> &mutexes) = 0;
    void move();
    Position getPosition();
protected:
    int id;
    int points;
    Position position{}; //weird shit
    Block* block;
private:
    void changePosition();
};


#endif //INC_04_DEEPMINER_BOT_H
