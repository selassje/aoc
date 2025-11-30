
#include "day1.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

using aoc25::day1::Input;
using inputs::FileReader;
using inputs::parseLine;

namespace {

}

TEST_CASE("Aoc25 Day1 Example", "[AoC25_Day1]")
{
  const auto input = 2;
  const auto [part1, part2] = aoc25::day1::solve(input);
  REQUIRE(part1 == 0);
  REQUIRE(part2 == 0);
}
