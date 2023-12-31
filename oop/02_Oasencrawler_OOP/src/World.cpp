//
// Created by alex on 01.03.22.
//

#include "../headers/World.h"

#define STAT_POS 20

#include <unistd.h>

World::World(int size) {
    this->gridSize = size;
    this->relics = 0;
    this->level = 1;
}

int World::play() {
    this->clear();
    this->initGrid();
    this->drawGrid();
    this->initPlayer();
    this->printStatistics();
    this->monster.kill();

    InputHandler input = InputHandler((char *) "wasdqxy");
    input.setICanonMode();
    char c;
    while ((c = (char) input.read()) != 'q') {
        if (c == 'y') {
            this->relics = 0;
            this->initGrid();
            this->drawGrid();
            this->initPlayer();
            this->monster.kill();
        }

        if (c == 'x') {
            this->shootEnemy();
        }

        this->player.move(c);
        this->action();

        if (player.getHealth() < 1) {
            printf("\nType y to restart, q to quit:");
        }

        if (player.getRelics() == this->relics) {
            printf("Type y to continue to next level, q to quit:\n");
            this->levelUp();
        }
    }
    return 1;
}

/**initialize Grid*/
int World::initGrid() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (i == 0 && j == 0) {
                continue; /// skip player position
            }
            this->grid[i][j].init();

            if (grid[i][j].type == relic) {
                this->relics++;
            }
        }
    }

    if (this->relics == 0) {
        spawnRelic();
    }
    return 1;
}

/**spawn relic*/
int World::spawnRelic() {
    int x, y = 0;
    x = rand() % gridSize;
    y = rand() % gridSize;
    this->grid[y][x].type = relic;

    return 1;
}

int World::clear() {
    printf("\033[2J");
    /*reset cursor*/
    printf("\033[1;1H");
    return 1;
}

int World::drawGrid() {
    this->clear();
    for (int i = 0; i < gridSize + 2; i++) {
        for (int j = 0; j < gridSize + 2; j++) {
            /*drawGrid border*/
            if (i == 0 || i == gridSize + 1) {
                std::cout << '-';
                if (j == gridSize + 1) {
                    std::cout << "         " << std::endl;
                }
            } else if ((i > 0 && i < gridSize + 2) && (j == 0 || j == gridSize + 1)) {
                std::cout << '|';
                if (j == gridSize + 1) {
                    std::cout << "          " << std::endl;
                }
            } else {
                /*drawGrid game grid */
                switch (grid[i - 1][j - 1].type) {
                    case empty:
                        std::cout << ' ';
                        break;
                    case danger:
                        std::cout << 'X';
                        break;
                    case well:
                        std::cout << 'O';
                        break;
                    case relic:
                        std::cout << 'i';
                        break;
                    default:
                        std::cout << 'n';
                        return 0;
                }
            }
        }
    }
    return 1;
}

int World::levelUp() {
    return ++this->level;
}

int World::initPlayer() {
    this->player = Player(&this->gridSize);
    ///set cursor to player position
    printf("\033[2;2H");
    printf("\033[2 q\033]12;white\a");
    return 1;
}

int World::action() {
    int rnd;
    int health = 0;
    struct position pos = player.getPosition();
    if (this->monster.getHealth() > 0) {
        this->chasePlayer();
    }
    switch (grid[pos.y][pos.x].type) {
        case empty:
            break;
        case danger:
            rnd = rand() % 6;
            if (rnd == 1) {
                player.damage(this->level);
            }
            break;
        case well:
            health = player.heal();
            if (health == 5) {
                rnd = rand() % 10;
                if (rnd == 9) {
                    player.toggleWeapon();
                } else if (rnd > 4) {
                    player.toggleBuff();
                }

            }
            break;
        case relic:
            player.addRelic();
            if (this->monster.getHealth() == 0) {
                this->spawnEnemy();
            }
            break;
    }
    grid[pos.y][pos.x].clear();
    printf("\033[C\b \033[D");
    this->printStatistics();
    return 0;
}

int World::spawnEnemy() {
    this->monster = Enemy();
    this->monster.spawn(this->level);
    return 1;
}

