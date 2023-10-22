//
// Created by alex on 09.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_TESTHANDLER_H
#define INC_02_OASENCRAWLER_OOP_TESTHANDLER_H

#include "Test.h"
#include "Field.h"
#include "Player.h"
#include "InputHandler.h"
#include "World.h"

class TestHandler {
public:
    TestHandler(int size);
    int run();

private:
    int worldSize;
    Field testField;
    Player testPlayer;
    World  testWorld;
};


#endif //INC_02_OASENCRAWLER_OOP_TESTHANDLER_H
