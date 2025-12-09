
#include "inputs.hpp"
#include "parsing.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>

import aoc25.day8;
import std;

using aoc25::day8::Input;
using aoc25::day8::Junction;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (auto line = fileReader.readLine()) {
    std::ranges::replace(*line, ',', ' ');
    const auto rawJunction =
      inputs::parseLine<std::uint64_t, std::uint64_t, std::uint64_t>(*line);
    const auto junction = Junction{ std::get<0>(rawJunction),
                                    std::get<1>(rawJunction),
                                    std::get<2>(rawJunction) };
    input.push_back(junction);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day8 Example", "[AoC25_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE);
  const auto [part1, part2] = aoc25::day8::solve(input, 10);
  REQUIRE(part1 == 40);
  REQUIRE(part2 == 25272);
}

TEST_CASE("Aoc25 Day8 Input", "[AoC25_Day8]")
{
  const auto input = readInput(inputs::day8::INPUT);
  const auto [part1, part2] = aoc25::day8::solve(input, 1000);
  REQUIRE(part1 == 112230);
  REQUIRE(part2 == 2573952864);
}