//
// Created by alex on 14.04.22.
//

#ifndef INC_04_DEEPMINER_GAMEHANDLER_H
#define INC_04_DEEPMINER_GAMEHANDLER_H

#include "Block.h"
#include "Bot.h"
#include "GreedyBot.h"
#include "SearchBot.h"
#include "SortBot.h"
#include <mutex>
#include <thread>
#include <stdexcept>

class GameHandler {
public:
    GameHandler();
    ~GameHandler();
    void init();
    void start();
    void play();
private:
    std::array<Bot*, 5> bots{};
    Block block;
    std::array<std::array<std::mutex, 5>, 5> mutexes;
    std::array<std::thread, 5> threads;
};


#endif //INC_04_DEEPMINER_GAMEHANDLER_H
