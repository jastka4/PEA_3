cmake_minimum_required(VERSION 3.12)
project(Lab_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(src)

add_executable(Lab_3
        include/Graph.cpp
        include/Graph.h
        include/GraphMatrix.cpp
        include/GraphMatrix.h
        include/Timer.cpp
        include/Timer.h
        src/main.cpp
        src/TSP.cpp
        src/TSP.h src/GA.cpp src/GA.h)
