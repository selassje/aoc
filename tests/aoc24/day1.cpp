
#include "day1.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using aoc24::day1::Input;
using Pair = std::pair<std::uint32_t, std::uint32_t>;

namespace {

auto
readInput(std::string_view path)
{
  std::vector<Pair> input{};
  std::ifstream ifs{ path.data() };
  std::string line{};
  while (std::getline(ifs, line)) {
    Pair pair{};
    std::stringstream streamLine{ line };
    streamLine >> pair.first >> pair.second;
    input.push_back(pair);
  }
  return input;
}

}

TEST_CASE("Aoc24 Day1 Example", "[AoC24_Day1]")
{
  const auto input = readInput(inputs::day1::EXAMPLE);
  const auto [part1, part2] = aoc24::day1::solve(input);
  REQUIRE(part1 == 11);
  REQUIRE(part2 == 31);
}

TEST_CASE("Aoc24 Day1 Input", "[AoC24_Day1]")
{
  const auto input = readInput(inputs::day1::INPUT);
  const auto [part1, part2] = aoc24::day1::solve(input);
  REQUIRE(part1 == 2176849);
  REQUIRE(part2 == 23384288);
}