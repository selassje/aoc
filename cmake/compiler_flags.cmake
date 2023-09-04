
if (MSVC)
    add_compile_options(/Wall /WX /external:anglebrackets /external:W0 /permissive /wd5045 /wd4868)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
                        -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation 
                        -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion
                        -Wformat=2 -Wimplicit-fallthrough) 
    if (ENABLE_COVERAGE_REPORT)
        add_compile_options(--coverage -fno-elide-constructors)
    endif()
endif()
