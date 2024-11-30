function(setup_compiler_warnings TARGET)
  if(MSVC)
    target_compile_options(
      ${TARGET}
      PRIVATE /W4
              /WX
              /external:anglebrackets
              /external:W0
              /permissive-
              /wd4868
              /wd5045
              /wd4324)
  else()
    target_compile_options(
      ${TARGET}
      PRIVATE -Wall
              -Wextra
              -Wpedantic
              -Werror
              -Wshadow
              -Wnon-virtual-dtor
              -Wold-style-cast
              -Wcast-align
              -Wunused
              -Woverloaded-virtual
              -Wconversion
              -Wsign-conversion
              -Wmisleading-indentation
              -Wnull-dereference
              -Wdouble-promotion
              -Wformat=2
              -Wimplicit-fallthrough)

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT ${ENABLE_CLANG_TIDY})
      target_compile_options(
        ${TARGET} PRIVATE -Wduplicated-cond -Wduplicated-branches -Wlogical-op
                          -Wuseless-cast -Wnrvo)
    endif()
  endif()
endfunction()

function(setup_sanitizers TARGET)
  if(ENABLE_SANITIZERS)
    if(MSVC)
      target_compile_options(
        ${TARGET}
        PRIVATE /fsanitize=address)
    endif()
  endif()
endfunction()

function(setup_std_lib TARGET)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND (${CMAKE_SYSTEM_NAME}
                                                    STREQUAL "Linux"))
    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if(NOT ${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
      target_compile_options(${TARGET} PRIVATE -stdlib=libc++)
      target_link_options(${TARGET} PRIVATE -stdlib=libc++)
    endif()
  endif()
endfunction()

function(setup_tests_flags TARGET)
  if(ENABLE_FUZZ_TESTS)
    target_compile_options(
      ${TARGET}
      PRIVATE -g -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -UNDEBUG
              -fsanitize-coverage=inline-8bit-counters
              -fsanitize-coverage=trace-cmp)
    # -fsanitize=address -DADDRESS_SANITIZER)
    target_link_options(${TARGET} PRIVATE -fsanitize=address)
    target_compile_definitions(${TARGET} PRIVATE ENABLE_FUZZ_TESTS)
  endif()
  if(ENABLE_TESTS)
    target_compile_definitions(${TARGET} PRIVATE ENABLE_TESTS)
  endif()
  target_compile_definitions(${TARGET} PRIVATE $<$<CONFIG:DEBUG>:DEBUG>)
endfunction()
