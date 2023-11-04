#include "day18.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstddef>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>

using aoc22::day18::Cube;
using aoc22::day18::Input;

Input
readInput(const std::string_view path_)
{
  std::ifstream ifs{ path_.data() };
  Input input{};
  Cube cube{}; //NOLINT

  static constexpr std::array indexToCoord = { &Cube::x, &Cube::y, &Cube::z };

  const std::regex numberRegex{ "(\\d+)" };
  const auto endRegexIt = std::sregex_iterator();

  std::string line{};
  while (std::getline(ifs, line)) {
    std::size_t index = 0;
    auto coordIt = std::sregex_iterator(line.begin(), line.end(), numberRegex);
    while (coordIt != endRegexIt) {
      cube.*indexToCoord[index] = std::stoi(coordIt->str());
      ++index;
      ++coordIt;
    }
    input.push_back(cube);
  }
  return input;
}

TEST_CASE("Day18 Example", "[Day18]")
{
  const auto input = readInput(inputs::day18::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day18::solve(input);
  REQUIRE(part_1 == 64);
  REQUIRE(part_2 == 58);
}

TEST_CASE("Day18 Input", "[Day18]")
{
  const auto input = readInput(inputs::day18::INPUT);
  const auto& [part_1, part_2] = aoc22::day18::solve(input);
  REQUIRE(part_1 == 4390);
  REQUIRE(part_2 == 2534);
}
