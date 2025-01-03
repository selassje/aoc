

#include "day8.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string_view>

namespace {

using aoc24::day8::Input;

Input
readInput(std::string_view path)
{
  Input input{};
  inputs::FileReader fileReader{ path };
  while (auto line = fileReader.readLine()) {
    const auto row = inputs::parseStringDynamic<unsigned char>(*line);
    input.emplace_back(row);
  }
  return input;
}
}

TEST_CASE("Aoc24 Day8 Example2", "[AoC24_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE2);
  const auto [_, part2] = aoc24::day8::solve(input);
  REQUIRE(part2 == 9);
}

TEST_CASE("Aoc24 Day8 Example", "[AoC24_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE);
  const auto [part1, part2] = aoc24::day8::solve(input);
  REQUIRE(part1 == 14);
  REQUIRE(part2 == 34);
}

TEST_CASE("Aoc24 Day8 Input", "[AoC24_Day8]")
{
  const auto input = readInput(inputs::day8::INPUT);
  const auto [part1, part2] = aoc24::day8::solve(input);
  REQUIRE(part1 == 244);
  REQUIRE(part2 == 912);
}
