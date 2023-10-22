#include <iostream>
#include <cassert>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define GRID_HEIGHT 5
#define GRID_WIDTH 10
#define STAT_POS GRID_WIDTH * 2

enum fieldType {
    empty, danger, well, relic
};

struct position {
    int x;
    int y;
};

struct player {
    int hp;
    int relics;
    int damageTaken;
    int healing;
    struct position position;
};

struct monster {
    bool alive;
    struct position position;
};

struct world {
    int grid[GRID_HEIGHT][GRID_WIDTH];
    int relics;
    struct player player;
    struct monster monster;
};

void initWorld(struct world *world);

void initPlayer(struct player *player);

enum fieldType initField();

void clearField(struct world *world);

void resetField(struct world *world);

void draw(struct world *world);

void updateStatistics(struct world *world);

int play(struct world *world);

void spawnMonster(struct world *world);

void moveMonster(struct world *world);

void chasePlayer(struct world *world);

int main() {
    /*initialize random seed: */
    srand(time(NULL));
    struct world world;

    /*clear console*/
    printf("\033[2J");
    /*reset cursor*/
    printf("\033[1;1H");

    initWorld(&world);
    draw(&world);

    play(&world);

    /**tests*/
    assert("init world:" && (world.relics > 0));


    return 0;
}

/**start the game
 * 1. disable the input buffer to move the cursor without hitting enter
 * 2. reset terminal
 * 3. loop -> valid input (w,a,s,d) to move, q to quit
 *    move cursor on success, update player position -> else beep
 * 4. Check field-type of new player position for action
 * 5. Update Game statistics
 * 6. Clear field*/
int play(struct world *world) {
    int rnd = 0;
    /*skip input buffer*/
    // change terminal input to cannonical mode
    static struct termios oldt, newt;
    // write the settings of STDIN to oldt
    tcgetattr(STDIN_FILENO, &oldt);
    //copy the settings to newt
    newt = oldt;
    //turn off icanon and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    //change STDIN settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);



    /*move cursor to start*/
    printf("\033[2;2H");

    /*movement*/
    for (int c; (c = getchar()) != 'q';) {
        switch (c) {
            case 'w':
                if (world->player.position.y > 0) {
                    printf("\033[A"); // 033 = ESC, [, A = UP
                    world->player.position.y--;
                } else {
                    putchar('\a');
                }
                break;
            case 'a':
                if (world->player.position.x > 0) {
                    printf("\033[D"); // 033 = ESC, [, D = LEFT
                    world->player.position.x--;
                } else {
                    putchar('\a');
                }
                break;
            case 's':
                if (world->player.position.y < GRID_HEIGHT - 1) {
                    printf("\033[B"); // 033 = ESC, [, B = DOWN
                    world->player.position.y++;
                } else {
                    putchar('\a');
                }
                break;
            case 'd':
                if (world->player.position.x < GRID_WIDTH - 1) {
                    printf("\033[C"); // 033 = ESC, [, C = RIGHT
                    world->player.position.x++;
                } else {
                    putchar('\a');
                }
                break;
            case 'q':
            default:
                //restore STDIN settings
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return 0;
        }
        if (world->monster.alive) {
            chasePlayer(world);
        }

        /**check new field type -> handle events*/
        switch (world->grid[world->player.position.y][world->player.position.x]) {
            case empty:
                //nothing
                break;
            case danger:
                rnd = rand() % 6;
                if (rnd == 1) {
                    world->player.hp--;
                    world->player.damageTaken++;
                }
                if (world->player.hp == 0) {
                    updateStatistics(world);
                    //restore STDIN settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return 0;
                }
                break;
            case well:
                if (world->player.hp < 5) {
                    world->player.hp++;
                    world->player.healing++;
                }
                break;
            case relic:
                world->player.relics++;
                if (!world->monster.alive) {
                    spawnMonster(world);
                }
                if (world->player.relics == world->relics) {
                    clearField(world);
                    updateStatistics(world);
                    //restore STDIN settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return 0;
                }
                break;
        }
        updateStatistics(world);

        /**clear field*/
        clearField(world);




    }
    //restore STDIN settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
}

void spawnMonster(struct world *world) {
    /**random spawn position*/
    world->monster.alive = true; // rand mod SIZE-2 to
    world->monster.position.x = (rand() % (GRID_WIDTH-1) + 1);
    world->monster.position.y = (rand() % (GRID_HEIGHT - 1) + 1);

    moveMonster(world);
}

void moveMonster(struct world *world) {
    //save cursor position
    printf("\033[s");
    //move cursor to spawn
    printf("\033[%d;%dH", world->monster.position.y + 2, world->monster.position.x + 2);
    //spawn monster
    printf("\033[0;31mö\033[1;37m");
    //restore cursor position
    printf("\033[u");
}

void clearField(struct world *world) {
    world->grid[world->player.position.y][world->player.position.x] = empty;
    printf("\033[C\b \033[D");
}

