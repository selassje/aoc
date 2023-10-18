
#ifndef AOC22_DAY15_HPP
#define AOC22_DAY15_HPP

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace aoc22::day15 {

struct Point
{
  std::int32_t x;
  std::int32_t y;
};

struct Record
{
  Point sensor;
  Point beacon;
};

using Input = std::vector<Record>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif