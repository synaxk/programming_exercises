//
// Created by alex on 21.05.22.
//

#include "../headers/Destroyer.h"

Destroyer::Destroyer(int id, std::array<std::array<int, 10>, 20> &grid) : Ship(grid) {
    this->name = "Destroyer";
    this->id = id;
    this->shell = 150;
    this->size = 6;
    this->damage = 60;
}

void Destroyer::attack(Ship &enemy) {
    int rnd = rand() % 10;
    int dist = this->getDistance(enemy);
    if (rnd >= (enemy.getSize()-2)) {
        std::cout << "Attack successful." << std::endl;
        enemy.takeDamage(this->damage - dist);
    } else {
        std::cout << "Attack failed." << std::endl;
    }
}

Destroyer::~Destroyer() = default;
