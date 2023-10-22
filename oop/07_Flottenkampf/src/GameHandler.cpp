//
// Created by alex on 21.05.22.
//

#include "../headers/GameHandler.h"

GameHandler::GameHandler(){
    this->initGrid();
}

void GameHandler::start() {
    this->players[0] = initPlayer(false);
    this->players[1] = initPlayer(true);
    this->printStats();
    this->play();
}

Player* GameHandler::initPlayer(bool playerR) {
    std::cout << "Player name: ";
    std::string name;
    while (name.empty()) {
        try {
            name = InputHandler::read();
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    return new Player(name, playerR, this->grid);
}

void GameHandler::playerAction(bool player) {
    std::cout << "Player: " << this->players[player]->getName() << std::endl;
    std::cout << "Select ship: " << std::endl;
    std::string ship, target;

    while (ship.empty()) {
        try {
            ship = InputHandler::read(this->players[!player]->getShipIds());
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "Select target: ";

    while (target.empty()) {
        try {
            target = InputHandler::read(this->players[!player]->getShipIds());
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    try {
        this->players[player]->selectShip(std::stoi(ship))->attack(
                *(this->players[!player]->selectShip(std::stoi(target))));
    } catch (ShipIsDeadException &e) {

    }

    this->players[!player]->removeDestroyedShips();

}

void GameHandler::play() {
    for (int i = 0; !players[i%2]->getFleet().empty() && !players[(i+1)%2]->getFleet().empty(); i++) {
        this->playerAction(i%2);
        this->printStats();
        this->printGrid();
    }
    std::cout << "Game Over" << std::endl;
}

void GameHandler::printStats() {
    this->players[0]->printFleet();
    std::cout << std::endl;
    this->players[1]->printFleet();
    std::cout << std::endl;
}

void GameHandler::printGrid() {
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            if (i == 10) {
                std::cout << " - ";
            } else if (i >= 10){
                if (grid[i-1][j] == -1) {
                    std::cout << " □ ";
                } else {
                    std::cout << " " << grid[i-1][j] << " ";
                }
            } else {
                if (grid[i][j] == -1) {
                    std::cout << " □ ";
                } else {
                    std::cout << " " << grid[i][j] << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

void GameHandler::initGrid() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = -1;
        }
    }
}

GameHandler::~GameHandler() {
    delete players[0];
    delete players[1];
}
