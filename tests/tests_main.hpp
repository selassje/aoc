
#ifndef TESTS_MAIN_HPP
#define TESTS_MAIN_HPP

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#ifdef ENABLE_FUZZ_TESTS
#include <fuzztest/fuzztest.h>
#include <fuzztest/init_fuzztest.h>
#include <gtest/gtest.h>
#endif

int
main(int argc, char* argv[])
{
  const auto catchTestsResult = Catch::Session().run(argc, argv);
#ifdef ENABLE_FUZZ_TESTS
  testing::InitGoogleTest();
  fuzztest::InitFuzzTest(&argc, &argv);
  const auto fuzzTestResult = RUN_ALL_TESTS();
#else
  const auto fuzzTestResult = 0;
#endif
  return fuzzTestResult + catchTestsResult;
}

#endif