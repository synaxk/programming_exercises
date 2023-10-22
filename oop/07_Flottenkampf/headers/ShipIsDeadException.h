//
// Created by alex on 21.05.22.
//

#ifndef FLOTTENKAMPF_SHIPISDEADEXCEPTION_H
#define FLOTTENKAMPF_SHIPISDEADEXCEPTION_H

#include <exception>

class ShipIsDeadException : public std::exception {
    const char* what() const noexcept override {
        return "Enemy ship is already destroyed.";
    }
};

#endif //FLOTTENKAMPF_SHIPISDEADEXCEPTION_H
