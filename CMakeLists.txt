cmake_minimum_required(VERSION 3.12)

project(AoC22 CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(${CMAKE_PROJECT_NAME} INTERFACE "src/day1.hpp")