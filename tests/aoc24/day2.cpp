
#include "day2.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

namespace {

using inputs::FileReader;
using inputs::parseStringDynamic;
using List = std::vector<std::size_t>;

auto
readInput(std::string_view path)
{
  std::vector<List> input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    input.emplace_back(parseStringDynamic<std::size_t>(*line));
  }
  return input;
}

}

TEST_CASE("Aoc24 Day2 Example", "[AoC24_Day2]")
{
  const auto input = readInput(inputs::day2::EXAMPLE);
  const auto [part1, part2] = aoc24::day2::solve(input);
  REQUIRE(part1 == 2);
  REQUIRE(part2 == 2);
}