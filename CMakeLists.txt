cmake_minimum_required(VERSION 3.6)

project(AoC22 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_TESTS "Enable Tests" OFF)
option(ENABLE_CLANG_TIDY "Enable Clang Tidy" OFF)
option(ENABLE_COVERAGE_REPORT "Enable Test Coverage Report" OFF)

if(ENABLE_CLANG_TIDY)
    message("Clang-tidy enabled")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
endif()

add_subdirectory(src)

if(ENABLE_TESTS)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    enable_testing()
    add_subdirectory(tests)
endif()

include(cmake/clang_format.cmake)
include(cmake/coverage_report.cmake)

unset(ENABLE_CLANG_TIDY CACHE)
unset(CMAKE_CXX_FLAGS CACHE)

