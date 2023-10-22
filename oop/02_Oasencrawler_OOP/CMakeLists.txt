cmake_minimum_required(VERSION 3.21)
project(02_Oasencrawler_OOP)

set(CMAKE_CXX_STANDARD 14)

add_executable(02_Oasencrawler_OOP main.cpp src/World.cpp headers/World.h src/Player.cpp headers/Player.h main.h headers/utils.h src/Field.cpp headers/Field.h src/InputHandler.cpp headers/InputHandler.h src/Test.cpp headers/Test.h src/TestHandler.cpp headers/TestHandler.h src/Enemy.cpp headers/Enemy.h src/Bullet.cpp headers/Bullet.h)
