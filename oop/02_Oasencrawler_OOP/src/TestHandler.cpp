//
// Created by alex on 09.03.22.
//

#include "../headers/TestHandler.h"
#include "../headers/World.h"

TestHandler::TestHandler(int size) {
    this->testField = Field();
    this->worldSize = size;
    this->testWorld = World(size);
    this->testPlayer = Player(&worldSize);


}

int TestHandler::run() {
    int size = 0;
    bool success = true;
    Test tests[] = {
            {(char *) "World -> Info      ",      (bool) testWorld.info()},
            {(char *) "World -> InitGrid  ",      (bool) testWorld.initGrid()},
            {(char *) "World -> InitPlayer",      (bool) testWorld.initPlayer()},
            {(char *) "World -> LevelUp   ",      (bool) testWorld.levelUp()},
            {(char *) "World -> SpawnRelic",      (bool) testWorld.spawnRelic()},
            {(char *) "World -> DrawGrid  ",     (bool) testWorld.drawGrid()},
            {(char *) "World -> PrintStat ", (bool) testWorld.printStatistics()},
            {(char *) "Field -> Init      ",            (bool) testField.init()},
            {(char *) "Field -> Clear     ",           (bool) testField.clear()},
            {(char *) "Player -> GetHealth",      (bool) testPlayer.getHealth()},
            {(char *) "Player -> GetRelics",      (bool) !testPlayer.getRelics()},
            {(char *) "Player -> AddRelic ",       (bool) testPlayer.addRelic()},
            {(char *) "Player -> Heal     ",           (bool) testPlayer.heal()},
            {(char *) "Player -> Damage   ",         (bool) testPlayer.damage(1)},
            {(char *) "Player -> Buff     ",           (bool) testPlayer.toggleBuff()},
            {(char *) "Player -> Debuff   ",         (bool) testPlayer.toggleDeBuff()},
            {(char *) "Player -> Weapon   ",         (bool) !testPlayer.toggleWeapon()},
    };
    size = sizeof(tests) / sizeof(Test);

    for (int i = 0; i < size; i++) {
        if (!tests[i].test()) {
            success = false;
        }
    }
    printf("\nTests executed %s\n", success ? "successfully." : "with errors.");
    sleep(4);
    return 1;
}