cmake_minimum_required(VERSION 3.6)

include(CTest)


include("cmake/configure_input_paths.cmake")

set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
add_subdirectory(Catch2)


foreach(i RANGE 1 2)
    set(TEST_NAME day${i})

    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    if(ENABLE_CLANG_TIDY)
        set_target_properties(${TEST_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()

    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                    PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)

    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain PRIVATE aoc22)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
    


