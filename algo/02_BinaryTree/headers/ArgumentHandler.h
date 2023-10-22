//
// Created by alex on 06.04.22.
//

#ifndef UE_2_ARGUMENTHANDLER_H
#define UE_2_ARGUMENTHANDLER_H
#include <fstream>
#include <getopt.h>
#include <iostream>

#include <string>
#include "list"

class ArgumentHandler {
public:
    ArgumentHandler(int argc, char **argv);
    ~ArgumentHandler();

    std::string baseFile;
    std::string subFile;
};


#endif //UE_2_ARGUMENTHANDLER_H
