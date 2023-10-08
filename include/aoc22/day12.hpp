
#ifndef AOC22_DAY12_HPP
#define AOC22_DAY12_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc22::day12 {

struct Position
{
  std::size_t x;
  std::size_t y;
};

using Grid = std::vector<std::vector<std::byte>>;

struct Input
{
  Position startPosition;
  Position finalPosition;
  Grid grid;
};

struct Result
{
  std::uint64_t Part1;
  std::uint64_t Part2;
};

Result
solve(const Input& grid);

};

#endif