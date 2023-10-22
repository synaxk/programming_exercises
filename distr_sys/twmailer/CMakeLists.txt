cmake_minimum_required(VERSION 3.23)
project(TWMailer C)

set(CMAKE_C_STANDARD 11)

add_executable(client src/client.c)

add_executable(server src/server.c)