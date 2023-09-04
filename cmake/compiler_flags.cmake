
if (MSVC)
    add_compile_options(/Wall /WX /external:anglebrackets /external:W0 /permissive /wd5045 /wd4868)
else()

    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
                        -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation 
                        -Wnull-dereference  -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough)
                        
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT ${ENABLE_CLANG_TIDY})
        add_compile_options(-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast)
    endif()

    if (ENABLE_COVERAGE_REPORT)
        add_compile_options(--coverage -fno-elide-constructors)
    endif()
endif()
