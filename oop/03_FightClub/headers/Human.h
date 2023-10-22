//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_HUMAN_H
#define INC_03_FIGHTCLUB_HUMAN_H
#include "Fighter.h"

class Human : public Fighter {
public:
    Human();
    Human(std::string name);
    void react() override;

private:

};


#endif //INC_03_FIGHTCLUB_HUMAN_H
