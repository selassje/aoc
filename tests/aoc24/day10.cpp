
#include "day10.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

using aoc24::day10::Input;

Input
readInput(std::string_view path)
{
  Input input{};
  inputs::FileReader fileReader{ path };
  while (const auto line = fileReader.readLine()) {
    auto row = inputs::parseStringDynamic<unsigned char>(*line) |
               std::ranges::views::transform(
                 [](const auto c) { return static_cast<std::byte>(c - '0'); }) |
               std::ranges::to<std::vector>();
    input.push_back(row);
  }
  return input;
}
}

TEST_CASE("Aoc24 Day10 Example", "[AoC24_Day10]")
{
  auto input = readInput(inputs::day10::EXAMPLE);
  const auto [part1, part2] = aoc24::day10::solve(input);
  REQUIRE(part1 == 36);
  REQUIRE(part2 == 81);
}

TEST_CASE("Aoc24 Day10 Input", "[AoC24_Day10]")
{
  auto input = readInput(inputs::day10::INPUT);
  const auto [part1, part2] = aoc24::day10::solve(input);
  REQUIRE(part1 == 825);
  REQUIRE(part2 == 1805);
}