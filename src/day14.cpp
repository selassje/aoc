#include "day14.hpp"

#include <cassert>
#include <cmath>
#include <optional>
#include <ranges>

namespace aoc22::day14 {

struct Line
{
  Point start{};
  Point end{};

  constexpr bool isVertical() const { return start.x == end.x; }
};

using Lines = std::vector<Line>;

bool
isPointOnLine(const Point& point, const Line& line)
{
  if (line.isVertical() && point.x == line.start.x) {
    const auto minY = std::min(line.start.y, line.end.y);
    const auto maxY = std::max(line.start.y, line.end.y);
    return minY <= point.y && point.y <= maxY;
  }
  if (!line.isVertical() && point.y == line.start.y) {
    const auto minX = std::min(line.start.x, line.end.x);
    const auto maxX = std::max(line.start.x, line.end.x);
    return minX <= point.x && point.x <= maxX;
  }
  return false;
}

bool
isPointAboveLine(const Point& point, const Line& line)
{
  if (line.isVertical() && point.x == line.start.x) {
    return point.y < line.start.y;
  } else if (!line.isVertical()) {
    const auto minX = std::min(line.start.x, line.end.x);
    const auto maxX = std::max(line.start.x, line.end.x);
    return minX <= point.x && point.x <= maxX && point.y < line.start.y;
  }
  return false;
}

Result
solve(const Input& input)
{
  Lines lines{};
  for (const auto& path : input) {
    for (std::size_t i = 1; i < path.size(); ++i) {
      lines.emplace_back(path[i - 1], path[i]);
    }
  }

  static constexpr Point startPoint = { 500, 0 };
  std::size_t part1 = 0;

  auto isAbyssAhead = [&lines](const Point& point) {
    return std::ranges::find_if(lines, [&point](const Line& line) {
             return isPointAboveLine(point, line);
           }) == lines.end();
  };
  auto isPointBlocked = [&lines](const Point& point) {
    const auto it = std::ranges::find_if(
      lines, [&point](const Line& line) { return isPointOnLine(point, line); });
    return it != lines.end() ? std::to_address(it) : nullptr;
  };

  auto addBlockedPoint = [&lines](Line& line, const Point& point) {
    if (line.isVertical()) {
      assert(line.start.x == point.x);
      if (point.y + 1 == line.start.y) {
        line.start = point;
      } else if (point.y + 1 == line.end.y) {
        line.end = point;
      } else {
        assert(false);
      }
    } else {
      lines.emplace_back(point, point);
    }
  };

  while (true) {
    Point sand = startPoint;
    while (true) {
      if (isAbyssAhead(sand)) {
        goto exit;
      }
      Point below{ sand.x, sand.y + 1 };
      Point belowLeft{ sand.x - 1, sand.y + 1 };
      Point belowRight{ sand.x + 1, sand.y + 1 };

      auto lineBelow = isPointBlocked(below);
      if (lineBelow == nullptr) {
        sand = below;
      } else if (!isPointBlocked(belowLeft)) {
        sand = belowLeft;
      } else if (!isPointBlocked(belowRight)) {
        sand = belowRight;
      } else {
        ++part1;
        assert(lineBelow);
        addBlockedPoint(*lineBelow, sand);
        break;
      }
    }
  }
exit:
  return { part1, part1 };
}
}