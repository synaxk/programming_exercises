//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_INPUTHANDLER_H
#define FLOTTENKAMPF_INPUTHANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <exception>

class InputHandler {
public:
    InputHandler();
    ~InputHandler();
    static std::string read(std::vector<std::string> opts = {});
};


#endif //FLOTTENKAMPF_INPUTHANDLER_H
