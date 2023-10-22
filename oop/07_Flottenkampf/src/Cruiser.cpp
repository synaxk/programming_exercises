//
// Created by alex on 21.05.22.
//

#include "../headers/Cruiser.h"

Cruiser::Cruiser(int id, std::array<std::array<int, 10>, 20> &grid) : Ship(grid) {
    this->name = "Cruiser";
    this->id = id;
    this->shell = 250;
    this->size = 8;
    this->damage = 50;
}

void Cruiser::attack(Ship &enemy) {
    int rnd = rand() % 10;
    int dist = 0;
    if (rnd >= (enemy.getSize())) {
        std::cout << "Attack successful." << std::endl;
        dist = this->getDistance(enemy);
        enemy.takeDamage(this->damage - dist);
        attack(enemy);
    } else {
        std::cout << "Attack failed." << std::endl;
    }
}

Cruiser::~Cruiser() = default;
