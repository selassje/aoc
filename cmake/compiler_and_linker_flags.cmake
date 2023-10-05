
function(setup_compiler_flags TARGET)

if (MSVC)
     target_compile_options(${TARGET} PRIVATE /W4 /WX /external:anglebrackets /external:W0 /permissive /wd4868 /wd5045 /wd4324)
else()
     target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
                            -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation 
                            -Wnull-dereference  -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough)
                        
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT ${ENABLE_CLANG_TIDY})
        target_compile_options(${TARGET} PRIVATE -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast)
    endif()

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
        target_compile_options(${TARGET} PRIVATE -stdlib=libc++)
    endif()

    if (ENABLE_COVERAGE_REPORT)
        target_compile_options(${TARGET} PRIVATE --coverage -fno-elide-constructors)
    endif()
    
    if (ENABLE_FUZZ_TESTS)
        target_compile_options(${TARGET} PRIVATE -g -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -UNDEBUG 
                                                 -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-cmp)
                                                 #-fsanitize=address -DADDRESS_SANITIZER)
    endif()
endif()
endfunction(setup_compiler_flags TARGET)

function(setup_definitions TARGET)
    if(ENABLE_FUZZ_TESTS)
      target_compile_definitions(${TARGET} PRIVATE ENABLE_FUZZ_TESTS)
    endif()
    if(ENABLE_TESTS)
      target_compile_definitions(aoc22 PRIVATE ENABLE_TESTS)
    endif()
endfunction()

function(setup_linker_flags TARGET)
    if(ENABLE_FUZZ_TESTS)
      target_link_options(${TARGET} PRIVATE -fsanitize=address)
    endif()
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
        target_link_options(${TARGET} PRIVATE -stdlib=libc++)
    endif()
endfunction()

function(setup_compiler_and_linker_flags TARGET)
  setup_definitions(${TARGET})
  setup_compiler_flags(${TARGET})
  setup_linker_flags(${TARGET})
endfunction()