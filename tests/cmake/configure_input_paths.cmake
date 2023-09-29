macro(set_path DAY FILENAME)
  cmake_path(SET ROOT "${CMAKE_SOURCE_DIR}/tests/inputs")
  string(TOUPPER ${FILENAME} FILENAME_UPPER)
  cmake_path(SET INPUTS_DAY${DAY}_${FILENAME_UPPER} "${ROOT}/day${DAY}/${FILENAME}.txt")
endmacro(set_path DAY FILENAME)

foreach(i RANGE 1 11)
  set_path(${i} input)
  set_path(${i} example)
endforeach()

set_path(9 example2)

configure_file(inputs.hpp.in inputs.hpp)