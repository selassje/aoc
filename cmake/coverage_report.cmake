if (ENABLE_COVERAGE_REPORT)
    message("Enabling Test Coverage Report")

    find_program(LCOV "lcov")

    if(NOT LCOV)
        message(SEND_ERROR "LCOV not found.Test Coverage Report Not Possible. Try disabling ENABLE_COVERAGE_REPORT")
    endif()
  
   add_custom_target(
    generate-coverage-report
    COMMAND ${LCOV} --capture --directory ${CMAKE_BINARY_DIR} --output ${CMAKE_BINARY_DIR}/coverage.info 
            --exclude '*Catch2*' 
            --exclude '*11/*' 
            --exclude '/usr/include/*'
    COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage.info --output ${CMAKE_BINARY_DIR}/lcov/index.html 
    )
endif()