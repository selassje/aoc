
#include "aoc23/day3.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using aoc23::day3::Input;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  std::string line{};
  while (std::getline(ifs, line)) {
    input.push_back(line);
  }
  return input;
}

TEST_CASE("Aoc23 Day3 Example", "[AoC23_Day3]")
{
  const auto input = readInput(inputs::day3::EXAMPLE);
  const auto& [part1, part2] = aoc23::day3::solve(input);
  REQUIRE(part1 == 114);
  REQUIRE(part2 == 114);
}

TEST_CASE("Aoc23 Day3 Input", "[AoC23_Day3]")
{
  const auto input = readInput(inputs::day3::INPUT);
  const auto& [part1, part2] = aoc23::day3::solve(input);
  REQUIRE(part1 == 114);
  REQUIRE(part2 == 114);
}