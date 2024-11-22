cmake_minimum_required(VERSION 3.28)

project(AoC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

option(ENABLE_TESTS "Enable Tests" OFF)
option(ENABLE_FUZZ_TESTS "Enable Fuzzing Tests" OFF)
option(ENABLE_CLANG_TIDY "Enable Clang Tidy" OFF)
option(ENABLE_COVERAGE_REPORT "Enable Test Coverage Report" OFF)
option(ENABLE_MSVC_ANALYSIS "Enable MSVC Analysis" OFF)

include(cmake/compiler_and_linker_flags.cmake)
include(cmake/setup_static_analysis.cmake)
include(cmake/clang_format.cmake)
include(cmake/coverage_report.cmake)

add_subdirectory(src)

if(ENABLE_TESTS)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    enable_testing()
    add_subdirectory(tests)
endif()

setup_coverage_report_target()

