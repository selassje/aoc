
#ifndef AOC22_DAY12_HPP
#define AOC22_DAY12_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc22::day12 {

using IndexType =
  std::conditional_t<sizeof(std::size_t) == 8, std::uint32_t, std::uint16_t>;

struct Position
{
  IndexType x;
  IndexType y;
};

using Grid = std::vector<std::vector<char>>;

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