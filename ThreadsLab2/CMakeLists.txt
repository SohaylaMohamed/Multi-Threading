cmake_minimum_required(VERSION 3.12)
project(ThreadsLab2)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
find_package(Threads)

add_executable(ThreadsLab2 main.cpp matrixMultiplication.cpp matrixMultiplication.h mergeSort.cpp mergeSort.h)