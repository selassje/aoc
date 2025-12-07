module;
#include <cstdint>
#include <vector>

module aoc25.day7;

import aoc.matrix;

enum class TileEx : std::uint8_t {
  Empty = 0,
  Start,
  Splitter,
  Beam,
};

using Matrix = aoc::matrix::Matrix<TileEx>;

namespace aoc25::day7 {
Result
solve(const Input& input)
{
  Matrix matrix(input, [](Tile t) { return static_cast<TileEx>(t); });


  std::uint64_t part1 = input.size();
  std::uint64_t part2 = 0;
  return {part1, part2};
}
}
