
#include "day12.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <iterator>
#include <sstream>
#include <string_view>
#include <vector>

using aoc22::day12::Grid;
using aoc22::day12::Input;

Input
readInput(const std::string_view path)
{
  Input input{};
  std::ifstream ifs{ path.data() };
  std::string line{};

  while (std::getline(ifs, line)) {
    std::istringstream iss{ line };
  }
  return input;
}

TEST_CASE("Day12 Example", "[Day12]")
{
  const auto input = readInput(inputs::day12::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day12::solve(input);
  REQUIRE(part_1 == 31);
  REQUIRE(part_2 == 31);
}