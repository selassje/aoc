
#include "day12.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <ranges>
#include <string_view>

namespace {

using aoc24::day12::Input;

Input
readInput(std::string_view path)
{
  Input input{};
  inputs::FileReader fileReader{ path };
  while (const auto line = fileReader.readLine()) {
    auto row = inputs::parseStringDynamic<unsigned char>(*line);
    input.push_back(row);
  }
  return input;
}
}
TEST_CASE("Aoc24 Day12 Example5", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::EXAMPLE5);
  const auto [_, part2] = aoc24::day12::solve(input);
  REQUIRE(part2 == 368);
}

TEST_CASE("Aoc24 Day12 Example4", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::EXAMPLE4);
  const auto [_, part2] = aoc24::day12::solve(input);
  REQUIRE(part2 == 236);
}
TEST_CASE("Aoc24 Day12 Example1", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::EXAMPLE1);
  const auto [part1, part2] = aoc24::day12::solve(input);
  REQUIRE(part1 == 140);
  REQUIRE(part2 == 80);
}

TEST_CASE("Aoc24 Day12 Example2", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::EXAMPLE2);
  const auto [part1, part2] = aoc24::day12::solve(input);
  REQUIRE(part1 == 772);
  REQUIRE(part2 == 436);
}

TEST_CASE("Aoc24 Day12 Example3", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::EXAMPLE3);
  const auto [part1, part2] = aoc24::day12::solve(input);
  REQUIRE(part1 == 1930);
  REQUIRE(part2 == 1206);
}



TEST_CASE("Aoc24 Day12 Input", "[AoC24_Day12]")
{
  auto input = readInput(inputs::day12::INPUT);
 // const auto [part1, part2] = aoc24::day12::solve(input);
 // REQUIRE(part1 == 1363484);
  //REQUIRE(part2 == 1363484);
}