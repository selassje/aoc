module;

#include <cstddef>
#include <cstdint>
#include <vector>

module aoc25.day7;

import aoc.matrix;

enum class TileEx : std::uint8_t
{
  Empty = 0,
  Start,
  Splitter,
  Beam,
};

using Matrix = aoc::matrix::Matrix<TileEx>;

namespace {
bool
tryUpdate(Matrix& matrix, std::uint64_t& splittedCount)
{
  bool updated = false;
  for (std::size_t y = 0; y < matrix.height(); ++y) {
    for (std::size_t x = 0; x < matrix.width(); ++x) {
      if (matrix[x, y] == TileEx::Beam) {
        if (y < matrix.height() - 1 && matrix[x, y + 1] == TileEx::Empty) {
          matrix[x, y + 1] = TileEx::Beam;
          updated = true;
        }
        bool splitted = false;
        if (y < matrix.height() - 1 && matrix[x, y + 1] == TileEx::Splitter) {
          if (x > 0 && matrix[x - 1, y + 1] == TileEx::Empty) {
            matrix[x - 1, y + 1] = TileEx::Beam;
            updated = true;
            ++splittedCount;
            splitted = true;
          }
          if (x < matrix.width() - 1 && matrix[x + 1, y + 1] == TileEx::Empty) {
            matrix[x + 1, y + 1] = TileEx::Beam;
            updated = true;
            if (!splitted) {
              ++splittedCount;
            }
          }
        }
      }
    }
  }
  return updated;
}

}

namespace aoc25::day7 {
Result
solve(const Input& input)
{
  Matrix matrix(input, [](Tile t) { return static_cast<TileEx>(t); });
  const auto start = matrix.find(TileEx::Start)[0];
  matrix[start.x, start.y] = TileEx::Beam;

  std::uint64_t part1 = 0;
  while (tryUpdate(matrix, part1)) {
    // keep updating
  }

  auto print = [](TileEx t) {
    switch (t) {
      case TileEx::Empty:
        return '.';
      case TileEx::Start:
        return 'S';
      case TileEx::Splitter:
        return '^';
      case TileEx::Beam:
        return '|';
      default:
        return '?';
    }
  };

  //matrix.print(print);

  std::uint64_t part2 = part1;
  return { part1, part2 };
}
}
