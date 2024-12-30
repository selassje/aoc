#include "aoc24/day10.hpp"
#include <compare>
#include <deque>
#include <set>
#include <vector>

namespace aoc24::day10 {

namespace {

struct Point
{
  std::size_t x;
  std::size_t y;
  constexpr std::strong_ordering operator<=>(const Point&) const = default;
};

struct Size
{
  std::size_t height;
  std::size_t width;
};

auto
getNeighbours(Point p, Size size)
{
  std::vector<Point> neighbours{};
  const auto [x, y] = p;
  const auto [height, width] = size;
  if (x > 0) {
    neighbours.emplace_back(x - 1, y);
  }
  if (x < width - 1) {
    neighbours.emplace_back(x + 1, y);
  }
  if (y > 0) {
    neighbours.emplace_back(x, y - 1);
  }
  if (y < height - 1) {
    neighbours.emplace_back(x, y + 1);
  }
  return neighbours;
}

std::size_t
getTrailHeadScore(Point start, const Input& input)
{
  std::size_t score = 0;
  const auto size = Size{ input.size(), input[0].size() };
  std::deque<Point> toBeVisitedPoints{ start };
  std::set<Point> visitedPoints{};
  std::set<Point> topsReached{};

  while (!toBeVisitedPoints.empty()) {
    const auto point = toBeVisitedPoints.front();
    visitedPoints.insert(point);
    toBeVisitedPoints.pop_front();
    for (const auto& neighbour : getNeighbours(point, size)) {
      if (!visitedPoints.contains(neighbour) &&
          static_cast<unsigned char>(input[point.y][point.x]) + 1 ==
            static_cast<unsigned char>(input[neighbour.y][neighbour.x])) {
        if (input[neighbour.y][neighbour.x] == static_cast<std::byte>(9)) {
        //  ++score;
          topsReached.insert(neighbour);
        } else {
          toBeVisitedPoints.push_back(neighbour);
        }
      }
    }
  }
  return topsReached.size();
}

}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  const auto size = Size{ input.size(), input[0].size() };
  for (std::size_t y = 0; y < size.height; ++y) {
    for (std::size_t x = 0; x < size.width; ++x) {
      if (input[y][x] == static_cast<std::byte>(0)) {
         part1 += getTrailHeadScore({ x, y }, input);
      }
    }
  }
  return { part1, part1 };
}

}