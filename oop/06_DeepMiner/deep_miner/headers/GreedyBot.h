//
// Created by alex on 14.04.22.
//

#ifndef INC_04_DEEPMINER_GREEDYBOT_H
#define INC_04_DEEPMINER_GREEDYBOT_H

#include "Bot.h"

class GreedyBot : public Bot {
public:
    explicit GreedyBot(int id, Block *block);
    ~GreedyBot();

    void mine(std::array<std::array<std::mutex,5>,5> &mutexes) override;
};


#endif //INC_04_DEEPMINER_GREEDYBOT_H
