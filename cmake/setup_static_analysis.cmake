
if(ENABLE_CLANG_TIDY)
    message("Clang-tidy enabled")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
endif()
function(setup_clang_tidy TARGET)
  if(ENABLE_CLANG_TIDY)
    set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
endfunction(setup_clang_tidy TARGET)
    
function(setup_msvc_analysis TARGET)
  if(ENABLE_MSVC_ANALYSIS)
    set(RULESET_FILE ${CMAKE_SOURCE_DIR}/.vsAnalyze.ruleset)
    get_filename_component(CXX_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(PLUGIN_NAME ${CXX_DIR}/EspXEngine.dll)
    target_compile_definitions(aoc22 PRIVATE CODE_ANALYSIS)
    set_property(
      TARGET ${TARGET}
      PROPERTY COMPILE_OPTIONS /analyze /analyze:ruleset${RULESET_FILE} /analyze:plugin${PLUGIN_NAME} /analyze:external-)
  endif()
endfunction(setup_msvc_analysis)