cmake_minimum_required(VERSION 3.12)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)


add_executable(untitled main.cpp batMonitor.cpp batMonitor.h)