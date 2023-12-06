

#ifndef AOC23_DAY6_HPP
#define AOC23_DAY6_HPP

#include <cstddef>
#include <vector>

namespace aoc23::day6 {

struct Race
{
  std::size_t recordDistance;
  std::size_t time;
};

using Input = std::vector<Race>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);
}

#endif