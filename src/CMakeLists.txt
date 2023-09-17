cmake_minimum_required(VERSION 3.6)

add_library(aoc22)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include/aoc22)

file(GLOB HEADER_FILES ${INCLUDE}/*.hpp)
file(GLOB SOURCE_FILES *.cpp)

target_sources(aoc22 PUBLIC ${HEADER_FILES}     
                     PRIVATE ${SOURCE_FILES}) 

target_include_directories(aoc22 PUBLIC ${INCLUDE})

if(ENABLE_TESTS)
    target_link_libraries(aoc22 PRIVATE Catch2)
    target_compile_definitions(aoc22 PRIVATE ENABLE_TESTS)
endif()

setup_compiler_flags(aoc22)

if(ENABLE_CLANG_TIDY)
    setup_clang_tidy(aoc22)
endif()
