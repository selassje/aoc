if (ENABLE_COVERAGE_REPORT)
    
message("Enabling Test Coverage Report")

find_program(GCOV "gcov")

    if(GCOV)

    else()
        message("GCOV not found. Test Coverage Report Disabled")
    endif()

endif()