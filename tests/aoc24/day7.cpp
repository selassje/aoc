
#include "day7.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace {

using aoc24::day7::Input;

Input
readInput(std::string_view path)
{
  Input input{};
  inputs::FileReader fileReader{ path };
  while (auto line = fileReader.readLine()) {
    std::replace(std::begin(*line), std::end(*line), ':', ' ');
    auto record =  inputs::parseStringDynamic<std::size_t>(*line);
    const auto testValue = record[0];
    record.erase(record.begin()); 
    input.emplace_back(testValue,record);
  }
  return input;
}
}

TEST_CASE("Aoc24 Day7 Example", "[AoC24_Day7]")
{
  const auto input = readInput(inputs::day7::EXAMPLE);
  const auto [part1, part2] = aoc24::day7::solve(input);
  REQUIRE(part1 == 3749);
  REQUIRE(part2 == 3749);
}