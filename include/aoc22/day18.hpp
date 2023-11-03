
#ifndef AOC22_DAY18_HPP
#define AOC22_DAY18_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc22::day18 {

struct Cube
{
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
};

using Input = std::vector<Cube>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

};

#endif