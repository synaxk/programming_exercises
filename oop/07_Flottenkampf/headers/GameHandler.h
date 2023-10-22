//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_GAMEHANDLER_H
#define FLOTTENKAMPF_GAMEHANDLER_H

#include "Player.h"
#include "InputHandler.h"
#include <array>

class GameHandler {
public:
    GameHandler();
    ~GameHandler();
    void start();
    void printGrid();

private:
    Player* initPlayer(bool player);
    void playerAction(bool player);
    void initGrid();
    void play();
    void printStats();
    Player* players[2];
    std::array<std::array<int, 10>, 20> grid;
};


#endif //FLOTTENKAMPF_GAMEHANDLER_H
