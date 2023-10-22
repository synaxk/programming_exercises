//
// Created by alex on 01.03.22.
//

#include "../headers/Field.h"

Field::Field() {
    this->type = empty;
}

int Field::init() {
    /**init rnd seed*/

    int rnd = rand() % 10;

    switch (rnd) {
        case 0 ... 3 :
            this->type = empty;
            break;
        case 4 ... 7:
            this->type = danger;
            break;
        case 8:
            this->type = well;
            break;
        case 9:
            this->type = relic;
        default:
            break;
    }
    return 1;
}

int Field::clear() {
    this->type = empty;
    return 1;
}
