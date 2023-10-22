cmake_minimum_required(VERSION 3.22)
project(Flottenkampf)

set(CMAKE_CXX_STANDARD 14)

add_executable(Flottenkampf main.cpp src/GameHandler.cpp headers/GameHandler.h src/Player.cpp headers/Player.h src/Ship.cpp headers/Ship.h src/Destroyer.cpp headers/Destroyer.h src/InputHandler.cpp headers/InputHandler.h src/Hunter.cpp headers/Hunter.h src/Cruiser.cpp headers/Cruiser.h headers/ShipIsDeadException.h)
