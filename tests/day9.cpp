
#include "day9.hpp"
#include "inputs.hpp"

#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <unordered_map>

using aoc22::day9::Direction;
using aoc22::day9::Move;
using aoc22::day9::Moves;
using enum aoc22::day9::Direction;

Moves
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Moves moves{};
  std::string line{};

  static const std::unordered_map<char, Direction> charToDirection{
    { 'R', Right }, { 'U', Up }, { 'L', Left }, { 'D', Down }
  };

  while (std::getline(ifs, line)) {
    std::istringstream iss{ line };
    char d{};
    std::size_t count{};
    iss >> d >> count;
    moves.emplace_back<Move>({ charToDirection.at(d), count });
  }
  return moves;
}

TEST_CASE("Day9 Example", "[Day9]")
{
  const auto input = readInput(inputs::day9::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day9::solve(input);
  REQUIRE(part_1 == 13);
  REQUIRE(part_2 == 1);
}

TEST_CASE("Day9 Example2", "[Day9]")
{
  const auto input = readInput(inputs::day9::EXAMPLE2);
  const auto& [_, part_2] = aoc22::day9::solve(input);
  REQUIRE(part_2 == 36);
}

TEST_CASE("Day9 Input", "[Day9]")
{
  const auto input = readInput(inputs::day9::INPUT);
  const auto& [part_1, part_2] = aoc22::day9::solve(input);
  REQUIRE(part_1 == 6011);
  REQUIRE(part_2 == 2419);
}