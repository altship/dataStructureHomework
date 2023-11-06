cmake_minimum_required(VERSION 3.26)
project(C__Projects)

set(CMAKE_CXX_STANDARD 17)

add_executable(C__Projects FIFO_simulator.cpp
        altship.h
        avlTree.cpp
        copy.cpp
)
