#include "day11.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

using aoc24::day11::Input;

Input
readInput(std::string_view path)
{
  inputs::FileReader fileReader{ path };
  if (const auto line = fileReader.readLine(); line) {
    return inputs::parseStringDynamic<std::size_t>(*line);
  }
  return {};
}
}

TEST_CASE("Aoc24 Day11 Example", "[AoC24_Day11]")
{
  auto input = Input{ 125, 17 }; // NOLINT
  const auto [part1, _] = aoc24::day11::solve(input);
  REQUIRE(part1 == 55312);
}

TEST_CASE("Aoc24 Day11 Input", "[AoC24_Day11]")
{
  auto input = readInput(inputs::day11::INPUT);
  const auto [part1, part2] = aoc24::day11::solve(input);
  REQUIRE(part1 == 185894);
  REQUIRE(part2 == 221632504974231);
}