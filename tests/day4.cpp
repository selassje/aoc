#include "day4.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

using aoc22::day4::Input;
using aoc22::day4::Pair;
using aoc22::day4::Range;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};

  std::string line{};
  while (std::getline(ifs, line)) {

    std::replace(line.begin(), line.end(), '-', ' ');
    std::replace(line.begin(), line.end(), ',', ' ');

    std::istringstream iss{ line };

    Range elf1{};
    Range elf2{};

    iss >> elf1.first >> elf1.second >> elf2.first >> elf2.second;
    input.emplace_back(elf1, elf2);
  }
  return input;
}

TEST_CASE("Day4 Example", "[Day4]")
{
  const auto input = readInput(inputs::day4::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day4::solve(input);
  REQUIRE(part_1 == 2);
  REQUIRE(part_2 == 4);
}

TEST_CASE("Day4 Input", "[Day4]")
{
  const auto input = readInput(inputs::day4::INPUT);
  const auto& [part_1, part_2] = aoc22::day4::solve(input);
  REQUIRE(part_1 == 509);
  REQUIRE(part_2 == 870);
}
};