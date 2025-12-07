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

std::optional<Path>
trySplitterCombination(const Matrix& matrix,
                       std::uint64_t combination,
                       Point startPosition,
                       const PositionToSplitterMap& splitterMap)
{
  Matrix tempMatrix = matrix;
  Path path{ startPosition };
  Point currentPosition = startPosition;
  while (currentPosition.y < tempMatrix.height() - 1) {
    if (tempMatrix[currentPosition.x, currentPosition.y + 1] == TileEx::Empty) {
      tempMatrix[currentPosition.x, currentPosition.y + 1] = TileEx::Beam;
      currentPosition.y += 1;
      path.push_back(currentPosition);
    } else if (tempMatrix[currentPosition.x, currentPosition.y + 1] ==
               TileEx::Splitter) {
      Point splitterPoint{ currentPosition.x, currentPosition.y + 1 };
      const auto splitterIndex = splitterMap.find(splitterPoint)->second;
      const bool goLeft = (combination & (1ULL << splitterIndex)) == 0;
      if (goLeft) {
        if (currentPosition.x > 0 &&
            tempMatrix[currentPosition.x - 1, currentPosition.y + 1] ==
              TileEx::Empty) {
          tempMatrix[currentPosition.x - 1, currentPosition.y + 1] =
            TileEx::Beam;
          currentPosition.x -= 1;
          currentPosition.y += 1;
          path.push_back(currentPosition);
        } else {
          return std::nullopt;
        }
      } else {
        if (currentPosition.x < tempMatrix.width() - 1 &&
            tempMatrix[currentPosition.x + 1, currentPosition.y + 1] ==
              TileEx::Empty) {
          tempMatrix[currentPosition.x + 1, currentPosition.y + 1] =
            TileEx::Beam;
          currentPosition.x += 1;
          currentPosition.y += 1;
          path.push_back(currentPosition);
        } else {
          return std::nullopt;
        }
      }

    } else {
      return std::nullopt;
    }
  }
  return path;
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
  /*
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
  */
  // matrix.print(print);
  const auto splitters = matrix.find(TileEx::Splitter);
  const auto combinationsCount = 1ULL << splitters.size();
  PositionToSplitterMap splitterMap;
  UniquePaths uniquePaths;
  for (std::size_t i = 0; i < splitters.size(); ++i) {
    Point point = { splitters[i].x, splitters[i].y };
    splitterMap[point] = i;
  }

  std::uint64_t part2 = 0;
  for (std::uint64_t combination = 0; combination < combinationsCount;
       ++combination) {
    const auto pathOpt = trySplitterCombination(
      originalMatrix, combination, Point{ start.x, start.y }, splitterMap);
    if (pathOpt.has_value()) {
      //std::print("Inserting\n");
      //++part2;
      uniquePaths.insert(*pathOpt);
    }
  }
  part2 = uniquePaths.size();
  return { part1, part2 };
}
}
