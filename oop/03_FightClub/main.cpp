#include <iostream>
#include "headers/GameHandler.h"

int main() {
    srand (time(NULL));

    GameHandler gh;
    gh.initGame();
    gh.startGame();

    return 0;
}
