#ifndef AOC23_DAY2_HPP
#define AOC23_DAY2_HPP

#include <cstddef>
#include <vector>

namespace aoc23::day2 {

struct Draw {
  std::size_t redCount;
  std::size_t greenCount;
  std::size_t blueCount;
};

using Game = std::vector<Draw>;

using Input = std::vector<Game>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}
#endif