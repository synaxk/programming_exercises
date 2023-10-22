cmake_minimum_required(VERSION 3.21)
project(03_FightClub)

set(CMAKE_CXX_STANDARD 14)

add_executable(03_FightClub main.cpp src/Fighter.cpp headers/Fighter.h src/Ability.cpp headers/Ability.h src/GameHandler.cpp headers/GameHandler.h src/Fight.cpp headers/Fight.h src/OrcFighter.cpp headers/OrcFighter.h src/Human.cpp headers/Human.h)
