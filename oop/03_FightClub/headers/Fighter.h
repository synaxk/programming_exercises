//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_FIGHTER_H
#define INC_03_FIGHTCLUB_FIGHTER_H
#include <string>

#include "Ability.h"
class Fighter {
public:
    Fighter();
    Fighter(std::string name, int health);

    void setAbility(int index, Ability *ability);

    std::string getName();
    int changeHealth(int value);
    int getHealth();

    Ability *abilities[2];
    virtual void react();

protected:
    std::string name;
    int health;
};


#endif //INC_03_FIGHTCLUB_FIGHTER_H
