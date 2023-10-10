
function(setup_compiler_warnings TARGET)
  if (MSVC)
    cmake_path(SET RULESET_FILE ${CMAKE_SOURCE_DIR}/.vsAnalyze.ruleset)
    cmake_path(NATIVE_PATH RULESET_FILE RULESET_FILE)
    cmake_path(NATIVE_PATH CMAKE_SOURCE_DIR RULESET_DIR)
    message("PATH " ${RULESET_FILE})
    set(ANALYZE_FLAG  /analyze:ruleset${RULESET_FILE})
    set(ANALYZE_FLAG2 /analyze:projectdirectory${RULESET_DIR})
    set(ANALYZE_FLAG3 /analyze:rulesetdirectory${RULESET_DIR})


    get_filename_component(CXX_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(PLUGIN_NAME ${CXX_DIR}/EspXEngine.dll)

    set(ANALYZE_FLAG4 /analyze:plugin${PLUGIN_NAME})
    message("ANAL " ${ANALYZE_FLAG})
    #target_compile_options(${TARGET} PRIVATE "${ANALYZE_FLAG}")
#    target_compile_options(${TARGET} PRIVATE /analyze:ruleset [=["NativeRecommendedRules.ruleset"]=] )
    configure_file(${CMAKE_SOURCE_DIR}/.vsAnalyze.props.in ${CMAKE_SOURCE_DIR}/.vsAnalyze.props)
#    set_property(
 #     TARGET ${TARGET}
  #    PROPERTY VS_USER_PROPS ${CMAKE_SOURCE_DIR}/.vsAnalyze.props)

  target_compile_definitions(aoc22 PRIVATE CODE_ANALYSIS)

    set_property(
      TARGET ${TARGET}
      PROPERTY COMPILE_OPTIONS /analyze  ${ANALYZE_FLAG} ${ANALYZE_FLAG4})
    target_compile_options(${TARGET} PRIVATE /W4 /WX /external:anglebrackets /external:W0 /permissive- /wd4868 /wd5045 /wd4324)
  else()
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
                            -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation 
                            -Wnull-dereference  -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough)
                      
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT ${ENABLE_CLANG_TIDY})
      target_compile_options(${TARGET} PRIVATE -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast)
    endif()
  endif()
endfunction()

function(setup_std_lib TARGET)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if(NOT ${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
      target_compile_options(${TARGET} PRIVATE -stdlib=libc++)
      target_link_options(${TARGET} PRIVATE -stdlib=libc++)
    endif()
  endif()
endfunction()

function(setup_tests_flags TARGET)
  if(ENABLE_FUZZ_TESTS)
    target_compile_options(${TARGET} PRIVATE -g -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -UNDEBUG 
                                             -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-cmp)
                                                #-fsanitize=address -DADDRESS_SANITIZER)
    target_link_options(${TARGET} PRIVATE -fsanitize=address)
    target_compile_definitions(${TARGET} PRIVATE ENABLE_FUZZ_TESTS)
  endif()
  if(ENABLE_TESTS)
    target_compile_definitions(aoc22 PRIVATE ENABLE_TESTS)
  endif()
endfunction()