#include <iostream>
#include "headers/GameHandler.h"

int main() {
    srand(time(nullptr));
    GameHandler gh;
    gh.start();
    return 0;
}
