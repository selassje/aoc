include(CTest)

set(CMAKE_CXX_STANDARD 20)

include("cmake/configure_input_paths.cmake")

add_executable(Day1 day1.cpp)
target_include_directories(Day1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)

add_test(NAME Day1 COMMAND Day1)
