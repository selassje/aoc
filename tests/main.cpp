#include <catch2/catch_session.hpp>

#ifdef ENABLE_FUZZ_TESTS
#include <fuzztest/init_fuzztest.h> //NOLINT
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
