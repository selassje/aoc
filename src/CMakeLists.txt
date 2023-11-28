
add_library(aoc)

set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include)
function(configure_sources YEAR)

  file(GLOB HEADER_FILES ${INCLUDE}/aoc${YEAR}/*.hpp)
  file(GLOB SOURCE_FILES aoc${YEAR}/*.cpp)

  target_sources(aoc  PUBLIC ${HEADER_FILES}     
                      PRIVATE ${SOURCE_FILES}) 

endfunction(configure_sources)
  
target_include_directories(aoc PUBLIC ${INCLUDE})

configure_sources(22)
configure_sources(23)

if(ENABLE_TESTS)
    target_link_libraries(aoc PRIVATE Catch2)
endif()

setup_compiler_warnings(aoc)
setup_std_lib(aoc)
setup_clang_tidy(aoc)
setup_msvc_analysis(aoc)
setup_tests_flags(aoc)
setup_coverage_report_flags(aoc)
