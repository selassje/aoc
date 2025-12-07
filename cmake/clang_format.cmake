file(
  GLOB
  ALL_CXX_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/aoc*/*
  ${CMAKE_SOURCE_DIR}/include/aoc*/*.hpp
  ${CMAKE_SOURCE_DIR}/import/aoc*/*.cppm
  ${CMAKE_SOURCE_DIR}/src/common/*
  ${CMAKE_SOURCE_DIR}/tests/*.hpp
  ${CMAKE_SOURCE_DIR}/tests/*.cpp
  ${CMAKE_SOURCE_DIR}/tests/aoc*/*.cpp)

if("${ALL_CXX_SOURCE_FILES}" STREQUAL "")
  message(SEND_ERROR "No files to format")
endif()

find_program(CLANG_FORMAT "clang-format")

if(CLANG_FORMAT)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i -style=file
                                         ${ALL_CXX_SOURCE_FILES})
  add_custom_target(
    clang-format-check COMMAND ${CLANG_FORMAT} -dry-run -style=file -Werror
                               ${ALL_CXX_SOURCE_FILES})
else()
  message("Clang-format not available")
endif()
