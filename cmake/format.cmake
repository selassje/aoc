file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI] *.hpp.in
     )

list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ${CMAKE_BINARY_DIR})

find_program(CLANG_FORMAT "clang-format")

if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND clang-format
    -dry-run
    -style=file
    -Werror
    ${ALL_CXX_SOURCE_FILES}
    )
endif()
