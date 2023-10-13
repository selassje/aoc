
#ifndef AOC22_DAY14_HPP
#define AOC22_DAY14_HPP

#include <cstddef>
#include <utility>
#include <vector>

namespace aoc22::day14 {

struct Point
{
  std::size_t x;
  std::size_t y;
};

using Path = std::vector<Point>;
using Input = std::vector<Path>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif