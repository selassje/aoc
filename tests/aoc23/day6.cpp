
#include "aoc23/day6.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using aoc23::day6::Input;

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
  const auto stringRepl = std::regex_replace(string, std::regex(" +"), " ");
  const auto numbersStr = split(stringRepl.substr(1), ' ');
  for (const auto& numberStr : numbersStr) {
    const auto number = std::strtoull(numberStr.c_str(), nullptr, 10);
    numbers.push_back(number);
  }
  return numbers;
}

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  std::vector<std::size_t> distances{};
  std::vector<std::size_t> recordTimes{};
  while (std::getline(ifs, line)) {
      const auto numberStr = split(line,':')[1];
      if ( line.starts_with("Time:")) {
        recordTimes = readNumbers(numberStr);
      } else {
        distances = readNumbers(numberStr);
      }
  }
  Input input{};
  for ( std::size_t i = 0 ; i < distances.size(); ++i) {
      input.emplace_back(distances[i], recordTimes[i]);
  }
  return input;
}


TEST_CASE("Aoc23 Day6 Example", "[AoC23_Day6]")
{
  const auto input = readInput(inputs::day6::EXAMPLE);
  const auto& [part1, part2] = aoc23::day6::solve(input);
  REQUIRE(part1 == 288);
  REQUIRE(part2 == 288);
}

TEST_CASE("Aoc23 Day6 Input", "[AoC23_Day6]")
{
  const auto input = readInput(inputs::day6::INPUT);
  const auto& [part1, part2] = aoc23::day6::solve(input);
  REQUIRE(part1 == 0);
  REQUIRE(part2 == 0);
}

