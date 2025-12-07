module;

#include <cstddef>
#include <cstdint>
#include <optional>
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

struct Point
{
  std::size_t x;
  std::size_t y;

  bool operator<(const Point& other) const
  {
    return (y < other.y) || (y == other.y && x < other.x);
  }
};

using Matrix = aoc::matrix::Matrix<TileEx>;

namespace {
auto
tryUpdate(Matrix& matrix)
{
  std::uint64_t splittedCount = 0;
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
  return updated ? std::optional<std::uint64_t>{ splittedCount } : std::nullopt;
}

auto
countSplits(const Matrix& matrix)
{
  Matrix tmpMatrix = matrix;
  const auto start = tmpMatrix.find(TileEx::Start)[0];
  tmpMatrix[start.x, start.y] = TileEx::Beam;
  std::uint64_t count = 0;
  while (const auto splitted = tryUpdate(tmpMatrix)) {
    count += *splitted;
  }
  return count;
}

auto
countUniqueTimelines(const Matrix& matrix)
{
  const auto start = matrix.find(TileEx::Start)[0];
  aoc::matrix::Matrix dp{ { matrix.width(), matrix.height() }, 0ULL };
  for (std::size_t x = 0; x < matrix.width(); ++x) {
    dp[x, matrix.height() - 1] = 1;
  }

  for (std::size_t i = 1; i < matrix.height(); ++i) {
    const std::size_t y = matrix.height() - 1 - i;
    for (std::size_t x = 0; x < matrix.width(); ++x) {
      if (matrix[x, y] == TileEx::Empty || matrix[x, y] == TileEx::Start) {

        if (matrix[x, y + 1] != TileEx::Splitter) {
          dp[x, y] += dp[x, y + 1];
        } else {
          if (x > 0) {
            dp[x, y] += dp[x - 1, y + 1];
          }
          if (x < matrix.width() - 1) {
            dp[x, y] += dp[x + 1, y + 1];
          }
        }
      }
    }
  }
  return dp[start.x, start.y]; //
}
}

namespace aoc25::day7 {
Result
solve(const Input& input)
{
  const  Matrix matrix(input, [](Tile t) { return static_cast<TileEx>(t); });
  const std::uint64_t part1 = countSplits(matrix);
  const std::uint64_t part2 = countUniqueTimelines(matrix);
  return { part1, part2 };
}
}
