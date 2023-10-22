//
// Created by alex on 01.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_WORLD_H
#define INC_02_OASENCRAWLER_OOP_WORLD_H
#include "Player.h"
#include "Field.h"
#include "utils.h"
#include "InputHandler.h"
#include "Enemy.h"
#include "Bullet.h"

class World {
friend class TestHandler;
public:
    World(int size);
    World();
    int play();
    int info();
    int levelUp();

private:
    int initGrid();
    int initPlayer();
    int drawGrid();
    int printStatistics();
    int spawnEnemy();
    int resetField(struct position position);
    int chasePlayer();
    int shootEnemy();
    int clear();
    int level;
    int action();
    int spawnRelic();
    int gridSize;
    int relics;
    Field grid[10][10];
    Player player;
    Enemy monster;
};


#endif //INC_02_OASENCRAWLER_OOP_WORLD_H
