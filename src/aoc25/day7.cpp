module;

#include <cstddef>
#include <cstdint>
#include <flat_map>
#include <print>
#include <set>
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

using Path = std::vector<Point>;
using UniquePaths = std::set<Path>;
using PositionToSplitterMap = std::flat_map<Point, std::size_t>;

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
  Matrix originalMatrix = matrix;
  const auto start = matrix.find(TileEx::Start)[0];
  matrix[start.x, start.y] = TileEx::Beam;

  std::uint64_t part1 = 0;
  while (tryUpdate(matrix, part1)) {
    // keep updating
  }
 aoc::matrix::Matrix dp{{ originalMatrix.width(), originalMatrix.height() }, 0ULL };

  for(std::size_t x = 0; x < matrix.width(); ++x) {
      dp[x, matrix.height() - 1] = 1;
  }

  for (std::size_t i = 1; i < matrix.height(); ++i) {
    std::size_t y = matrix.height() - 1 - i;
    for (std::size_t x = 0; x < matrix.width(); ++x) {
      if (originalMatrix[x, y] == TileEx::Empty ||  originalMatrix[x, y] == TileEx::Start) {
          //originalMatrix[x, y] == TileEx::Splitter) 
        
        
        if(originalMatrix[x, y + 1] != TileEx::Splitter) {
          dp[x, y] += dp[x, y + 1];
        }
        else {
          if(x > 0) {
            dp[x, y] += dp[x - 1, y + 1];
          }
          if(x < matrix.width() - 1) {
            dp[x, y] += dp[x + 1, y + 1];
          }
        }  

      }
    }
  }
  const std::uint64_t part2 = dp[start.x, start.y];//
 // part2 = dp[start.x, matrix.height() - 1];
  return { part1, part2 };
}
}
