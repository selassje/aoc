
#include "aoc23/day4.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using aoc23::day4::Card;
using aoc23::day4::Input;

auto
split(const std::string& string, const char delimeter)
{
  std::vector<std::string> parts{};
  std::size_t searchOffset = 0;
  std::size_t nextDelimeter = string.find(delimeter);
  while (nextDelimeter != std::string::npos) {
    const auto part = string.substr(searchOffset, nextDelimeter - searchOffset);
    parts.push_back(part);
    searchOffset = nextDelimeter + 1;
    nextDelimeter = string.find(delimeter, searchOffset);
  }
  const auto lastPart =
    string.substr(searchOffset, string.size() - searchOffset);
  parts.push_back(lastPart);
  return parts;
};

auto
readNumbers(const std::string& string)
{
  std::vector<std::size_t> numbers{};
  const auto numbersStr = split(string, ' ');
  for (const auto& numberStr : numbersStr) {
    numbers.push_back(std::strtoull(numberStr.c_str(), nullptr, 10));
  }
  numbers.erase(numbers.begin());
  return numbers;
}

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  std::string line{};
  while (std::getline(ifs, line)) {
    Card card{};
    const auto listsStr = split(line, ':')[1];
    const auto numbersStrs = split(listsStr, '|');
    card.winningNumbers = readNumbers(numbersStrs[0]);
    card.actualNumbers = readNumbers(numbersStrs[1]);
    input.push_back(card);
  }
  return input;
}

TEST_CASE("Aoc23 Day4 Example", "[AoC23_Day4]")
{
  const auto input = readInput(inputs::day4::EXAMPLE);
  const auto& [part1, part2] = aoc23::day4::solve(input);
  REQUIRE(part1 == 13);
  REQUIRE(part2 == 13);
}

TEST_CASE("Aoc23 Day4 Input", "[AoC23_Day4]")
{
  const auto input = readInput(inputs::day4::INPUT);
  const auto& [part1, part2] = aoc23::day4::solve(input);
  REQUIRE(part1 == 13);
  REQUIRE(part2 == 13);
}