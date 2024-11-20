#ifndef AOC23_DAY7_HPP
#define AOC23_DAY7_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc23::day7 {

enum class Card : std::uint8_t
{
  Ace = 13,
  King = 12,
  Queen = 11,
  JackOrJoker = 10,
  Ten = 9,
  Nine = 8,
  Eight = 7,
  Seven = 6,
  Six = 5,
  Five = 4,
  Four = 3,
  Three = 2,
  Two = 1
};

using Hand = std::array<Card, 5>; // NOLINT

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