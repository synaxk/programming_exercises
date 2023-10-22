//
// Created by alex on 21.05.22.
//

#include "../headers/InputHandler.h"

InputHandler::~InputHandler() = default;

InputHandler::InputHandler() = default;

std::string InputHandler::read(std::vector<std::string> opts) {
    std::string input;
    std::cin >> input;

    if (opts.empty()) {
        return input;
    }

    for (auto it = std::begin(opts); it != std::end(opts); it++) {
        if (input == *it) {
            return input;
        }
    }

    throw std::invalid_argument("Invalid Argument:" + input + "\n");
}
