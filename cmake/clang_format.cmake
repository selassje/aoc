file(
  GLOB
  ALL_CXX_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/aoc22/*.cpp
  ${CMAKE_SOURCE_DIR}/src/aoc23/*.cpp
  ${CMAKE_SOURCE_DIR}/src/aoc24/*.cpp
  ${CMAKE_SOURCE_DIR}/src/aoc25/*.cpp
  ${CMAKE_SOURCE_DIR}/include/aoc22/*.hpp
  ${CMAKE_SOURCE_DIR}/include/aoc23/*.hpp
  ${CMAKE_SOURCE_DIR}/include/aoc24/*.hpp
  ${CMAKE_SOURCE_DIR}/include/aoc25/*.hpp
  ${CMAKE_SOURCE_DIR}/import/aoc25/*.cppm
  ${CMAKE_SOURCE_DIR}/src/common/*
  ${CMAKE_SOURCE_DIR}/tests/*.hpp
  ${CMAKE_SOURCE_DIR}/tests/*.cpp
  ${CMAKE_SOURCE_DIR}/tests/aoc22/*.cpp
  ${CMAKE_SOURCE_DIR}/tests/aoc23/*.cpp
  ${CMAKE_SOURCE_DIR}/tests/aoc24/*.cpp
  ${CMAKE_SOURCE_DIR}/tests/aoc25/*.cpp)

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
