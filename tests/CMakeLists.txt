cmake_minimum_required(VERSION 3.25)

include(CTest)

include("cmake/configure_input_paths.cmake")

add_subdirectory(Catch2 SYSTEM)

if(ENABLE_FUZZ_TESTS)
  add_subdirectory(fuzztest)
endif()


foreach(i RANGE 1 11)
    set(TEST_NAME day${i})

    add_executable(${TEST_NAME} ${TEST_NAME}.cpp main.cpp)
    setup_compiler_flags(${TEST_NAME})

    if(ENABLE_CLANG_TIDY)
        setup_clang_tidy(${TEST_NAME})
    endif()

    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                            PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)
    
    target_link_libraries(${TEST_NAME} PRIVATE Catch2 PRIVATE aoc22)

    if(ENABLE_FUZZ_TESTS)
      link_fuzztest(${TEST_NAME})
      target_link_options(${TEST_NAME} PRIVATE -fsanitize=address)
      target_compile_definitions(${TEST_NAME} PRIVATE ENABLE_FUZZ_TESTS)
    endif()

    if(ENABLE_COVERAGE_REPORT)
        target_link_libraries(${TEST_NAME} PRIVATE gcov)
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
    


