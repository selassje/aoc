

#ifndef AOC23_DAY6_HPP
#define AOC23_DAY6_HPP

#include <cstddef>

namespace aoc23::day6 {

struct Input
{
};

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);
}

#endif