//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_CRUISER_H
#define FLOTTENKAMPF_CRUISER_H

#include "Ship.h"

class Cruiser : public Ship {
public:
    Cruiser(int id, std::array<std::array<int, 10>, 20> &grid);
    ~Cruiser();
    void attack(Ship &enemy) override;
};


#endif //FLOTTENKAMPF_CRUISER_H
