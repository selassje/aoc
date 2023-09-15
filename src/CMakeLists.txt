cmake_minimum_required(VERSION 3.6)

add_library(aoc22)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include/aoc22)

target_sources(aoc22 INTERFACE  ${INCLUDE}/day1.hpp 
                     PUBLIC     ${INCLUDE}/day2.hpp ${INCLUDE}/day3.hpp ${INCLUDE}/day4.hpp ${INCLUDE}/day5.hpp ${INCLUDE}/day6.hpp ${INCLUDE}/day7.hpp ${INCLUDE}/day8.hpp 
                     PRIVATE    day2.cpp day3.cpp day4.cpp day5.cpp day6.cpp day7.cpp day8.cpp) 

target_include_directories(aoc22 PUBLIC ${INCLUDE})

if(ENABLE_TESTS)
    target_include_directories(aoc22 PRIVATE ${CMAKE_SOURCE_DIR}/tests/Catch2/single_include)
    target_compile_definitions(aoc22 PRIVATE ENABLE_TESTS)
endif()


if(ENABLE_CLANG_TIDY)
    setup_clang_tidy(aoc22)
endif()