int World::shootEnemy() {
    int distance_x, distance_y;

    struct position playerPos = this->player.getPosition();
    struct position monsterPos = this->monster.getPosition();
    Bullet bullet = Bullet(this->level, playerPos);
    struct position bulletPos = bullet.getPosition();

    distance_x = playerPos.x - monsterPos.x;
    distance_y = playerPos.y - monsterPos.y;

    if (distance_x == 0) {
        if (distance_y < 0) {
            ///shoot down
            /**spawn bullet*/
            bulletPos.y++;
            bullet.setPosition(bulletPos);
            bullet.move();
            for (;bulletPos.y < monsterPos.y; ) {
                setbuf(stdout, NULL);
                usleep(100000);
                resetField(bulletPos);
                bulletPos.y++;
                bullet.setPosition(bulletPos);
                bullet.move();
            }
        } else if (distance_y > 0) {
            ///shoot up
            /**spawn bullet*/
            bulletPos.y--;
            bullet.setPosition(bulletPos);
            bullet.move();
            for (;bulletPos.y > monsterPos.y; ) {
                setbuf(stdout, NULL);
                usleep(100000);
                resetField(bulletPos);
                bulletPos.y--;
                bullet.setPosition(bulletPos);
                bullet.move();
            }
        }
    } else if (distance_y == 0) {
        if (distance_x < 0) {
            ///right
            /**spawn bullet*/
            bulletPos.x++;
            bullet.setPosition(bulletPos);
            bullet.move();
            for (;bulletPos.x < monsterPos.x; ) {
                setbuf(stdout, NULL);
                usleep(100000);
                resetField(bulletPos);
                bulletPos.x++;
                bullet.setPosition(bulletPos);
                bullet.move();
            }
        } else if ( distance_x > 0) {
            ///left
            /**spawn bullet*/
            bulletPos.x--;
            bullet.setPosition(bulletPos);
            bullet.move();
            for (;bulletPos.x < monsterPos.x; ) {
                setbuf(stdout, NULL);
                usleep(100000);
                resetField(bulletPos);
                bulletPos.x--;
                bullet.setPosition(bulletPos);
                bullet.move();
            }
        }
        resetField(bulletPos);
        this->monster.damage(1);
    }
    return 1;
}

int World::resetField(struct position position) {
    char c;
    struct position pos = position;
    switch (this->grid[pos.y][pos.x].type) {
        case empty:
            c = ' ';
            break;
        case danger:
            c = 'X';
            break;
        case well:
            c = 'O';
            break;
        case relic:
            c = 'i';
            break;
        default:
            return 0;
    }
    //save cursor position
    printf("\033[s");
    //move cursor to monster
    printf("\033[%d;%dH", pos.y + 2, pos.x + 2);
    //remove monster
    printf("\033[C\b%c\033[D", c);
    //restore cursor position
    printf("\033[u");
    return 1;
}


int World::chasePlayer() {
    int distance_x, distance_y;
    struct position playerPos = this->player.getPosition();
    struct position monsterPos = this->monster.getPosition();

    distance_x = playerPos.x - monsterPos.x;
    distance_y = playerPos.y - monsterPos.y;

    if (distance_y == 0 && distance_x == 0) {
        resetField(this->monster.getPosition());
        player.damage(monster.getHealth());
        monster.kill();
        return 1;
    }

    this->resetField(this->monster.getPosition());
    ///move on x or y
    if ((std::abs(distance_x) >= std::abs(distance_y)) && distance_y != 0) {
        ///move on y
        if (distance_y > 0) {
            ///down
            monsterPos.y++;
        } else if (distance_y < 0) {
            ///up
            monsterPos.y--;
        }
    } else if (((std::abs(distance_x) <= std::abs(distance_y)) && distance_x != 0)|| (distance_x != 0 && distance_y == 0)) {
        ///move on x
        if (distance_x > 0) {
            ///right
            monsterPos.x++;
        } else if (distance_x < 0) {
            ///left
            monsterPos.x--;
        }
    }
    this->monster.setPosition(monsterPos);
    this->monster.move();
    return 1;
}

