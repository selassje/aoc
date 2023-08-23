cmake_minimum_required(VERSION 3.6)

include(CTest)

set(CMAKE_CXX_STANDARD 20)

include("cmake/configure_input_paths.cmake")

set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
add_subdirectory(Catch2)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
cmake_path(NATIVE_PATH CLANG_TIDY_EXE CLANG_TIDY_EXE)

add_executable(Day1 day1.cpp)
set_target_properties(Day1 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(Day1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)

target_link_libraries(Day1 PRIVATE Catch2::Catch2WithMain)


add_test(NAME Day1 COMMAND Day1)

