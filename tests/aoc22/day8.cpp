#include "day8.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>

using aoc22::day8::Grid;

namespace {

Grid
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Grid grid{};

  std::string line{};
  while (std::getline(ifs, line)) {
    auto asBytes = line | std::ranges::views::transform([](const char& c) {
                     return static_cast<std::byte>(c - '0');
                   });
    grid.emplace_back();
    std::ranges::copy(asBytes, std::back_inserter(grid.back()));
  }
  return grid;
}
}
TEST_CASE("Day8 Example", "[Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day8::solve(input);
  REQUIRE(part_1 == 21);
  REQUIRE(part_2 == 8);
}

TEST_CASE("Day8 Input", "[Day8]")
{
  const auto input = readInput(inputs::day8::INPUT);
  const auto& [part_1, part_2] = aoc22::day8::solve(input);
  REQUIRE(part_1 == 1533);
  REQUIRE(part_2 == 345744);
}