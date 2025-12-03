
#include "day3.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
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
  if (auto line = fileReader.readLine()) {
    std::replace(line->begin(), line->end(), '-', ' ');
    std::replace(line->begin(), line->end(), ',', ' ');
    const auto numbers = inputs::parseStringDynamic<std::uint64_t>(*line);
    for (std::size_t i = 0; i < numbers.size(); i += 2) {
      input.emplace_back(numbers[i], numbers[i + 1]);
    }
  }
  return input;
}
}

TEST_CASE("Aoc25 Day3 Example", "[AoC25_Day3]")
{
  const auto input = readInput(inputs::day3::EXAMPLE);
  const auto [part1, part2] = aoc25::day3::solve(input);
  REQUIRE(part1 == 1227775554);
  REQUIRE(part2 == 4174379265);
}

TEST_CASE("Aoc25 Day3 Input", "[AoC25_Day3]")
{
  // const auto input = readInput(inputs::day3::INPUT);
  // const auto [part1, part2] = aoc25::day3::solve(input);
  // REQUIRE(part1 == 18595663903);
  // REQUIRE(part2 == 19058204438);
}