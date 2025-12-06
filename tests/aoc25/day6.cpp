
#include "day6.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

using aoc25::day6::Input;
using aoc25::day6::Problem;
using enum aoc25::day6::Operation;
using inputs::FileReader;

namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    if ((*line)[0] == '+' || (*line)[0] == '*') {
      const auto rawOperations = inputs::parseStringDynamic<unsigned char>(*line);
      for (std::size_t i = 0; i < rawOperations.size(); ++i) {
        if (rawOperations[i] == '+') {
            input[i].operation = Add;
        } else {
            input[i].operation = Multiply;
        }
      }
    }
    else {
      const auto numbers = inputs::parseStringDynamic<std::uint64_t>(*line);
      if (input.size() == 0) {
        input.resize(numbers.size());
      }
      for (std::size_t i = 0; i < numbers.size(); ++i) {
        input[i].operands.push_back(numbers[i]);
      }
    }
  }
  return input;
}
}

TEST_CASE("Aoc25 Day6 Example", "[AoC25_Day6]")
{
  const auto input = readInput(inputs::day6::EXAMPLE);
  const auto [part1, part2] = aoc25::day6::solve(input);
  REQUIRE(part1 == 4277556);
  REQUIRE(part2 == 4277556);
}

TEST_CASE("Aoc25 Day6 Input", "[AoC25_Day6]")
{
   const auto input = readInput(inputs::day6::INPUT);
   const auto [part1, part2] = aoc25::day6::solve(input);
   REQUIRE(part1 == 6725216329103);
   REQUIRE(part2 == 6725216329103);
}