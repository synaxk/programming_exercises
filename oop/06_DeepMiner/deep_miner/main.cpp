#include <iostream>
#include "headers/Block.h"
#include "headers/GameHandler.h"

int main() {

    srand(time(nullptr));

    GameHandler gh;

    gh.init();
    gh.play();


    return 0;
}
