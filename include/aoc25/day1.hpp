
#ifndef AOC25_DAY1_HPP
#define AOC25_DAY1_HPP

#include <cstdint>

namespace aoc25::day1 {

using Input = int;

struct Result
{
  std::uint32_t part1;
  std::uint32_t part2;
};

Result
solve(Input input);

}

#endif