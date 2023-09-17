#include "day1.hpp"
#include "inputs.hpp"
#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

using std::vector;
using NestedVector = vector<vector<unsigned int>>;

namespace {

NestedVector
readInput(const std::string_view path)
{

  std::ifstream ifs{ path.data() };

  NestedVector input{};
  vector<unsigned int> caloriesPerElf{};

  std::string line{};
  while (std::getline(ifs, line)) {
    if (line.empty()) {
      input.push_back(caloriesPerElf);
      caloriesPerElf.clear();
    } else {
      std::istringstream iss{ line };
      unsigned int calories = 0;
      iss >> calories;
      caloriesPerElf.push_back(calories);
    }
  }
  input.push_back(caloriesPerElf);
  return input;
}

}

TEST_CASE("Day1 Example", "[Day1]")
{
  auto input = readInput(inputs::day1::EXAMPLE);
  auto [part_1, part_2] = aoc22::day1::solve(input);
  REQUIRE(part_1 == 24000);
  REQUIRE(part_2 == 45000);
}

TEST_CASE("Day1 Input", "[Day1]")
{
  auto input = readInput(inputs::day1::INPUT);
  auto [part_1, part_2] = aoc22::day1::solve(input);
  REQUIRE(part_1 == 70613);
  REQUIRE(part_2 == 205805);
}
