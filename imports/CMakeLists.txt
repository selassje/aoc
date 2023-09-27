
file(GLOB MODULE_FILES *.cppm)

target_sources(aoc22 PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${MODULE_FILES})