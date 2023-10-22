#include <iostream>


/**classes*/
#include "headers/World.h"
#include "headers/Test.h"
#include "headers/TestHandler.h"

int testing();

int main() {
    srand (time(NULL));

    testing();

//    World world = World(10);
  // world.info();
   // world.play();

    return 0;
}


int testing(){
    TestHandler unitTests = TestHandler(10);
    unitTests.run();
    return 0;
}


