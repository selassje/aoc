
#include "day3.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using std::string;
using std::vector;

using Rucksacks = std::vector<std::string>;

Rucksacks
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };

  Rucksacks rucksacks{};

  std::string line{};
  while (std::getline(ifs, line)) {
    rucksacks.emplace_back(line);
  }
  return rucksacks;
}
TEST_CASE("Day3 Example", "[Day3]")
{
  const vector<string> input = {
    "vJrwpWtwJgWrhcsFMMfFFhFp", "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
    "PmmdzqPrVvPwwTWBwg",       "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
    "ttgJtRGJQctTZtZT",         "CrZsJsPPZsGzwwsLwLmpwMDw"
  };

  const auto &[part_1, part_2] = aoc22::day3::solve(input);
  REQUIRE(157 == part_1);
  REQUIRE(70 == part_2);
}

TEST_CASE("Day3 Input", "[Day3]")
{
  const auto input = readInput(inputs::day3::INPUT);
  const auto &[part_1, part_2] = aoc22::day3::solve(input);

  REQUIRE(part_1 == 8243);
  REQUIRE(part_2 == 2631);
}