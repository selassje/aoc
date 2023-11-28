
#include "day1.hpp"
#include "inputs.hpp"

#include <string_view>

#include <catch2/catch_test_macros.hpp>

using aoc23::day1::Input;

Input
readInput(std::string_view) // NOLINT
{
  return {};
}

TEST_CASE("Aoc23 Day1 Example", "[AoC23_Day1]")
{
  const auto input = readInput(inputs::day1::EXAMPLE);
  const auto& [part_1, part_2] = aoc23::day1::solve(input);
  REQUIRE(part_1 == 0);
  REQUIRE(part_2 == 0);
}