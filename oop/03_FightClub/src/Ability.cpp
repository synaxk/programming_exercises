//
// Created by alex on 30.03.22.
//

#include "../headers/Ability.h"

Ability::Ability() = default;

Ability::Ability(std::string name, std::string description, int effect) {
    this->name = name;
    this->description = description;
    this->effect = effect;
}

std::string Ability::getName() {
    return this->name;
}

std::string Ability::getDescription() {
    return this->description;
}

int Ability::execute() {
    std::cout << "used ability '" << name << "' ";
    return this->effect;
}