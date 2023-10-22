//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_ABILITY_H
#define INC_03_FIGHTCLUB_ABILITY_H
#include <string>
#include <iostream>
class Ability {
public:
    Ability();
    Ability(std::string name, std::string description, int effect);
    std::string getName();
    std::string getDescription();
    int execute();
private:
    std::string name;
    std::string description;
    int effect;
};


#endif //INC_03_FIGHTCLUB_ABILITY_H
