//
// Created by alex on 06.04.22.
//

#ifndef INC_04_DEEPMINER_BLOCK_H
#define INC_04_DEEPMINER_BLOCK_H
#include <array>
#include <random>
#include <algorithm>
#include <iostream>
typedef struct {
    int x;
    int y;
} Position;

class Block {
public:
    Block();
    ~Block();
    void initFields();
    void printSection(Position pos);
    std::array<int, 10> getSection(Position position);
    void changeSection(Position position, std::array<int, 10> section);
    bool checkIfSectionEmpty(std::array<int,10> section);
    bool checkIfBlockEmpty();
    void printBlock();
private:
    std::array<std::array<std::array<int, 10>, 5>, 5> block;

};


#endif //INC_04_DEEPMINER_BLOCK_H
