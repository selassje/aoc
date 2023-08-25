cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

add_library(aoc22)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include/aoc22)

target_sources(aoc22 INTERFACE "${INCLUDE}/day1.hpp" 
                     PUBLIC    "${INCLUDE}/day2.hpp" 
                     PRIVATE   "day2.cpp") 

target_include_directories(aoc22 PUBLIC ${INCLUDE})

if(ENABLE_CLANG_TIDY)
    set_target_properties(aoc22 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()
