//
// Created by alex on 14.04.22.
//

#include "../headers/SortBot.h"
SortBot::~SortBot() = default;

SortBot::SortBot(int id, Block *block) : Bot(id, block) {

}

void SortBot::mine(std::array<std::array<std::mutex,5>,5> &mutexes){
    try {
        this->move();
    } catch (std::out_of_range &exception) {
        std::cout << exception.what() << std::endl;
    }

    mutexes[this->position.x][this->position.y].lock();
    this->block->printSection(position);
    std::array<int, 10> section = this->block->getSection(this->position);

    std::sort(section.begin(), section.end());

    this->points += section[9];
    section[9] = 0;

    this->block->changeSection(this->position, section);

    mutexes[this->position.x][this->position.y].unlock();
    this->block->printSection(this->position);
    std::cout << "SortBot mined successfully! Points: " << this->points << std::endl;

}

