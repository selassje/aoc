
include(CTest)

include("cmake/configure_input_paths.cmake")

add_subdirectory(Catch2 SYSTEM)

setup_std_lib(Catch2)

if(ENABLE_FUZZ_TESTS)

  function(setup_std_lib_for_targets DIR)
    get_property(TGTS DIRECTORY "${DIR}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(TGT IN LISTS TGTS)
      setup_std_lib(${TGT})
    endforeach() 
   get_property(SUBDIRS DIRECTORY "${DIR}" PROPERTY SUBDIRECTORIES)
    foreach(SUBDIR IN LISTS SUBDIRS)
      setup_std_lib_for_targets("${SUBDIR}")
    endforeach()
  endfunction()

  add_subdirectory(fuzztest)
  setup_std_lib_for_targets(fuzztest)

endif()

foreach(i RANGE 1 12)
    set(TEST_NAME day${i})

    add_executable(${TEST_NAME} ${TEST_NAME}.cpp main.cpp)

    setup_msvc_analysis(${TEST_NAME})
    setup_compiler_warnings(${TEST_NAME})
    setup_std_lib(${TEST_NAME})
    setup_clang_tidy(${TEST_NAME})
    setup_tests_flags(${TEST_NAME})
    setup_coverage_report_flags(${TEST_NAME})

    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                            PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc22)
    
    target_link_libraries(${TEST_NAME} PRIVATE Catch2 PRIVATE aoc22)

    if(ENABLE_FUZZ_TESTS)
      setup_std_lib(fuzztest_gtest_main)
      link_fuzztest(${TEST_NAME})
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    setup_coverate_report_env(${TEST_NAME})
endforeach()
    


