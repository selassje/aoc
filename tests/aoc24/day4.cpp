#include "day4.hpp"

#include "aoc24/inputs.hpp.in"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

namespace   {
  auto readInput(std::string_view path) {
      aoc24::day4::Input input{};
      inputs::FileReader fileReader{path};
      return input;
  }
}

TEST_CASE("Aoc24 Day4 Example", "[AoC24_Day4]")
{
  const auto input = readInput(inputs::day4::EXAMPLE);
  const auto [part1, _] = aoc24::day4::solve(input);
  REQUIRE(part1 == 18);
}