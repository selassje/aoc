cmake_minimum_required(VERSION 3.6)

project(AoC22 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

include(cmake/format.cmake)


