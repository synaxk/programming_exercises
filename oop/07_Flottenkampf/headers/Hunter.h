//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_HUNTER_H
#define FLOTTENKAMPF_HUNTER_H

#include "Ship.h"

class Hunter : public Ship{
public:
    Hunter(int id, std::array<std::array<int, 10>, 20> &grid);
    ~Hunter();
    void attack(Ship &enemy) override;
};


#endif //FLOTTENKAMPF_HUNTER_H
