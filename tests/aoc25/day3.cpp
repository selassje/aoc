
#include "day3.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string_view>
#include <vector>

using aoc25::day3::Input;
using inputs::FileReader;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    const auto numbers = inputs::parseStringDynamic<std::uint8_t>(*line);
    input.emplace_back(numbers);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day3 Example", "[AoC25_Day3]")
{
  const auto input = readInput(inputs::day3::EXAMPLE);
  const auto [part1, part2] = aoc25::day3::solve(input);
  REQUIRE(part1 == 357);
  REQUIRE(part2 == 3121910778619);
}

TEST_CASE("Aoc25 Day3 Input", "[AoC25_Day3]")
{
  const auto input = readInput(inputs::day3::INPUT);
  const auto [part1, part2] = aoc25::day3::solve(input);
  REQUIRE(part1 == 17445);
  REQUIRE(part2 == 173229689350551);
}