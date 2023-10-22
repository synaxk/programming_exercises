cmake_minimum_required(VERSION 3.21)
project(04_DeepMiner)

set(CMAKE_CXX_STANDARD 14)

add_executable(04_DeepMiner main.cpp src/Block.cpp headers/Block.h src/Bot.cpp headers/Bot.h src/GameHandler.cpp headers/GameHandler.h src/GreedyBot.cpp headers/GreedyBot.h src/SortBot.cpp headers/SortBot.h src/SearchBot.cpp headers/SearchBot.h google_tests/DeepMinerTest.cpp google_tests/bot_tests/bot_tests.cpp)

add_subdirectory(google_tests)
add_subdirectory()