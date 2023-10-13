
#include "day14.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <regex>
#include <string>
#include <string_view>

using aoc22::day14::Input;
using aoc22::day14::Path;
using aoc22::day14::Point;

Input
readInput(const std::string_view path_)
{
  std::ifstream ifs{ path_.data() };
  Input input{};
  Path path{};
  Point point{};

  const std::regex regex("(\\d+)");
  const auto endRegexIt = std::sregex_iterator();

  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      bool nextX = true;
      auto numbersBegin = std::sregex_iterator(line.begin(), line.end(), regex);   
      while ( numbersBegin != endRegexIt) {
        const auto number = std::strtoull(numbersBegin->str().c_str(), nullptr, 10);
        if (nextX) {
            point.x = number;          
        } else {
            point.y = number;
            path.push_back(point);
        }
        ++numbersBegin;
        nextX = !nextX;
      }
      input.push_back(path);
      path.clear();
    }
  }
  return input;
}

TEST_CASE("Day14 Example", "[Day14]")
{
  const auto input = readInput(inputs::day14::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day14::solve(input);
  REQUIRE(part_1 == 24);
  REQUIRE(part_2 == 93);
}

TEST_CASE("Day14 Input", "[Day14]")
{
  const auto input = readInput(inputs::day14::INPUT);
  const auto& [part_1, part_2] = aoc22::day14::solve(input);
  REQUIRE(part_1 == 885);
  REQUIRE(part_2 == 885);
}