#ifndef AOC22_SRC_UNIT_TEST_HELPER_HPP // NOLINT
#define AOC22_SRC_UNIT_TEST_HELPER_HPP

#ifdef ENABLE_TESTS
#include <catch2/catch.hpp>

#define ADD_TC(a) a // NOLINT
#else
#define ADD_TC(a) // NOLINT
#endif

#endif