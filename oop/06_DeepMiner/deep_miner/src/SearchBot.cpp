//
// Created by alex on 14.04.22.
//

#include "../headers/SearchBot.h"
SearchBot::~SearchBot() = default;

SearchBot::SearchBot(int id, Block *block) : Bot(id, block) {

}

void SearchBot::mine(std::array<std::array<std::mutex,5>,5> &mutexes){

}
