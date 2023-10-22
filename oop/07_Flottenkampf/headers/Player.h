//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_PLAYER_H
#define FLOTTENKAMPF_PLAYER_H

#include <string>
#include <vector>
#include "Ship.h"
#include <iostream>
#include "InputHandler.h"
#include "Hunter.h"
#include "Destroyer.h"
#include "Cruiser.h"
#include <array>


class Player {
public:
    Player(std::array<std::array<int, 10>, 20> &grid);
    Player(std::string name, bool player, std::array<std::array<int, 10>, 20> &grid);
    ~Player();
    void initFleet();
    Ship* selectShip(int id);
    std::vector<std::string> getShipIds();
    std::string getName();
    std::vector<Ship*> getFleet();
    void printFleet();
    void removeDestroyedShips();
private:
    Ship* initShip(int id);
    std::string name;
    std::vector<Ship*> fleet;
    std::array<std::array<int, 10>, 20>& grid;
    bool player;
};


#endif //FLOTTENKAMPF_PLAYER_H
