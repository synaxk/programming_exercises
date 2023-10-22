//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_DESTROYER_H
#define FLOTTENKAMPF_DESTROYER_H

#include "Ship.h"

class Destroyer : public Ship {
public:
    Destroyer(int id, std::array<std::array<int, 10>, 20> &grid);
    ~Destroyer();
    void attack(Ship &enemy) override;
};


#endif //FLOTTENKAMPF_DESTROYER_H
