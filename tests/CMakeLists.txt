include(CTest)

set(CMAKE_CXX_STANDARD 20)

include("cmake/configure_input_paths.cmake")

set(CATCH_BUILD_STATIC_LIBRARY ON)
add_subdirectory(Catch2)

add_executable(Day1 day1.cpp)
target_include_directories(Day1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)

target_link_libraries(Day1 PRIVATE Catch2::Catch2WithMain)

add_test(NAME Day1 COMMAND Day1)


