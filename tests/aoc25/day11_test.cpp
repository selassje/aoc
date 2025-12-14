

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>


import aoc25.day11;
import std;

using aoc25::day11::Input;
using aoc25::day11::Connection;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    Connection conn{};
    auto rawStrings = inputs::parseStringDynamic<std::string>(*line);
    rawStrings[0].erase(rawStrings[0].size() - 1,1);
    conn.input  = rawStrings[0];
    for(std::size_t i = 1 ; i < rawStrings.size();++i) {
      conn.outputs.push_back(rawStrings[i]);
    }
    input.push_back(conn);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day11 Example", "[AoC25_Day11]")
{
  const auto input = readInput(inputs::day11::EXAMPLE);
  const auto [part1, part2] = aoc25::day11::solve(input);
  REQUIRE(part1 == 5);
  REQUIRE(part2 == 5);
}

TEST_CASE("Aoc25 Day11 Input", "[AoC25_Day11]")
{
  const auto input = readInput(inputs::day7::INPUT);
  const auto [part1, part2] = aoc25::day11::solve(input);
  REQUIRE(part1 == 5);
  REQUIRE(part2 == 5);
}