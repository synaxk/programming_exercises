//
// Created by alex on 14.04.22.
//

#include "../headers/GameHandler.h"

GameHandler::GameHandler() {
    this->block = Block();
}

void GameHandler::init() {
    this->bots[0] = new GreedyBot(1, &block);
    this->bots[1] = new GreedyBot(2, &block);
    this->bots[2] = new GreedyBot(3, &block);
    this->bots[3] = new GreedyBot(4, &block);
    this->bots[4] = new SortBot(5, &block);
    this->block.printBlock();
}

void GameHandler::start() {
    while (!this->block.checkIfBlockEmpty()) {
        this->play();
    }
}

void GameHandler::play() {
    for (size_t i = 0; i < bots.size(); i++) {
        threads[i] = std::thread(&Bot::mine, bots[i], std::ref(mutexes));
    }
    for (std::thread &th: threads) {
        th.join();
    }
    this->block.printBlock();
}


GameHandler::~GameHandler() {

}

