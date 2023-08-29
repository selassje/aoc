cmake_minimum_required(VERSION 3.5)

include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)

add_library(aoc22)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include/aoc22)

target_sources(aoc22 INTERFACE  ${INCLUDE}/day1.hpp 
                     PUBLIC     ${INCLUDE}/day2.hpp ${INCLUDE}/day3.hpp 
                     PRIVATE    day2.cpp day3.cpp) 

target_include_directories(aoc22 PUBLIC ${INCLUDE})

target_setup_compile_options(aoc22)

if(ENABLE_CLANG_TIDY)
    set_target_properties(aoc22 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()
