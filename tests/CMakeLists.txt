include(CTest)

include("cmake/configure_input_paths.cmake")

add_subdirectory(Catch2 SYSTEM)

setup_std_lib(Catch2)
setup_sanitizers(Catch2)

if(ENABLE_FUZZ_TESTS)

  function(setup_std_lib_for_targets DIR)
    get_property(
      TGTS
      DIRECTORY "${DIR}"
      PROPERTY BUILDSYSTEM_TARGETS)
    foreach(TGT IN LISTS TGTS)
      setup_std_lib(${TGT})
    endforeach()
    get_property(
      SUBDIRS
      DIRECTORY "${DIR}"
      PROPERTY SUBDIRECTORIES)
    foreach(SUBDIR IN LISTS SUBDIRS)
      setup_std_lib_for_targets("${SUBDIR}")
    endforeach()
  endfunction()

  add_subdirectory(fuzztest)
  setup_std_lib_for_targets(fuzztest)

endif()

list(APPEND EXCLUDED_TESTS aoc22_day16)

function(configure_tests YEAR)
  configure_test_inputs(${YEAR})
  set(MODULES ${CMAKE_SOURCE_DIR}/../import)
  file(GLOB MODULE_FILES ${MODULES}/aoc${YEAR}/*.cppm)

  file(GLOB SOURCE_FILES aoc${YEAR}/*.cpp)
  foreach(SRC IN LISTS SOURCE_FILES)
    get_filename_component(FILENAME ${SRC} NAME_WE)
    set(TEST_NAME aoc${YEAR}_${FILENAME})
    add_executable(${TEST_NAME})

    setup_compiler_warnings(${TEST_NAME})
    setup_sanitizers(${TEST_NAME})
    setup_std_lib(${TEST_NAME})
    setup_clang_tidy(${TEST_NAME})
    setup_msvc_analysis(${TEST_NAME})
    setup_tests_flags(${TEST_NAME})
    setup_coverage_report_flags(${TEST_NAME})

    target_sources(
      ${TEST_NAME}
      PRIVATE main.cpp
      PRIVATE ${SRC}
      PRIVATE FILE_SET
              cxx_modules
              TYPE
              CXX_MODULES
              BASE_DIRS
              ${MODULES}
              FILES
              ${MODULE_FILES})

    target_include_directories(
      ${TEST_NAME}
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/aoc${YEAR}
      PRIVATE ${CMAKE_SOURCE_DIR}/include/aoc${YEAR})

    target_link_libraries(
      ${TEST_NAME}
      PRIVATE Catch2
      PRIVATE aoc)

    if(ENABLE_FUZZ_TESTS)
      setup_std_lib(fuzztest_gtest_main)
      link_fuzztest(${TEST_NAME})
    endif()

    list(FIND EXCLUDED_TESTS ${TEST_NAME} index)
    if(${index} EQUAL -1)
      add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
      setup_coverate_report_env(${TEST_NAME})
    endif()
  endforeach()
endfunction()

configure_tests(22)
configure_tests(23)
configure_tests(24)
