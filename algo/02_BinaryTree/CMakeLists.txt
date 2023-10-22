cmake_minimum_required(VERSION 3.21)
project(treecheck)

set(CMAKE_CXX_STANDARD 14)

add_executable(treecheck main.cpp src/BinaryTree.cpp headers/BinaryTree.h src/ArgumentHandler.cpp headers/ArgumentHandler.h)
