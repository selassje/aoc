
#include "day5.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstdint>
#include <string_view>
#include <vector>

using aoc25::day5::Input;
using aoc25::day5::Range;
using aoc25::day5::Ranges;
using inputs::FileReader;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  FileReader fileReader{ path };
  bool readFreshIngredients = true;
  while (const auto& line = fileReader.readLine()) {
    if (line->empty()) {
      readFreshIngredients = false;
      continue;
    }
    if (readFreshIngredients) {
      auto lineCopy = *line;
      std::ranges::replace(lineCopy, '-', ' ');
      const auto rawRange = inputs::parseStringDynamic<std::uint64_t>(lineCopy);
      input.freeshIngredients.emplace_back(Range{ rawRange[0], rawRange[1] });
    } else {
      const auto id = *inputs::parseStringDynamic<std::uint64_t>(*line).begin();
      input.availableIngredientIds.emplace_back(id);
    }
  }
  return input;
}
}

TEST_CASE("Aoc25 Day5 Example", "[AoC25_Day5]")
{
  const auto input = readInput(inputs::day5::EXAMPLE);
  const auto [part1, part2] = aoc25::day5::solve(input);
  REQUIRE(part1 == 3);
  REQUIRE(part2 == 14);
}

TEST_CASE("Aoc25 Day5 Input", "[AoC25_Day5]")
{
  const auto input = readInput(inputs::day5::INPUT);
  const auto [part1, part2] = aoc25::day5::solve(input);
  REQUIRE(part1 == 640);
  REQUIRE(part2 == 365804144481581);
}