int World::printStatistics() {
    //save cursor position
    printf("\033[s");
    //move cursor to STAT_POS and update
    printf("\033[1;%dH", STAT_POS);
    printf("02_Oasencrawler Level %d", this->level);
    printf("\033[2;%dH", STAT_POS);
    printf("HP: %d/5", player.getHealth());
    printf("\033[3;%dH", STAT_POS);
    printf("Relics: %d/%d", player.getRelics(), this->relics);
    printf("\033[4;%dH", STAT_POS);
    printf("Damage taken: %d", player.damageTaken);
    printf("\033[5;%dH", STAT_POS);
    printf("Healing done: %d", player.healingDone);
    printf("\033[6;%dH", STAT_POS);
    printf("Player %d|%d", player.getPosition().x, player.getPosition().y);

    if (this->monster.getHealth() > 0) {
        printf("\033[7;%dH", STAT_POS);
        printf("Monster %d|%d\t%d", monster.getPosition().x, monster.getPosition().y), monster.getHealth();
    }
    printf("\033[8;%dH", STAT_POS);
    printf("\033[1;33m\u2588\033[1;37m\t Buff - You are stronger");
    printf("\033[9;%dH", STAT_POS);
    printf("\033[0;31m\u2588\033[1;37m\t Debuff - Next danger field...");
    printf("\033[10;%dH", STAT_POS);
    printf("\033[0;34m\u2588\033[1;37m\t You found a Gun, press X to shoot");
    //restore cursor position
    printf("\033[u");

    if (player.getHealth() < 1) {
        printf("\033[12;%dH", STAT_POS);
        printf("GAME OVER\n\n\n");
    }
    if (this->relics == player.getRelics()) {
        printf("\033[12;%dH", STAT_POS);
        printf("Level Completed!\v\n");
    }
    return 1;
}

int World::info() {
    this->clear();
    int logo[] = {
            0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x0A, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5C, 0x20, 0x5F, 0x5F,
            0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F,
            0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20,
            0x20, 0x5C, 0x5C, 0x5F, 0x5F, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F,
            0x2F, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20,
            0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x2F, 0x20, 0x20, 0x20,
            0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x5C, 0x5F, 0x5C, 0x5F,
            0x5F, 0x5F, 0x20, 0x5C, 0x5C, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x20, 0x20, 0x20, 0x7C,
            0x20, 0x20, 0x5C, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x0A,
            0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x28, 0x5F, 0x5F, 0x5F, 0x5F, 0x20,
            0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x3E, 0x5C, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x3E,
            0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
            0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x5C, 0x5F, 0x20, 0x20,
            0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
            0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x20,
            0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x0A,
            0x2F, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x5C, 0x2F, 0x5C, 0x5F, 0x20, 0x20, 0x5F, 0x5F,
            0x20, 0x5C, 0x5F, 0x5F, 0x20, 0x20, 0x5C, 0x5C, 0x20, 0x5C, 0x2F, 0x20, 0x5C, 0x2F, 0x20, 0x2F,
            0x20, 0x20, 0x7C, 0x20, 0x5F, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x5C, 0x5F, 0x20, 0x20, 0x5F, 0x5F,
            0x20, 0x5C, 0x20, 0x0A, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x7C,
            0x20, 0x20, 0x7C, 0x20, 0x5C, 0x2F, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x5C, 0x5C, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x2F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x5C, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C,
            0x20, 0x20, 0x7C, 0x20, 0x5C, 0x2F, 0x20, 0x0A, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
            0x20, 0x20, 0x2F, 0x7C, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x28, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20,
            0x2F, 0x5C, 0x2F, 0x5C, 0x5F, 0x2F, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F,
            0x5F, 0x20, 0x20, 0x3E, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
            0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x5F,
            0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x5F,
            0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x2F,
            0x20, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20,
            0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
            0x5F, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x5C, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x20,
            0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x5C, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A
    };

    int size = 0;
    size = sizeof(logo) / sizeof(int);

    for (int i = 0; i < size; i++) {
        printf("%c", (char) logo[i]);
    }

    printf("\n\t\tLoading...\n");
    printf("\n");
    setbuf(stdout, NULL);
    for (int i = 0; i < 50; i++) {
        putchar('#');
        usleep(80000);
    }

    printf("\n\n");

    sleep(1);
    return 1;
}

World::World() {

}