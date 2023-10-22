//
// Created by alex on 05.03.22.
//

#ifndef INC_02_OASENCRAWLER_OOP_INPUTHANDLER_H
#define INC_02_OASENCRAWLER_OOP_INPUTHANDLER_H

#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <iostream>
#include <cstdio>
#include <cstring>


class InputHandler {
public:
    InputHandler(char *opts);
    int setICanonMode();
    int setNormalMode();
    int read();
    char input;
private:
    char *opts;
    int validate();
};


#endif //INC_02_OASENCRAWLER_OOP_INPUTHANDLER_H
