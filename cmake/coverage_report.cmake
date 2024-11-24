function(setup_coverage_report_target)
  if(ENABLE_COVERAGE_REPORT)
    message("Enabling Test Coverage Report")

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      find_program(LCOV "lcov")
      if(NOT LCOV)
        message(
          SEND_ERROR
            "LCOV not found.Test Coverage Report Not Possible. Try disabling ENABLE_COVERAGE_REPORT"
        )
      endif()

      add_custom_target(
        generate-coverage-report
        COMMAND
          ${LCOV} --capture --directory ${CMAKE_BINARY_DIR} --output
          ${CMAKE_BINARY_DIR}/coverage.info --exclude '*Catch2*' --exclude
          '*11/*' --exclude '/usr/include/*'
        COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage.info --output
                ${CMAKE_BINARY_DIR}/lcov/index.html)

    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
      string(REGEX REPLACE "\\..+" "" VER ${CMAKE_CXX_COMPILER_VERSION})
      set(LLVM_PROFDATA "llvm-profdata-${VER}")
      set(LLVM_COV "llvm-cov-${VER}")

      get_property(
        TEST_TARGETS
        DIRECTORY ${CMAKE_SOURCE_DIR}/tests
        PROPERTY BUILDSYSTEM_TARGETS)
      foreach(TEST IN LISTS TEST_TARGETS)
        set(OBJECTS ${OBJECTS} -object $<TARGET_FILE:${TEST}>)
      endforeach()
      add_custom_target(
        generate-coverage-report
        COMMAND ${LLVM_PROFDATA} merge ${CMAKE_BINARY_DIR}/coverage/*.profraw
                --output ${CMAKE_BINARY_DIR}/coverage.profdata
        COMMAND
          ${LLVM_COV} show ${OBJECTS}
          -instr-profile=${CMAKE_BINARY_DIR}/coverage.profdata -format=text >
          coverage.info)
    endif()

  endif()
endfunction()

function(setup_coverage_report_flags TARGET)
  if(ENABLE_COVERAGE_REPORT)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      target_compile_options(${TARGET} PRIVATE --coverage
                                               -fno-elide-constructors)
      target_link_libraries(${TARGET} PRIVATE --coverage)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
      target_compile_options(${TARGET} PRIVATE -fprofile-instr-generate
                                               -fcoverage-mapping)
      target_link_libraries(${TARGET} PRIVATE -fprofile-instr-generate
                                              -fcoverage-mapping)
    endif()
  endif()
endfunction()

function(setup_coverate_report_env TEST_NAME)
  if(ENABLE_COVERAGE_REPORT)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
      set_property(
        TEST ${TEST_NAME}
        PROPERTY
          ENVIRONMENT
          "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/coverage/${TEST_NAME}.profraw")
    endif()
  endif()
endfunction()
