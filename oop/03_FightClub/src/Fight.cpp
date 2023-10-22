//
// Created by alex on 30.03.22.
//

#include "../headers/Fight.h"

Fight::Fight() = default;

Fight::Fight(Fighter *fighter1, Fighter *fighter2) {
    this->round = 0;
    this->fighter1 = fighter1;
    this->fighter2 = fighter2;
}

void Fight::startRound() {
    int rnd = rand() % 10;

    this->round++;
    std::cout << "Round: " << round << std::endl <<
    fighter1->getName() << "\tHP: " << fighter1->getHealth() << std::endl <<
    fighter2->getName() << "\tHP: " << fighter2->getHealth() << std::endl << std::endl;

    ///random fighter starts
    if (rnd < 5) {
        fighterAction(fighter1, fighter2);
        fighterAction(fighter2, fighter1);
    } else {
        fighterAction(fighter2, fighter1);
        fighterAction(fighter1, fighter2);
    }
}

void Fight::fighterAction(Fighter *fighter, Fighter *enemy) {
    int rnd = rand() % 10;
    if ((fighter->getHealth() < enemy->getHealth()) && rnd < 3) {
        ///selfheal
        std::cout << "Fighter " << fighter->getName() << " ";
        fighter->changeHealth(fighter->abilities[1]->execute());
        std::cout << "on himself" << std::endl;
    } else {
        ///hit enemy
        std::cout << "Fighter " << fighter->getName() << " ";
        enemy->changeHealth(fighter->abilities[0]->execute());
        std::cout << "on " << enemy->getName() << std::endl;
        std::cout << enemy->getName() << ": ";
        enemy->react();
        std::cout << std::endl;
    }
}

bool Fight::end() {
    if (fighter1->getHealth() < 1 || fighter2->getHealth() < 1) {
        return true;
    }
    return false;
}
