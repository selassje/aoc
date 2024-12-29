
#ifndef AOC24_DAY10_HPP
#define AOC24_DAY10_HPP

#include <cstddef>
#include <mdspan>

namespace aoc24::day10 {

using Input = std::mdspan<
  std::byte,
  std::dextents<std::size_t, 2>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(Input input);

}

#endif