#include <catch2/catch_test_macros.hpp>

import aoc25.day7;

TEST_CASE("Aoc25 Day6 Example", "[AoC25_Day6]")
{
  const auto part1 = aoc25::day7::solve(); //NOLINT
  REQUIRE(part1 == true);
}
