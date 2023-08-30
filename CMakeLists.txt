cmake_minimum_required(VERSION 3.6)

project(AoC22 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

option(ENABLE_CLANG_TIDY "Enable Clang Tidy" ON)

if(ENABLE_CLANG_TIDY)
    message("Clang-tidy enabled")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
endif()

add_subdirectory(src)
add_subdirectory(tests)

include(cmake/clang_format.cmake)

unset(ENABLE_CLANG_TIDY CACHE)
unset(CMAKE_CXX_FLAGS CACHE)

