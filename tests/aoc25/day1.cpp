
#include "day1.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <string_view>
#include <utility>
#include <vector>

using aoc25::day1::Input;
using enum aoc25::day1::Direction;
using inputs::FileReader;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    const auto direction = (*line)[0] == 'L' ? Left : Right;
    const auto count = std::strtoull(line->substr(1).c_str(), nullptr, 10);
    input.emplace_back(direction, count);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day1 Example", "[AoC25_Day1]")
{
  const auto input = readInput(inputs::day1::EXAMPLE);
  const auto [part1, part2] = aoc25::day1::solve(input);
  REQUIRE(part1 == 3);
  REQUIRE(part2 == 6);
}

TEST_CASE("Aoc25 Day1 Input", "[AoC25_Day1]")
{
  const auto input = readInput(inputs::day1::INPUT);
  const auto [part1, part2] = aoc25::day1::solve(input);
  REQUIRE(part1 == 997);
  REQUIRE(part2 == 5978);
}
