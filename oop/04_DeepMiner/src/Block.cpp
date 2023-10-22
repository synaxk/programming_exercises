//
// Created by alex on 06.04.22.
//

#include "../headers/Block.h"
Block::~Block() = default;

Block::Block() {
    this->initFields();
}

void Block::initFields() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 10; k++) {
                this->block[i][j][k] = (rand() % 10);
            }
        }
    }
}

std::array<int, 10> Block::getSection(Position position) {
    std::array<int, 10> section = this->block[position.x][position.y];
    return section;
}

void Block::changeSection(Position position, std::array<int, 10> section) {
    this->block[position.x][position.y] = section;
}

void Block::printSection(Position pos) {
    for (size_t i = 0; i < this->block[pos.x][pos.y].size(); i++) {
        std::cout << "Value at index(" << i << "): " << this->block[pos.x][pos.y][i] << std::endl;
    }
}

bool Block::checkIfSectionEmpty(std::array<int, 10> section) {
    if (std::all_of(section.begin(), section.end(), [](int i){return i == 0;})) {
        return true;
    }
    return false;
}

bool Block::checkIfBlockEmpty() {
    Position p = {.x = 0, .y = 0};
    for (;p.x < 5; p.x++){
        for (;p.y<5;p.y++){
            std::array<int, 10> section = this->getSection(p);
            if (!checkIfSectionEmpty(section)) {
                return false;
            }
        }
    }
    return true;
}

void Block::printBlock() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int val = 0;
            for (int k = 0; k < 10; k++) {
                val += block[i][j][k];
            }
            std::cout << " " << val <<" ";
            if (j == 4) {
                std::cout << std::endl;
            }
        }
    }
}