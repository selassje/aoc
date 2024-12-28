
#ifndef AOC24_DAY9_HPP
#define AOC24_DAY9_HPP

#include <cstddef>
#include <span>

namespace aoc24::day9 {

using Input = std::span<std::byte>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(Input input);

}

#endif