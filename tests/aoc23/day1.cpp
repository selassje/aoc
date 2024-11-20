
#include "day1.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>

using aoc23::day1::Input;

namespace {

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};

  std::string line{};
  while (std::getline(ifs, line)) {
    input.emplace_back(line);
  }
  return input;
}
}
TEST_CASE("Aoc23 Day1 Example", "[AoC23_Day1]")
{
  const auto input = readInput(inputs::day1::EXAMPLE);
  const auto& [part_1, _] = aoc23::day1::solve(input);
  REQUIRE(part_1 == 142);
}

TEST_CASE("Aoc23 Day1 Example2", "[AoC23_Day1]")
{
  const auto input = readInput(inputs::day1::EXAMPLE2);
  const auto& [_, part_2] = aoc23::day1::solve(input);
  REQUIRE(part_2 == 281);
}

TEST_CASE("Aoc23 Day1 Input", "[AoC23_Day1]")
{
  const auto input = readInput(inputs::day1::INPUT);
  const auto& [part_1, part_2] = aoc23::day1::solve(input);
  REQUIRE(part_1 == 55621);
  REQUIRE(part_2 == 53592);
}