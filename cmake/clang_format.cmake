file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.cpp *.hpp *.hpp.in
     )

#message("Sources to check " ${ALL_CXX_SOURCE_FILES})

get_filename_component(BINARY_DIR_NAME ${CMAKE_BINARY_DIR} DIRECTORY)

message("Bin dir regex " ${BINARY_DIR_NAME})

#list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ${BINARY_DIR_NAME})
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX CMakeCXXCompilerId.cpp)
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX inputs.hpp)
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX Catch2)
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX fuzztest)


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
