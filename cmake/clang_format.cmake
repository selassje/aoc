file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI] *.hpp.in
     )

list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ${CMAKE_BINARY_DIR})
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX Catch2)

find_program(CLANG_FORMAT "clang-format")

if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
  add_custom_target(
    clang-format-check
    COMMAND clang-format
    -dry-run
    -style=file
    -Werror
    ${ALL_CXX_SOURCE_FILES}
    )
endif()
