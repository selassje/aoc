#ifndef AOC22_DAY9_HPP
#define AOC22_DAY9_HPP

#include <cstdint>
#include <cstdlib>
#include <utility>
#include <vector>

namespace aoc22::day9 {

enum class Direction : std::uint8_t
{
  Left,
  Right,
  Up,
  Down
};

struct alignas(16) Move
{
  Direction direction;
  std::size_t count;
};

using Moves = std::vector<Move>;

std::pair<std::size_t, std::size_t>
solve(const Moves& moves);

};

#endif