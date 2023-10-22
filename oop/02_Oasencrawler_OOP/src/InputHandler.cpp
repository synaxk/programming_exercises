//
// Created by alex on 05.03.22.
//

#include "../headers/InputHandler.h"

InputHandler::InputHandler(char *opts) {
    this->opts = opts;
}

int InputHandler::setICanonMode() {
    /*skip input buffer*/
    // change terminal input to cannonical mode
    // write the settings of STDIN to oldt
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    //copy the settings to newt
    newt = oldt;
    //turn off icanon and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    //change STDIN settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

int InputHandler::setNormalMode() {
    //tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
}

int InputHandler::read() {
    this->input = (char)getchar();
    if (this->validate()) {
        return this->input;
    }
    return 0;
}

int InputHandler::validate() {
    int c = (int)strlen(opts);

    for(int i = 0; i < c; i++) {
        if (input == opts[i]) {
            return 1;
        }
    }
    return 0;
}