
#include "day1.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

using aoc24::day1::Input;
using inputs::FileReader;
using inputs::parseLine;

using Pair = std::pair<std::uint32_t, std::uint32_t>;
namespace {
auto
readInput(std::string_view path)
{
  std::vector<Pair> input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    input.emplace_back(parseLine<std::uint32_t, std::uint32_t>(*line));
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