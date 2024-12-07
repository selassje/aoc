

#include "day6.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace {

using aoc24::day6::Input;
using aoc24::day6::Tile;

Input
readInput(std::string_view path)
{
  Input input{};
  inputs::FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    const auto row = *line | std::views::transform([](char c) {
      switch (c) {
        case '.':
          return Tile::Empty;
        case '#':
          return Tile::Obstacle;
        case '^':
          return Tile::Guard;
        default:
          std::unreachable();
      }
    }) | std::ranges::to<std::vector>();
    input.push_back(row);
  }
  return input;
}
}

TEST_CASE("Aoc24 Day6 Example", "[AoC24_Day6]")
{
  const auto input = readInput(inputs::day6::EXAMPLE);
  const auto [part1, part2] = aoc24::day6::solve(input);
  REQUIRE(part1 == 41);
  REQUIRE(part2 == 6);
}

#ifndef DEBUG

TEST_CASE("Aoc24 Day6 Input", "[AoC24_Day6]")
{
  const auto input = readInput(inputs::day6::INPUT);
  const auto [part1, part2] = aoc24::day6::solve(input);
  REQUIRE(part1 == 4752);
  REQUIRE(part2 == 1719);
}

#endif