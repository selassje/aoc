#include "day4.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace   {
  auto readInput(std::string_view path) {
      aoc24::day4::Input input{};
      inputs::FileReader fileReader{path};
      using  enum aoc24::day4::Letter;
      while (const auto& line = fileReader.readLine()) {
          const auto letters = *line | std::views::transform([](const char c) {
              switch(c) {
                case 'X':
                  return X;
                case 'M':
                  return M;
                case 'A':
                  return A;
                case 'S':
                  return S;
                default:
                  std::unreachable();
              }
          }) | std::ranges::to<std::vector>();
          input.push_back(letters);       
      }
      return input;
  }
}

TEST_CASE("Aoc24 Day4 Example", "[AoC24_Day4]")
{
  const auto input = readInput(inputs::day4::EXAMPLE);
  const auto [part1, _] = aoc24::day4::solve(input);
  REQUIRE(part1 == 18);
}

TEST_CASE("Aoc24 Day4 Input", "[AoC24_Day4]")
{
  const auto input = readInput(inputs::day4::INPUT);
  const auto [part1, _] = aoc24::day4::solve(input);
  REQUIRE(part1 == 18);
}