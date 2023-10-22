//
// Created by alex on 30.03.22.
//

#include "../headers/OrcFighter.h"

OrcFighter::OrcFighter() = default;

OrcFighter::OrcFighter(std::string name, std::string clan) : Fighter(name, 5){
    this->clan = clan;
}

void OrcFighter::react() {
    std::cout << "Arrghhh URGHA!!!" << std::endl;
}