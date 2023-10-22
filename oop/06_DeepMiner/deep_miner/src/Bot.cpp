//
// Created by alex on 06.04.22.
//

#include "../headers/Bot.h"


Bot::Bot(int id, Block *block) {
    this->id = id;
    this->block = block;
    this->points = 0;
    this->position = {.x = rand() % 5, .y = rand() % 5};
}

Bot::~Bot() = default;

void Bot::mine(std::array<std::array<std::mutex,5>,5> &mutexes) {
    std::array<int, 10> section = this->block->getSection(this->position);
    /// do something to section
    /// add points
    this->points += section[9];
    section[9] = 0;
    this->block->changeSection(this->position, section);
}


void Bot::move() {
    try {
        changePosition();
    } catch (std::out_of_range& exception) {

    }
}

Position Bot::getPosition() {
    return this->position;
}

void Bot::changePosition() {
    ///decide if left or right
    int rnd = rand() % 4;

    switch (rnd) {
        case 0:///up
            if (this->position.y > 0) {
                this->position.y--;
            }
            break;
        case 1:///down
            if (this->position.y < 4) {
                this->position.y++;
            }
            break;
        case 2:///left
            if (this->position.x > 0) {
                this->position.x--;
            }
            break;
        case 3: ///right
            if (this->position.x < 4) {
                this->position.x++;
            }
            break;
        default:
            ///evtl exception thrown#
            throw std::out_of_range("Bot "+ std::to_string(this->id) + "can't change position.");
    }

}