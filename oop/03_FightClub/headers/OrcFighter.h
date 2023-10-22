//
// Created by alex on 30.03.22.
//

#ifndef INC_03_FIGHTCLUB_ORCFIGHTER_H
#define INC_03_FIGHTCLUB_ORCFIGHTER_H
#include "Fighter.h"

class OrcFighter : public Fighter{
public:
    OrcFighter();
    OrcFighter(std::string name, std::string clan);
    void react() override;

private:
    std::string clan;

};


#endif //INC_03_FIGHTCLUB_ORCFIGHTER_H
