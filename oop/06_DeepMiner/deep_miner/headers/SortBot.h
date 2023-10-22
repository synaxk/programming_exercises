//
// Created by alex on 14.04.22.
//

#ifndef INC_04_DEEPMINER_SORTBOT_H
#define INC_04_DEEPMINER_SORTBOT_H


#include "Bot.h"
class SortBot : public Bot {
public:
    explicit SortBot(int id, Block* block);
    ~SortBot();

    void mine(std::array<std::array<std::mutex,5>,5> &mutexes) override;
};



#endif //INC_04_DEEPMINER_SORTBOT_H
