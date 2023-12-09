

#include "aoc23/day9.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

using aoc23::day9::Input;
using aoc23::day9::Values;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  Input input{};
  while (std::getline(ifs, line)) {
    Values values{};
    std::istringstream iss{ line };
    std::string handStr{};
    while(!iss.eof()) {
        std::int64_t value;
        iss >> value;
        values.push_back(value);  
    }
    input.push_back(values);
  }
  return input;
}

TEST_CASE("Aoc23 Day9 Example", "[AoC23_Day9]")
{
  auto input = readInput(inputs::day9::EXAMPLE);
  const auto& [part1, part2] = aoc23::day9::solve(input);
  REQUIRE(part1 == 114);
  REQUIRE(part2 == 114);
}

TEST_CASE("Aoc23 Day9 Input", "[AoC23_Day9]")
{
  const auto input = readInput(inputs::day9::INPUT);
  const auto& [part1, part2] = aoc23::day9::solve(input);
  REQUIRE(part1 == 114);
  REQUIRE(part2 == 114);
}