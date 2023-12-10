
#include "aoc23/day10.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

using aoc23::day10::Input;
using aoc23::day10::PipeType;
using enum aoc23::day10::PipeType;

PipeType
charToPipeType(const char c)
{
  switch (c) {
    case '|':
      return NorthSouth;
    case '-':
      return EastWest;
    case 'L':
      return NorthEast;
    case 'J':
      return NorthWest;
    case '7':
      return SouthWest;
    case 'F':
      return SouthEast;
    case '.':
      return Ground;
    case 'S':
      return Start;
    default:
      std::abort();
  }
}

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  Input input{};
  while (std::getline(ifs, line)) {
    std::vector<PipeType> row{};
    for ( const char c : line ){
      row.push_back(charToPipeType(c));
    }
    input.push_back(row);
  }
  return input;
}

TEST_CASE("Aoc23 Day10 Example", "[AoC23_Day10]")
{
  auto input = readInput(inputs::day10::EXAMPLE);
  const auto& [part1, part2] = aoc23::day10::solve(input);
  REQUIRE(part1 == 4);
  REQUIRE(part2 == 4);
}

TEST_CASE("Aoc23 Day10 Example2", "[AoC23_Day10]")
{
  auto input = readInput(inputs::day10::EXAMPLE2);
  const auto& [part1, part2] = aoc23::day10::solve(input);
  REQUIRE(part1 == 8);
  REQUIRE(part2 == 8);
}

TEST_CASE("Aoc23 Day10 Input", "[AoC23_Day10]")
{
  const auto input = readInput(inputs::day10::INPUT);
  const auto& [part1, part2] = aoc23::day10::solve(input);
  REQUIRE(part1 == 6820);
  REQUIRE(part2 == 6820);
}