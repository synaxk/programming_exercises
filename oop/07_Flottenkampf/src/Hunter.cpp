//
// Created by alex on 21.05.22.
//

#include "../headers/Hunter.h"

Hunter::Hunter(int id, std::array<std::array<int, 10>, 20> &grid) : Ship(grid) {
    this->name = "Hunter";
    this->id = id;
    this->shell = 75;
    this->size = 4;
    this->damage = 30;
}

void Hunter::attack(Ship &enemy) {
    int rnd = rand() % 10;
    int dist = this->getDistance(enemy);
    if (rnd >= (enemy.getSize())) {
        if (rnd >= 8) {
            std::cout << "Attack successful - critical hit!" << std::endl;
            return enemy.takeDamage((this->damage - dist)*2);
        }
        std::cout << "Attack successful" << std::endl;
        enemy.takeDamage(this->damage - dist);
    } else {
        std::cout << "Attack failed." << std::endl;
    }
}

Hunter::~Hunter() = default;

