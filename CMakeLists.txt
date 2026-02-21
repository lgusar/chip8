cmake_minimum_required(VERSION 3.15)
project(chip8)

find_package(raylib 3.0 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

add_executable(chip8 main.cpp)

target_link_libraries(chip8 raylib)
