#include "aoc23/day8.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using aoc23::day8::Direction;
using aoc23::day8::Directions;
using aoc23::day8::Input;
using aoc23::day8::Node;
using aoc23::day8::Nodes;
using enum aoc23::day8::Direction;

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
  Input input{};
  std::getline(ifs, line);
  for (const char c : line) {
    input.directions.push_back(c == 'L' ? Left : Right);
  }

  Nodes nodes{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      Node node{};
      const auto splitStrs = split(line, '=');
      const auto id = splitStrs[0].substr(0, splitStrs[0].size() - 1);
      const auto dirsStr = splitStrs[1].substr(1, splitStrs[1].size() - 1);
      const auto dirs = split(dirsStr, ',');
      const auto left = dirs[0].substr(1);
      const auto right = dirs[1].substr(1, dirs[0].size() - 1);

      node.id = id;
      node.left = left;
      node.right = right;
      input.nodes.push_back(node);
    }
  }
  return input;
}

TEST_CASE("Aoc23 Day8 Example", "[AoC23_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE);
  const auto& [part1, _] = aoc23::day8::solve(input);
  REQUIRE(part1 == 2);
}

TEST_CASE("Aoc23 Day8 Example2", "[AoC23_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE2);
  const auto& [part1, _] = aoc23::day8::solve(input);
  REQUIRE(part1 == 6);
}

TEST_CASE("Aoc23 Day8 Example3", "[AoC23_Day8]")
{
  const auto input = readInput(inputs::day8::EXAMPLE3);
  const auto& [_, part2] = aoc23::day8::solve(input);
  REQUIRE(part2 == 6);
}

TEST_CASE("Aoc23 Day8 Input", "[AoC23_Day8]")
{
  const auto input = readInput(inputs::day8::INPUT);
  const auto& [part1, part2] = aoc23::day8::solve(input);
  REQUIRE(part1 == 12643);
  REQUIRE(part2 == 13133452426987);
}
