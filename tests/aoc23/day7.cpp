
#include "aoc23/day7.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>

#include <string_view>
#include <vector>

using aoc23::day7::Card;
using aoc23::day7::Hand;
using aoc23::day7::HandBid;
using aoc23::day7::Input;
using enum aoc23::day7::Card;

Card charToCard(const char c) {
    if ( std::isdigit(c) != 0) {
      return static_cast<Card>(c - '2');
    }
    switch(c) {
      case 'A':
        return Ace;
      case 'K':
        return King;
      case 'Q':
        return Queen;
      case 'J':
        return Jack;
      case 'T':
        return Ten;
      default:
        throw std::runtime_error{"charToCard: Invalid char"};
    }
};

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  Input input{};
  while (std::getline(ifs, line)) {
    HandBid handBid{};
    std::istringstream iss{line};
    std::string handStr{};
    iss >> handStr;
    assert(handStr.size() == 5);
    for ( std::size_t i = 0 ; i < 5; ++i) {
      handBid.hand[i] = charToCard(handStr[i]);
      assert(static_cast<std::size_t>(handBid.hand[i]) <= 12);
    }
    iss >> handBid.bid;
    input.push_back(handBid);
    for ( std::size_t i = 0 ; i < 5; ++i) {
      assert(static_cast<std::size_t>(handBid.hand[i]) <= 12);
    }
  }
  return input;
}

TEST_CASE("Aoc23 Day7 Example", "[AoC23_Day7]")
{
  auto input = readInput(inputs::day7::EXAMPLE);
  const auto& [part1, part2] = aoc23::day7::solve(input);
  REQUIRE(part1 == 6440);
  REQUIRE(part2 == 6440);
}

TEST_CASE("Aoc23 Day7 Input", "[AoC23_Day7]")
{
    const auto input = readInput(inputs::day7::INPUT);
    const auto& [part1, part2] = aoc23::day7::solve(input);
    REQUIRE(part1 == 246163188);
    REQUIRE(part2 == 6440);
}

