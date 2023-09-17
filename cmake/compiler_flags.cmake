
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
