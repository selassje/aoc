
#ifndef AOC22_DAY2_HPP
#define AOC22_DAY2_HPP

#include <utility>
#include <vector>

namespace aoc22::day2 {

enum class Move : unsigned char
{
  Rock = 1,
  Paper,
  Sciscors
};

using Guide = std::vector<std::pair<Move, Move>>;

std::pair<unsigned int, unsigned int>
solve(const Guide& guide) noexcept;

}

#endif