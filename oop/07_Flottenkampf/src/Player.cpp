//
// Created by alex on 21.05.22.
//

#include "../headers/Player.h"

#include <utility>


Player::Player(std::array<std::array<int, 10>, 20> &grid) : grid(grid) {
    this->player = true;
}

Player::Player(std::string name, bool player, std::array<std::array<int, 10>, 20> &grid) : grid(grid) {
    this->name = std::move(name);
    this->grid = grid;
    this->player = player;
    this->initFleet();
}

void Player::initFleet() {
    ///select up to 9 ships
    for (int i = 0; i < 9; i++) {
        Ship *ship = initShip(i);
        if (ship == nullptr) {
            break;
        }

        fleet.push_back(ship);
    }
}

Ship *Player::initShip(int id) {
    Ship *newShip = nullptr;
    std::cout << "Select Ship Type: " << std::endl << "1 ... Hunter" << std::endl;
    std::cout << "2 ... Destroyer" << std::endl << "3 ... Cruiser" << std::endl << "0 ... stop"<< std::endl;
    std::string type;
    while (type.empty()) {
        try {
            type = InputHandler::read({"0","1", "2", "3"});
        } catch (std::invalid_argument &e) {
            std::cout << e.what();
        }
    }

    switch (stoi(type)) {
        case 0:
            return nullptr;
        case 1:
            newShip = new Hunter(id, this->grid);
            break;
        case 2:
            newShip = new Destroyer(id, this->grid);
            break;
        case 3:
            newShip = new Cruiser(id, this->grid);
            break;
        default:
            throw std::runtime_error("hm ka");
    }
    newShip->spawn(this->player);
    return newShip;
}

Ship *Player::selectShip(int id) {
    std::vector<Ship *>::iterator it;

    it = std::find_if(this->fleet.begin(), this->fleet.end(),
                      [&id](Ship *ship) {
                          if (ship->getID() == id) return true;
                          return false;
                      });

    if (it != this->fleet.end()) {
        return *it;
    }
    return nullptr;
}

std::vector<std::string> Player::getShipIds() {
    std::vector<std::string> ids;
    auto it = this->fleet.begin();
    for (;it < this->fleet.end(); it++) {
        ids.push_back(std::to_string((*it)->getID()));
    }
    return ids;
}

void Player::printFleet() {
    std::cout << "Player: " << this->name << std::endl;
    for (auto it = this->fleet.begin(); it < this->fleet.end(); it++) {
        (*it)->print();
    }
}

void Player::removeDestroyedShips() {
    this->fleet.erase(
            std::remove_if(this->fleet.begin(), this->fleet.end(),
                           [](Ship *ship) {
                                if(ship->getShell() <= 0) {
                                    std::cout << "Ship " << ship->getID() << " " << ship->getName() << " destroyed." << std::endl;
                                    return true;
                                } else {
                                    return false;}}),
                      this->fleet.end());
}

std::string Player::getName() {
    return this->name;
}

std::vector<Ship*> Player::getFleet() {
    return this->fleet;
}



Player::~Player() = default;
