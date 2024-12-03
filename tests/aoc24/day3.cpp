
#include "day3.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <string_view>

#include <catch2/catch_test_macros.hpp>

namespace {

auto
readInput(std::string_view path)
{
  inputs::FileReader fileReader{ path };
  return fileReader.read();
}

}

TEST_CASE("Aoc24 Day3 Example", "[AoC24_Day3]")
{
  const auto* input =
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
  const auto [part1, part2] = aoc24::day3::solve(input);
  REQUIRE(part1 == 161);
  REQUIRE(part2 == 161);
}

TEST_CASE("Aoc24 Day3 Input", "[AoC24_Day3]")
{
  const auto input = readInput(inputs::day3::INPUT);
  const auto [part1, part2] = aoc24::day3::solve(input);
  REQUIRE(part1 == 175615763);
  REQUIRE(part2 == 175615763);
}