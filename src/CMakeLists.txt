set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include)

function(configure_sources YEAR)
  set(MODULES ${CMAKE_CURRENT_LIST_DIR}/../import)
  file(GLOB MODULE_FILES ${MODULES}/aoc${YEAR}/*.cppm)
  file(GLOB HEADER_FILES ${INCLUDE}/aoc${YEAR}/*.hpp)
  file(GLOB SOURCE_FILES aoc${YEAR}/*.cpp)
  target_sources(
    aoc
    PUBLIC ${HEADER_FILES}
    PUBLIC FILE_SET
           public_modules
           TYPE
           CXX_MODULES
           BASE_DIRS
           ${MODULES}
           FILES
           ${MODULE_FILES}
    PRIVATE ${SOURCE_FILES})

endfunction(configure_sources)

function(configure_common_sources)

  set(COMMON ${CMAKE_CURRENT_LIST_DIR}/common)

  file(GLOB MODULE_FILES ${COMMON}/*.cppm)
  file(GLOB HEADER_FILES ${COMMON}/*.hpp)
  file(GLOB SOURCE_FILES ${COMMON}/*.cpp)
  
  if(MSVC)
   message(${CMAKE_CXX_COMPILER})
   get_filename_component(CXX_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
   set(STD_MODULE_BASE_DIR ${CXX_DIR}../../../../modules)
   set(STD_MODULE ${STD_MODULE_BASE_DIR}/std.ixx)
  endif()
  
  target_sources(
    aoc
    PRIVATE ${HEADER_FILES}
    PRIVATE FILE_SET
            private_modules
            TYPE
            CXX_MODULES
            BASE_DIRS
            ${STD_MODULE_BASE_DIR}
            ${COMMON}
            FILES
            ${MODULE_FILES}
            ${STD_MODULE}
    PRIVATE ${SOURCE_FILES})

endfunction(configure_common_sources)

add_library(aoc)

configure_common_sources()

configure_sources(22)
configure_sources(23)
configure_sources(24)
configure_sources(25)

target_include_directories(aoc PUBLIC ${INCLUDE})

if(ENABLE_TESTS)
  target_link_libraries(aoc PRIVATE Catch2)
endif()

setup_compiler_warnings(aoc)
setup_sanitizers(aoc)
setup_std_lib(aoc)
setup_clang_tidy(aoc)
setup_msvc_analysis(aoc)
setup_tests_flags(aoc)
setup_coverage_report_flags(aoc)
