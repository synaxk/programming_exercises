//
// Created by alex on 30.03.22.
//

#include "../headers/Fighter.h"

Fighter::Fighter() = default;

Fighter::Fighter(std::string name, int health) {
    this->name = name;
    this->health = health;
    this->abilities[0] = nullptr;
    this->abilities[1] = nullptr;
}

std::string Fighter::getName() {
    return this->name;
}

void Fighter::setAbility(int index, Ability *ability) {
    this->abilities[index] = ability;
}

int Fighter::changeHealth(int value) {
    this->health += value;
    return this->health;
}

int Fighter::getHealth() {
    return this->health;
}

void Fighter::react() {
    std::cout << "Aua!" << std::endl;
}
