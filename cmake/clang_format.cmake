file(GLOB
     ALL_CXX_SOURCE_FILES
     ${CMAKE_SOURCE_DIR}/src/*.cpp
     ${CMAKE_SOURCE_DIR}/include/aoc22/*.hpp
     ${CMAKE_SOURCE_DIR}/tests/*.cpp
     )

get_filename_component(BINARY_DIR_NAME ${CMAKE_BINARY_DIR} DIRECTORY)

if ( "${ALL_CXX_SOURCE_FILES}" STREQUAL "")
  message(SEND_ERROR "No files to format")
endif()

find_program(CLANG_FORMAT "clang-format")

if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT}
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
  add_custom_target(
    clang-format-check
    COMMAND ${CLANG_FORMAT}
    -dry-run
    -style=file
    -Werror
    ${ALL_CXX_SOURCE_FILES}
    )
else()
  message("Clang-format not available")
endif()
