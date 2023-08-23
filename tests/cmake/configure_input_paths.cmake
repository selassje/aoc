macro(to_native path)
    cmake_path(NATIVE_PATH ${path} ${path})
endmacro()

cmake_path(SET ROOT "${CMAKE_CURRENT_SOURCE_DIR}/inputs")

cmake_path(SET INPUTS_DAY1_INPUT "${ROOT}/day1/input.txt")
cmake_path(SET INPUTS_DAY1_EXAMPLE "${ROOT}/day1/example.txt")

to_native(INPUTS_DAY1_EXAMPLE)
to_native(INPUTS_DAY1_INPUT)

configure_file(inputs.hpp.in inputs.hpp)