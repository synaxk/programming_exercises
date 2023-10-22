//
// Created by alex on 06.04.22.
//

#include "../headers/ArgumentHandler.h"

ArgumentHandler::ArgumentHandler(int argc, char **argv) {

    static const struct option longOpts[] = {
            {.name = "help", .has_arg = no_argument, .val = 'h'},
            {.name = "version", .has_arg = no_argument, .val = 'v'},
            {.name = "file", .has_arg = 1, .val = 'f'},
            {.name = "search", .has_arg = 1, .val = 's'}
    };

    opterr = 0;
    int longIndex = -1;
    for (int opt = 0;(opt = getopt_long(argc, argv, "hvfs", longOpts, &longIndex)) != -1;) {
        switch (opt) {
            case 'v':
                /// print version
                std::cout << "version" << std::endl;
                break;
            case 'h':
            case '?':
                /// print help
                std::cout << "help" << std::endl;
                break;
            case 'f':
                /// import data from file
                std::cout << "file: " << std::endl;
                baseFile = std::string(optarg);
                break;
            case 's':
                /// search something
                std::cout << "search" << std::endl;
                subFile = std::string(optarg);
                break;
            default:
                return;
        }
    }

}

ArgumentHandler::~ArgumentHandler() = default;