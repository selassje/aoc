
cmake_path(SET CATCH2 "${CMAKE_SOURCE_DIR}/tests/Catch2/single_include")
cmake_path(NATIVE_PATH CATCH2 CATCH2)

if (MSVC)
    set($COMPILE_FLAGS_TO_ADD /Wall /WX /external:anglebrackets /external:W0 /permissive /wd5045 /wd4868)
    set($COMPILE_FLAGS_TO_REMOVE)
else()
    set($COMPILE_FLAGS_TO_ADD -Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor)
    set($COMPILE_FLAGS_TO_REMOVE "")

    if (ENABLE_COVERAGE_REPORT)
        list(APPEND $COMPILE_FLAGS_TO_ADD --coverage -fno-elide-constructors)
    endif()

endif()

macro(target_setup_compile_options TARGET)
    separate_arguments(CXX_FLAGS NATIVE_COMMAND ${CMAKE_CXX_FLAGS})
    foreach(FLAG ${$COMPILE_FLAGS_TO_REMOVE})
       list(REMOVE_ITEM CXX_FLAGS ${FLAG})
    endforeach()    
    foreach(FLAG ${$COMPILE_FLAGS_TO_ADD})
       list(APPEND CXX_FLAGS ${FLAG})
    endforeach()    
    set_target_properties(${TARGET} PROPERTIES COMPILE_OPTIONS "${CXX_FLAGS}")
    message("Target " ${TARGET} " compile options : " ${CXX_FLAGS})
endmacro()

