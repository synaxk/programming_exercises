//
// Created by alex on 30.03.22.
//

#include "../headers/Human.h"

Human::Human() = default;

Human::Human(std::string name) : Fighter(name, 5){

}

void Human::react() {
    std::cout << "Aua, das tut weh!" << std::endl;
}