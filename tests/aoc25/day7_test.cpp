
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string_view>
#include <vector>

import aoc25.day7;

using aoc25::day7::Tile;
using enum aoc25::day7::Tile;
using aoc25::day7::Input;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (const auto &line = fileReader.readLine()) {
    std::vector<Tile> row{};
    for (const auto& ch : *line) {
      switch (ch) {
        case '.':
          row.push_back(Empty);
          break;
        case 'S':
          row.push_back(Start);
          break;
        case '^':
          row.push_back(Splitter);
          break;
        default:
          break;
      }
    }
    input.push_back(row);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day7 Example", "[AoC25_Day7]")
{
  const auto input = readInput(inputs::day7::EXAMPLE);
  const auto [part1, part2] = aoc25::day7::solve(input);
  REQUIRE(part1 == 21);
  REQUIRE(part2 == 40);
}

TEST_CASE("Aoc25 Day7 Input", "[AoC25_Day7]")
{
    const auto input = readInput(inputs::day7::INPUT);
    const auto [part1, part2] = aoc25::day7::solve(input);
    REQUIRE(part1 == 1566);
    REQUIRE(part2 == 5921061943075);
}
