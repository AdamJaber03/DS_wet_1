cmake_minimum_required(VERSION 3.27)
project(DS_wet_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Wet_1/implemantation)
include_directories(Wet_1/Scripts)

add_executable(DS_wet_1
        Wet_1/implemantation/avl.h
        Wet_1/Scripts/maina1.cpp
        Wet_1/Scripts/Olympicsa1.cpp
        Wet_1/Scripts/Olympicsa1.h
        Wet_1/Scripts/wet1util.h
        Wet_1/Tests/test1.in
        Wet_1/Tests/test1.out
        Wet_1/implemantation/avl.cpp
        Wet_1/implemantation/node.cpp)
