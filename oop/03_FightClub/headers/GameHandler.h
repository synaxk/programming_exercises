//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_GAMEHANDLER_H
#define INC_03_FIGHTCLUB_GAMEHANDLER_H
#include "Fight.h"

#include "Fighter.h"
#include "Ability.h"
#include "OrcFighter.h"
#include "Human.h"
#include <iostream>

class GameHandler {
public:
    GameHandler();
    std::string getInput();
    int startGame();
    int initGame();

    //Fighter* createNewFighter();
    Fighter* chooseFighter();
    Ability* chooseAbility(int index);

    void listFighters();
    void listAbilities();

private:
    Fight fight;
    Fighter* fighters[3];
    Ability* abilities[4];
};


#endif //INC_03_FIGHTCLUB_GAMEHANDLER_H
