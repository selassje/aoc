#ifndef AOC23_DAY7_HPP
#define AOC23_DAY7_HPP

#include <array>
#include <cstddef>
#include <vector>

namespace aoc23::day7 {

enum class Card : std::size_t
{
  Ace = 12,
  King = 11,
  Queen = 10,
  Jack = 9,
  Ten = 8,
  Nine = 7,
  Eight = 6,
  Seven = 5,
  Six = 4,
  Five = 3,
  Four = 2,
  Three = 1,
  Two = 0
};

using Hand = std::array<Card, 5>;

struct HandBid
{
  Hand hand;
  std::size_t bid;
};

using Input = std::vector<HandBid>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);
}

#endif