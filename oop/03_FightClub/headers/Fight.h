//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_FIGHT_H
#define INC_03_FIGHTCLUB_FIGHT_H
#include "Fighter.h"

class Fight {
public:
    Fight();
    Fight(Fighter *fighter1, Fighter *fighter2);
    void startRound();
    void fighterAction(Fighter *fighter, Fighter *enemy);
    void nextRound();
    bool end();
private:
    int round;
    Fighter *fighter1;
    Fighter *fighter2;
};


#endif //INC_03_FIGHTCLUB_FIGHT_H
