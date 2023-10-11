#include "day13.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using aoc22::day13::Input;
using aoc22::day13::Pair;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  Pair pair{};

  bool nextLineIsFirst = true;
  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      if (nextLineIsFirst) {
        pair.first = line;
      } else {
        pair.second = line;
        input.push_back(pair);
      }
      nextLineIsFirst = !nextLineIsFirst;
    }
  }
  return input;
}

TEST_CASE("Day13 Input", "[Day13]")
{
  const auto input = readInput(inputs::day13::INPUT);
  const auto& [part_1, part_2] = aoc22::day13::solve(input);
  REQUIRE(part_1 == 13);
 // REQUIRE(part_2 == 13);
}

TEST_CASE("Day13 Example", "[Day13]")
{
  const auto input = readInput(inputs::day13::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day13::solve(input);
  REQUIRE(part_1 == 5555);
  //REQUIRE(part_2 == 13);
}
