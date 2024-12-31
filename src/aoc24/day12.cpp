
#include "aoc24/day12.hpp"
#include <algorithm>
#include <compare>
#include <cstddef>
#include <deque>
#include <ranges>
#include <set>
#include <tuple>
#include <vector>

namespace aoc24::day12 {

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

struct Region
{
  std::vector<Point> points;
  std::size_t perimeter;
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

auto
getTotalPrice(const std::vector<Region>& regions)
{
  namespace rng = std::ranges;
  auto prices = regions | rng::views::transform([](const auto& r) {
                  return r.perimeter * r.points.size();
                });
  return rng::fold_left( // NOLINT
    prices,
    0ULL,
    std::plus{});
}

auto
getRegion(Point start, const Input& input)
{

  std::size_t perimeter = 0;
  const auto size = Size{ input.size(), input[0].size() };
  std::deque<Point> toBeVisitedPoints{ start };
  std::set<Point> topsReached{};
  std::vector<Point> points{ start };

  while (!toBeVisitedPoints.empty()) {
    const auto point = toBeVisitedPoints.front();
    toBeVisitedPoints.pop_front();
    for (const auto& neighbour : getNeighbours(point, size)) {
      if (static_cast<unsigned char>(input[point.y][point.x]) + 1 ==
          static_cast<unsigned char>(input[neighbour.y][neighbour.x])) {
        constexpr static auto maxHeight = 9;
        if (input[neighbour.y][neighbour.x] == maxHeight) {
          ++perimeter;
          topsReached.insert(neighbour);
        } else {
          toBeVisitedPoints.push_back(neighbour);
        }
      }
    }
  }
  return Region{ points, perimeter };
}

}

Result
solve(const Input& input)
{
  std::vector<Region> regions{};
  namespace rng = std::ranges;
  const auto size = Size{ input.size(), input[0].size() };
  for (std::size_t y = 0; y < size.height; ++y) {
    for (std::size_t x = 0; x < size.width; ++x) {
      const Point p = { x, y };
      if (rng::none_of(
            regions,
            [&p](const auto& points) {
              return rng::find(points, p) != std::end(points);
            },
            &Region::points)) {
        regions.push_back(getRegion(p, input));
      }
    }
  }
  const std::size_t part1 = getTotalPrice(regions);
  return { part1, part1 };
}

}