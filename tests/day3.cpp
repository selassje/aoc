
#include "day3.hpp"
#include "inputs.hpp"
#include <catch2/catch.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

  REQUIRE(157 == aoc22::day3::solve(input));
}

TEST_CASE("Day3 Input", "[Day3]")
{
  const auto input = readInput(inputs::day3::INPUT);

  REQUIRE(8243 == aoc22::day3::solve(input));
}