
#ifndef TESTS_MAIN_HPP
#define TESTS_MAIN_HPP

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fuzztest/fuzztest.h>
#include <fuzztest/init_fuzztest.h>
#include <gtest/gtest.h>

int
main(int argc, char* argv[])
{
  const auto catchTestsResult = Catch::Session().run(argc, argv);
  testing::InitGoogleTest();
  fuzztest::InitFuzzTest(&argc, &argv);
  const auto fuzzTestResult = RUN_ALL_TESTS();
  return fuzzTestResult + catchTestsResult;
}

#endif