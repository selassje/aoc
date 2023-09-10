
if(ENABLE_CLANG_TIDY)
    message("Clang-tidy enabled")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

    function(setup_clang_tidy TARGET)
      set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endfunction(setup_clang_tidy TARGET)
    
endif()