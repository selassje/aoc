macro(set_path DAY FILENAME)
  cmake_path(SET ROOT "${CMAKE_SOURCE_DIR}/tests/inputs")
  string(TOUPPER ${FILENAME} FILENAME_UPPER)
  cmake_path(SET INPUTS_DAY${DAY}_${FILENAME_UPPER} "${ROOT}/day${DAY}/${FILENAME}.txt")
endmacro(set_path DAY FILENAME)

set_path(1 input)
set_path(1 example)

set_path(2 input)

set_path(3 input)

set_path(4 input)
set_path(4 example)

set_path(5 input)
set_path(5 example)

set_path(6 input)

set_path(7 input)
set_path(7 example)

set_path(8 input)
set_path(8 example)

set_path(9 input)
set_path(9 example)
set_path(9 example2)

configure_file(inputs.hpp.in inputs.hpp)