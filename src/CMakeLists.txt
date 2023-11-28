
add_library(aoc)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include)

file(GLOB HEADER_FILES ${INCLUDE}/aoc22/*.hpp ${INCLUDE}/aoc23/*.hpp  )
file(GLOB SOURCE_FILES 2022/*.cpp 2023/*.cpp)

target_sources(aoc  PUBLIC ${HEADER_FILES}     
                    PRIVATE ${SOURCE_FILES}) 

target_include_directories(aoc PUBLIC ${INCLUDE})

if(ENABLE_TESTS)
    target_link_libraries(aoc PRIVATE Catch2)
endif()

setup_compiler_warnings(aoc)
setup_std_lib(aoc)
setup_clang_tidy(aoc)
setup_msvc_analysis(aoc)
setup_tests_flags(aoc)
setup_coverage_report_flags(aoc)
