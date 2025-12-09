
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

import aoc25.day9;
import std;

using aoc25::day9::Point;
using aoc25::day9::Input;

namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (auto line = fileReader.readLine()) {
    std::ranges::replace(*line, ',', ' ');
    const  auto point =  inputs::parseLine<std::uint64_t,std::uint64_t>(*line);
    input.push_back({std::get<0>(point), std::get<1>(point)});
  }
  return input;
}
}

TEST_CASE("Aoc25 Day9 Example", "[AoC25_Day9]")
{
  const auto input = readInput(inputs::day9::EXAMPLE);
  const auto [part1, part2] = aoc25::day9::solve(input);
  REQUIRE(part1 == 21);
  REQUIRE(part2 == 40);
}

TEST_CASE("Aoc25 Day9 Input", "[AoC25_Day9]")
{
  const auto input = readInput(inputs::day9::INPUT);
  const auto [part1, part2] = aoc25::day9::solve(input);
  REQUIRE(part1 == 1566);
  REQUIRE(part2 == 5921061943075);
}