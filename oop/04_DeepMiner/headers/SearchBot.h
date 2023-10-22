//
// Created by alex on 14.04.22.
//

#ifndef INC_04_DEEPMINER_SEARCHBOT_H
#define INC_04_DEEPMINER_SEARCHBOT_H

#include "Bot.h"
class SearchBot : public Bot {
public:
    explicit SearchBot(int id, Block* block);
    ~SearchBot();

    void mine(std::array<std::array<std::mutex,5>,5> &mutexes) override;
};

#endif //INC_04_DEEPMINER_SEARCHBOT_H