void resetField(struct world *world) {
    char c;
    switch (world->grid[world->monster.position.y][world->monster.position.x]) {
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
            c = 'n';
            break;
    }
    //save cursor position
    printf("\033[s");
    //move cursor to monster
    printf("\033[%d;%dH", world->monster.position.y + 2, world->monster.position.x + 2);
    //remove monster
    printf("\033[C\b%c\033[D", c);
    //restore cursor position
    printf("\033[u");
}

void chasePlayer(struct world *world) {
    int distance_x, distance_y;
    distance_x = distance_y = 0;

    distance_x = world->player.position.x - world->monster.position.x;
    distance_y = world->player.position.y - world->monster.position.y;

    /**move on x or y*/
    if ((std::abs(distance_x) >= std::abs(distance_y)) && distance_y != 0) {
        /**move on y*/
        if (distance_y > 0) {
            /**down*/
            resetField(world);
            world->monster.position.y++;
            moveMonster(world);
        } else if (distance_y < 0) {
            /**up*/
            resetField(world);
            world->monster.position.y--;
            moveMonster(world);
        }
    } else if (((std::abs(distance_x) <= std::abs(distance_y)) && distance_x != 0) || (distance_x != 0 && distance_y == 0)) {
        /**move on x*/033[0;31mö\
        if (distance_x > 0) {
            /**right*/
            resetField(world);
            world->monster.position.x++;
            moveMonster(world);
        } else if (distance_x < 0) {
            /**left*/
            resetField(world);
            world->monster.position.x--;
            moveMonster(world);
        }
    } else if (distance_x == 0 && distance_y == 0) {
        /**despawn*/
        resetField(world);
        world->monster.alive = false;
        world->player.hp--;
    }
}

/***/
void updateStatistics(struct world *world) {
    //save cursor position
    printf("\033[s");
    //move cursor to STAT_POS and update
    printf("\033[1;%dH", STAT_POS);
    printf("01_OASENCRAWLER");
    printf("\033[2;%dH", STAT_POS);
    printf("HP: %d/5", world->player.hp);
    printf("\033[3;%dH", STAT_POS);
    printf("Relics: %d/%d", world->player.relics, world->relics);
    printf("\033[4;%dH", STAT_POS);
    printf("Damage taken: %d", world->player.damageTaken);
    printf("\033[5;%dH", STAT_POS);
    printf("Healing done: %d", world->player.healing);
    printf("\033[6;%dH", STAT_POS);
    printf("Monster up: %s", world->monster.alive ? "true " : "false");
    printf("\033[7;%dH", STAT_POS);
    printf("Player %d|%d", world->player.position.x, world->player.position.y);
    printf("\033[8;%dH", STAT_POS);
    printf("Monster %d|%d", world->monster.position.x, world->monster.position.y);


    //restore cursor position
    printf("\033[u");
    if (world->player.hp == 0) {
        printf("\033[9;%dH", STAT_POS);
        printf("GAME OVER\n\n\n");
    }
    if (world->player.relics == world->relics) {
        printf("\033[9;%dH", STAT_POS);
        printf("Completed!\v\n");
    }

}

/***/
void draw(struct world *world) {
    for (int i = 0; i < GRID_HEIGHT + 2; i++) {
        for (int j = 0; j < GRID_WIDTH + 2; j++) {
            /*draw border*/
            if (i == 0 || i == GRID_HEIGHT + 1) {
                std::cout << '-';
                if (j == GRID_WIDTH + 1) {
                    std::cout << "         " << std::endl;
                }
            } else if ((i > 0 && i < GRID_HEIGHT + 2) && (j == 0 || j == GRID_WIDTH + 1)) {
                std::cout << '|';
                if (j == GRID_WIDTH + 1) {
                    std::cout << "          " << std::endl;
                }
            } else {
                /*draw game grid */
                switch (world->grid[i - 1][j - 1]) {
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
                        break;
                }
            }
        }
    }
    updateStatistics(world);
}

/**initialize world */
void initWorld(struct world *world) {
    enum fieldType field;
    world->relics = 0; //set relic counter
    /**init fields of grid*/
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (i == 0 && j == 0) {
                world->grid[0][0] = empty;
                continue;
            }
            field = initField();
            world->grid[i][j] = field;
            /*count relic fields*/
            if (field == relic) {
                world->relics++;
            }
        }
    }
    if (world->relics == 0) {
        initWorld(world);
    }
    /**initialize player*/
    initPlayer(&world->player);

    /**init monster*/
    world->monster.alive = false;
    world->monster.position.x = 0;
    world->monster.position.y = 0;
}

/**initialize player attributes with default values*/
void initPlayer(struct player *player) {
    player->hp = 5;
    player->relics = 0;
    player->damageTaken = 0;
    player->healing = 0;
    player->position.x = 0;
    player->position.y = 0;
}

/**initialize a field with a rnd field type*/
enum fieldType initField() {
    int rnd = 0;
    enum fieldType field;
    rnd = rand() % 10;

    switch (rnd) {
        /* case range 0 to 3 (only works with gcc)*/
        case 0 ... 3:
            field = empty;
            break;
        case 4 ... 7:
            field = danger;
            break;
        case 8:
            field = well;
            break;
        case 9:
            field = relic;
            break;
        default:
            printf("UPS!!!!!!!!!\n");
            break;
    }
    return field;
}
