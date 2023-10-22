//
// Created by alex on 14.04.22.
//

#include "../headers/GreedyBot.h"


GreedyBot::GreedyBot(int id, Block *block) : Bot(id, block) {

}

void GreedyBot::mine(std::array<std::array<std::mutex,5>,5> &mutexes) {
    try {
        this->move();
    } catch (std::out_of_range &exception) {
        std::cout << exception.what() << std::endl;
    }

    mutexes[this->position.x][this->position.y].lock();
    std::array<int, 10> section = this->block->getSection(this->position);
    this->points += section[9] + section[8] + section[7];
    section[9] = section[8] = section[7] = 0;
    this->block->changeSection(this->position, section);
    mutexes[this->position.x][this->position.y].unlock();
    std::cout << "GreedyBot mined successfully! Points: " << this->points << std::endl;
}

GreedyBot::~GreedyBot() =
default;
