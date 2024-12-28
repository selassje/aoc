
#include "day9.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

using aoc24::day9::Input;

auto
getInput(std::string_view line)
{

  const auto input = line | std::ranges::views::transform([](auto c) {
                       return static_cast<std::byte>(c - '0');
                     }) |
                     std::ranges::to<std::vector>();
  return input;
}

auto
readInput(std::string_view path)
{
  inputs::FileReader fileReader{ path };
  const auto line = fileReader.readLine();
  return getInput(*line);
}
}

TEST_CASE("Aoc24 Day9 Example", "[AoC24_Day9]")
{
  auto input = getInput("2333133121414131402");
  const auto [part1, part2] = aoc24::day9::solve(input);
  REQUIRE(part1 == 1928);
  REQUIRE(part2 == 1928);
}

TEST_CASE("Aoc24 Day9 Input", "[AoC24_Day9]")
{
  auto input = readInput(inputs::day9::INPUT);
  const auto [part1, part2] = aoc24::day9::solve(input);
  REQUIRE(part1 == 6461289671426);
  REQUIRE(part2 == 6461289671426);
}