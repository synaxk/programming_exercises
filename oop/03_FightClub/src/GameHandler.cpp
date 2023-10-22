//        std::cout << "on himself";

// Created by alex on 30.03.22.
//

#include "../headers/GameHandler.h"

GameHandler::GameHandler() {
    this->abilities[0] = new Ability("Faustschlag", "Faustschlag auf den Kopf", -1);
    this->abilities[1] = new Ability("Tritt", "Tritt den Gegner", -1);
    this->abilities[2] = new Ability("Wut", "Nächster schlag wird ignoriert", +1);
    this->abilities[3] = new Ability("Gebet", "Heilung des letzten Schadens", +1);

    this->fighters[0] = new Human("RudiDerRüde");
    this->fighters[1] = new OrcFighter("ThorstenMitBorsten", "Frostwolf");
    this->fighters[2] = nullptr;
}

int GameHandler::startGame() {
    std::string input;
    while (!fight.end()) {
        this->fight.startRound();
    }
    return 1;
}

int GameHandler::initGame() {
    this->fight = Fight(chooseFighter(), chooseFighter());
    return 1;
}

Fighter* GameHandler::chooseFighter() {
    Fighter *fighter = nullptr;
    std::string input;

    listFighters();
    std::cout << "Select Fighter by name: ";
    input = getInput();

    for (int i = 0; i < 3; i++) {
        if (fighters[i] != nullptr) {
            if (input == fighters[i]->getName()) {
                fighter = fighters[i];
            }
        }

    }
    fighter->setAbility(0, chooseAbility(0));
    fighter->setAbility(1, chooseAbility(1));
    return fighter;
}

Ability* GameHandler::chooseAbility(int index) {
    Ability *ability = nullptr;
    std::string input;
    std::string order = "first";
    listAbilities();
    if (index == 1) {
        order = "healing";
    }
    std::cout << "Select " << order << " ability by name: ";
    input = getInput();

    for (int i = 0; i < 4; i++) {
        if (input == abilities[i]->getName()) {
            ability = abilities[i];
        }
    }
    return ability;
}

std::string GameHandler::getInput() {
    std::string input;
    std::cin >> input;
    return input;
}

void GameHandler::listFighters() {
    std::cout << "Fighters" << std::endl;
    for (int i = 0; i < 3; i++) {
        if (fighters[i] != nullptr) {
            std::cout << fighters[i]->getName() << std::endl;
        }
    }
}

void GameHandler::listAbilities() {
    std::cout << "Ability\t\t\tDescription" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << abilities[i]->getName() << "\t\t" << abilities[i]->getDescription() << std::endl;
    }
}

