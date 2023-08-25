#include "day2.hpp"
#include "inputs.hpp"
#include <catch2/catch.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using aoc22::day2::Guide;

using enum aoc22::day2::Move;

Guide
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };

  Guide input;

  std::string line{};
  while (std::getline(ifs, line)) {
    unsigned char oponent{};
    unsigned char you{};
    std::istringstream iss{ line };
    iss >> oponent >> you;

    auto converToMove = [](unsigned char move) {
      switch (move) {
        case 'A':
        case 'X':
          return Rock;
        case 'B':
        case 'Y':
          return Paper;
        case 'C':
        case 'Z':
          return Sciscors;
        default:
          std::terminate();
      }
    };
    input.emplace_back(
      std::make_pair(converToMove(oponent), converToMove(you)));
  }
  return input;
}

TEST_CASE("Day2 Example", "[Day2]")
{
  auto input =
    Guide{ { Rock, Paper }, { Paper, Rock }, { Sciscors, Sciscors } };
  auto part_1 = aoc22::day2::solve(input);
  REQUIRE(part_1 == 15);
}

TEST_CASE("Day2 Input", "[Day2]")
{
  auto input = readInput(inputs::day2::INPUT);
  auto part_1 = aoc22::day2::solve(input);
  REQUIRE(part_1 == 11767);
}