
#include "day14.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>

using aoc22::day14::Input;
using aoc22::day14::Path;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  Path pair{};

  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
    }
  }
  return input;
}

TEST_CASE("Day14 Example", "[Day14]")
{
  const auto input = readInput(inputs::day14::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day14::solve(input);
  REQUIRE(part_1 == 24);
  REQUIRE(part_2 == 24);
}
