
#include "day17.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>

using aoc22::day17::Input;
using aoc22::day17::JetDir;

using enum JetDir;

namespace {

Input
readInput(const std::string_view path_)
{
  std::ifstream ifs{ path_.data() };
  Input input{};

  std::string line{};
  std::getline(ifs, line);
  for (const char jet : line) {
    const JetDir jetDir = jet == '<' ? Left : Right;
    input.push_back(jetDir);
  }
  return input;
}
}
TEST_CASE("Day17 Example", "[Day17]")
{
  const auto input = readInput(inputs::day17::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day17::solve(input);
  REQUIRE(part_1 == 3068);
  REQUIRE(part_2 == 1514285714288);
}

TEST_CASE("Day17 Input", "[Day17]")
{
  const auto input = readInput(inputs::day17::INPUT);
  const auto& [part_1, part_2] = aoc22::day17::solve(input);
  REQUIRE(part_1 == 3106);
  REQUIRE(part_2 == 1537175792495);
}

TEST_CASE("Day17 Input2", "[Day17]")
{
  const auto input = readInput(inputs::day17::INPUT2);
  const auto& [part_1, part_2] = aoc22::day17::solve(input);
  REQUIRE(part_1 == 3102);
}
