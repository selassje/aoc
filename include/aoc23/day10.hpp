
#ifndef AOC23_DAY10_HPP
#define AOC23_DAY10_HPP

#include <array>
#include <cstddef>
#include <vector>

namespace aoc23::day10 {

enum class PipeType
{
  NorthSouth,
  EastWest,
  NorthEast,
  NorthWest,
  SouthWest,
  SouthEast,
  Ground,
  Start,
};

using Input = std::vector<std::vector<PipeType>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif