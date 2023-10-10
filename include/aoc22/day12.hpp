
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
  bool operator==(const Position& position) const noexcept = default;
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
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

};

#endif