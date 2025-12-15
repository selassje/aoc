#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

import aoc25.day12;
import std;

using aoc25::day12::Input;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (const auto& line = fileReader.readLine()) {
  }
  return input;
}
}

TEST_CASE("Aoc25 Day12 Example", "[AoC25_Day12]")
{
  const auto input = readInput(inputs::day12::EXAMPLE);
  const auto [part1, part2] = aoc25::day12::solve(input);
  REQUIRE(part1 == 2);
  REQUIRE(part2 == 2);
}


TEST_CASE("Aoc25 Day12 Input", "[AoC25_Day12]")
{
  const auto input = readInput(inputs::day12::INPUT);
  const auto [part1, part2] = aoc25::day12::solve(input);
  REQUIRE(part1 == 662);
  REQUIRE(part2 == 429399933071120);
}