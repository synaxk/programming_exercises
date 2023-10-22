//
// Created by alex on 07.03.22.
//

#include "../headers/Test.h"
#include <cassert>
#include <cstdio>

Test::Test(char *message, bool result) {
    this->message = message;
    this->result = result;
}

Test::Test() {

}

int Test::test() {
    if (this->result) {
        printf("\n%s\t\t\033[1;33m\u2713\033[1;37m", this->message);
        return 1;
    }

    printf("\n%s\t\t\033[0;31m\u2715\033[1;37m", this->message);
    return 0;
}