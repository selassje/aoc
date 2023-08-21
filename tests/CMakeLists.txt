include(CTest)

include("cmake/configure_input_paths.cmake")

add_executable(Day1 day1.cpp)
target_include_directories(Day1 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
add_test(NAME Day1 COMMAND Day1)
