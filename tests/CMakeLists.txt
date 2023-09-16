cmake_minimum_required(VERSION 3.6)

include(CTest)

include("cmake/configure_input_paths.cmake")

set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
add_subdirectory(Catch2)

if(ENABLE_FUZZ_TESTS)
  add_subdirectory(fuzztest)
endif()

foreach(i RANGE 1 9)
    set(TEST_NAME day${i})

    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    if(ENABLE_CLANG_TIDY)
        setup_clang_tidy(${TEST_NAME})
    endif()

    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                    PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)

    target_link_libraries(${TEST_NAME} PRIVATE Catch2WithMain PRIVATE aoc22)

    if(ENABLE_COVERAGE_REPORT)
        target_link_libraries(${TEST_NAME} PRIVATE gcov)
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
    


