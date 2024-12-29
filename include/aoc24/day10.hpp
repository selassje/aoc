
#ifndef AOC24_DAY10_HPP
#define AOC24_DAY10_HPP

#include <cstddef>
#include <vector>

namespace aoc24::day10 {

using Input = std::vector<std::vector<std::byte>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif